#include <terraces/bipartitions.hpp>
#include <terraces/supertree.hpp>
#include <terraces/union_find.hpp>

#include <iostream>
#include <sstream>
#include <unordered_map>

namespace terraces {

tree_master::tree_master(){};

size_t tree_master::count_supertree(const tree& tree, const constraints& c) {
	fast_index_set leaves{tree.size()};
	for (index i = 0; i < tree.size(); i++) {
		if (is_leaf(tree[i])) {
			leaves.insert(i);
		}
	}
	fast_index_set c_occ{c.size()};
	for (index i = 0; i < c.size(); ++i) {
		c_occ.insert(i);
	}

	leaves.finalize_edit();
	c_occ.finalize_edit();
	return tree_master::count_supertree(leaves, c_occ, c);
}

size_t tree_master::count_supertree(index count, const constraints& c) {
	fast_index_set leaves{count};
	for (index i = 0; i < count; i++) {
		leaves.insert(i);
	}
	fast_index_set c_occ{c.size()};
	for (index i = 0; i < c.size(); ++i) {
		c_occ.insert(i);
	}

	leaves.finalize_edit();
	c_occ.finalize_edit();
	return count_supertree(leaves, c_occ, c);
}

size_t tree_master::count_supertree(const fast_index_set& leaves, const fast_index_set& in_c_occ,
                                    const constraints& in_c) {
	size_t number = 0;
	std::stringstream ss;
	fast_index_set c_occ{in_c_occ.max_size()}; // Constraint_OCCurrence
	for (auto c_i : in_c_occ) {                // Constraint_Index
		if (leaves.contains(in_c[c_i].left) && leaves.contains(in_c[c_i].shared) &&
		    leaves.contains(in_c[c_i].right)) {
			c_occ.insert(c_i);
		}
	}
	c_occ.finalize_edit(); // TODO unnecessary!
	if (leaves.size() == 2) {
		return 1;
	}
	if (c_occ.size() == 0) {
		size_t res = 1;
		for (size_t i = 3; i <= leaves.size() + 1; i++) {
			res *= (2 * i - 5);
		}
		return res;
	}
	union_find sets = make_set(leaves.size());
	for (auto c_i : c_occ) {
		auto& c = in_c[c_i];
		merge(sets, leaves.rank(c.left), leaves.rank(c.shared));
	}
	fast_index_set set_rep(leaves.size());
	for (index i = 0; i < leaves.size(); ++i) {
		set_rep.insert(find(sets, i));
	}
	set_rep.finalize_edit();
	for (bipartition_iterator bip_it(set_rep.size()); bip_it.is_valid(); bip_it.increase()) {
		fast_index_set subleaves{leaves.max_size()};
		index ii = 0;
		for (auto i : leaves) {
			if (bip_it.get(set_rep.rank(find(sets, ii)))) {
				subleaves.insert(i);
			}
			++ii;
		}
		subleaves.finalize_edit();
		index count_left = count_supertree(subleaves, c_occ, in_c);
		for (auto i : leaves) {
			if (subleaves.contains(i)) {
				subleaves.remove(i);
			} else {
				subleaves.insert(i);
			}
		}
		subleaves.finalize_edit();
		index count_right = count_supertree(subleaves, c_occ, in_c);
		number += count_left * count_right;
	}
	return number;
}

std::string tree_master::printable_tree_representation(const fast_index_set& leaves,
                                                       const name_map& n_map) {
	std::stringstream ss;
	bool first;
	for (auto i : leaves) {
		if (!first) {
			ss << ",";
		}
		ss << n_map.at(i);
		if (first) {
			first = false;
		}
	}
	return ss.str();
}

std::string tree_master::printable_leaf_pair_representation(const fast_index_set& leaves,
                                                            const name_map& n_map) {
	std::stringstream ss;
	ss << "{";
	bool first = true;
	for (auto i : leaves) {
		if (!first) {
			ss << ",";
		}
		ss << n_map.at(i);
		if (first) {
			first = false;
		}
	}
	ss << "}";
	return ss.str();
}

counted_supertree tree_master::enumerate_supertrees(const fast_index_set& leaves,
                                                    const fast_index_set& in_c_occ,
                                                    const constraints& in_c,
                                                    const name_map& n_map) {

	// TODO: TEST OUTPUT CODE

	size_t number = 0;
	std::stringstream ss;
	fast_index_set c_occ{in_c_occ.max_size()}; // Constraint_OCCurrence
	for (auto c_i : in_c_occ) {                // Constraint_Index
		if (leaves.contains(in_c[c_i].left) && leaves.contains(in_c[c_i].shared) &&
		    leaves.contains(in_c[c_i].right)) {
			c_occ.insert(c_i);
		}
	}
	c_occ.finalize_edit(); // TODO unnecessary!
	if (leaves.size() == 2) {
		ss << "(";
		ss << n_map.at(*leaves.begin());
		ss << ",";
		ss << n_map.at(*(++leaves.begin()));
		ss << ")";
		index count = 1;
		return counted_supertree({count, ss.str()});
	}
	if (c_occ.size() == 0) {
		size_t res = 1;
		for (size_t i = 3; i <= leaves.size() + 1; i++) {
			res *= (2 * i - 5);
		}
		ss << printable_leaf_pair_representation(leaves, n_map);
		return counted_supertree({res, ss.str()});
	}
	union_find sets = make_set(leaves.size());
	for (auto c_i : c_occ) {
		auto& c = in_c[c_i];
		merge(sets, leaves.rank(c.left), leaves.rank(c.shared));
	}
	fast_index_set set_rep(leaves.size());
	for (index i = 0; i < leaves.size(); ++i) {
		set_rep.insert(find(sets, i));
	}
	set_rep.finalize_edit();
	for (bipartition_iterator bip_it(set_rep.size()); bip_it.is_valid(); bip_it.increase()) {
		fast_index_set subleaves{leaves.max_size()};
		index ii = 0;
		for (auto i : leaves) {
			if (bip_it.get(set_rep.rank(find(sets, ii)))) {
				subleaves.insert(i);
			}
			++ii;
		}
		subleaves.finalize_edit();
		std::string left_subtree_repr = printable_tree_representation(subleaves, n_map);

		counted_supertree left_subtree =
		        enumerate_supertrees(subleaves, c_occ, in_c, n_map);
		for (auto i : leaves) {
			if (subleaves.contains(i)) {
				subleaves.remove(i);
			} else {
				subleaves.insert(i);
			}
		}
		subleaves.finalize_edit();
		std::string right_subtree_repr = printable_tree_representation(subleaves, n_map);

		counted_supertree right_subtree =
		        enumerate_supertrees(subleaves, c_occ, in_c, n_map);

		number += left_subtree.count * right_subtree.count;
		ss << "(" << left_subtree_repr << "|" << right_subtree_repr << ")";
	}
	return counted_supertree({number, ss.str()});
}

} // namespace terraces
