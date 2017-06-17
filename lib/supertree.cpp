#include <terraces/bipartitions.hpp>
#include <terraces/supertree.hpp>
#include <terraces/union_find.hpp>

#include <iostream>
#include <unordered_map>

namespace terraces {

tree_master::tree_master() {}

fast_index_set tree_master::filter_constraints(const fast_index_set& leaves,
                                               const fast_index_set& c_occ,
                                               const constraints& c) const {
	fast_index_set result{c_occ.max_size()};
	for (auto c_i : c_occ) {
		if (leaves.contains(c[c_i].left) && leaves.contains(c[c_i].shared) &&
		    leaves.contains(c[c_i].right)) {
			result.insert(c_i);
		}
	}
	result.finalize_edit();
	return result;
}

union_find tree_master::apply_constraints(const fast_index_set& leaves, const fast_index_set& c_occ,
                                          const constraints& c) const {
	auto sets = union_find(leaves.size());
	for (auto c_i : c_occ) {
		auto& cons = c[c_i];
		sets.merge(leaves.rank(cons.left), leaves.rank(cons.shared));
	}
	return sets;
}

constraints tree_master::map_constraints(const fast_index_set& leaves, const constraints& cs) {
	auto result = cs;
	for (auto& c : result) {
		c.left = leaves.rank(c.left);
		c.shared = leaves.rank(c.shared);
		c.right = leaves.rank(c.right);
	}
	return result;
}

fast_index_set tree_master::full_set(index size) {
	fast_index_set set{size};
	for (index i = 0; i < size; i++) {
		set.insert(i);
	}
	set.finalize_edit();
	return set;
}

fast_index_set tree_master::leave_occ(const tree& tree) {
	fast_index_set leaves{tree.size()};
	for (index i = 0; i < tree.size(); i++) {
		if (is_leaf(tree[i])) {
			leaves.insert(i);
		}
	}
	leaves.finalize_edit();
	return leaves;
}

counted_supertree tree_master::count_supertree(const tree& tree, const constraints& c,
                                               index root_species) {
	auto leaves = leave_occ(tree);
	auto mapped_c = map_constraints(leaves, c);
	auto c_occ = full_set(c.size());
	auto mapped_root = leaves.rank(root_species);
	leaves = full_set(leaves.size());
	leaves.remove(mapped_root);
	leaves.finalize_edit();
	m_stack_state.reserve(leaves.size());
	m_stack_state.clear();

	return tree_master::count_supertree(leaves, c_occ, mapped_c);
}

counted_supertree tree_master::count_supertree(index count, const constraints& c) {
	auto leaves = full_set(count);
	auto c_occ = full_set(c.size());
	leaves.finalize_edit();
	c_occ.finalize_edit();
	m_stack_state.reserve(leaves.size());
	m_stack_state.clear();

	return count_supertree(leaves, c_occ, c);
}

counted_supertree tree_master::count_supertree(const fast_index_set& leaves,
                                               const fast_index_set& in_c_occ,
                                               const constraints& in_c, const name_map& n_map) {
	raii_stackstate state{m_stack_state};
	size_t number = 0;
	std::stringstream ss;

	if (leaves.size() == 2) {
		ss << printable_leaf_pair_representation(leaves, n_map);
		return counted_supertree({1, ss.str()});
	}

	fast_index_set c_occ = tree_master::filter_constraints(leaves, in_c_occ, in_c);
	if (c_occ.size() == 0) {
		size_t res = 1;
		for (size_t i = 3; i <= leaves.size() + 1; i++) {
			res *= (2 * i - 5);
		}
		return counted_supertree({res, ss.str()});
	}

	union_find sets = apply_constraints(leaves, c_occ, in_c);
	bipartition_iterator bip_it(leaves, sets);
	state.init_iteration(bip_it.end_bip());
	while (bip_it.is_valid()) { // TODO: Ask Tobias if I understood this code correctly.
		state.update(bip_it.cur_bip(), number);
		state.go_left();
		std::string left_subtree_repr =
		        printable_tree_representation(bip_it.get_current_set(), n_map);
		index count_left = count_supertree(bip_it.get_current_set(), c_occ, in_c).count;

		bip_it.flip_sets();
		state.go_right();
		std::string right_subtree_repr =
		        printable_tree_representation(bip_it.get_current_set(), n_map);
		index count_right = count_supertree(bip_it.get_current_set(), c_occ, in_c).count;
		number += count_left * count_right;
		ss << "(" << left_subtree_repr << "|" << right_subtree_repr << ")";

		bip_it.increase();
	}
	return counted_supertree({number, ss.str()});
}

size_t tree_master::check_supertree(const fast_index_set& leaves, const fast_index_set& in_c_occ,
                                    const constraints& in_c, index init_number) {
	size_t number = init_number;
	if (leaves.size() == 2) {
		return number + 1;
	}
	fast_index_set c_occ = tree_master::filter_constraints(leaves, in_c_occ, in_c);

	if (c_occ.size() == 0 && number + leaves.size() > 2) {

		return 2;
	}

	union_find sets = apply_constraints(leaves, c_occ, in_c);
	bipartition_iterator bip_it(leaves, sets);
	while (bip_it.is_valid() && number < 2) {
		index count_left = check_supertree(bip_it.get_current_set(), c_occ, in_c, number);
		if (count_left == 0)
			continue;
		if (count_left == 2)
			number = 1;
		bip_it.flip_sets();
		index count_right = check_supertree(bip_it.get_current_set(), c_occ, in_c, number);

		number += count_left * count_right;

		bip_it.increase();
	}
	return number;
}

std::string tree_master::printable_tree_representation(const fast_index_set& leaves,
                                                       const name_map& n_map) {
	bool n_map_given = true;
	if (n_map.size() == 0) {
		n_map_given = false;
	}
	std::stringstream ss;
	bool first;
	for (auto i : leaves) {
		if (!first) {
			ss << ",";
		}
		if (n_map_given) {
			ss << n_map.at(i);
		} else {
			ss << i;
		}
		if (first) {
			first = false;
		}
	}
	return ss.str();
}

std::string tree_master::printable_leaf_pair_representation(const fast_index_set& leaves,
                                                            const name_map& n_map) {
	bool n_map_given = true;
	if (n_map.size() == 0) {
		n_map_given = false;
	}
	std::stringstream ss;
	ss << "(";
	if (n_map_given) {
		ss << n_map.at(*leaves.begin()) << "," << n_map.at(*(++leaves.begin())) << ")";
	} else {
		ss << *leaves.begin() << "," << *(++leaves.begin()) << ")";
	}
	return ss.str();
}

} // namespace terraces
