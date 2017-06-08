#include <terraces/bipartitions.hpp>
#include <terraces/supertree.hpp>
#include <terraces/union_find.hpp>

#include <iostream>
#include <unordered_map>

namespace terraces {

std::string prefix = "";

size_t count_supertree(index count, const constraints& c) {
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

size_t count_supertree(const tree& tree, const constraints& c) {
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
	return count_supertree(leaves, c_occ, c);
}

size_t count_supertree(const fast_index_set& leaves, const fast_index_set& in_c_occ,
                       const constraints& in_c) {
	size_t number = 0;
	fast_index_set c_occ{in_c_occ.max_size()};
	std::cout << prefix << "leaves {";
	for (auto l : leaves) {
		std::cout << l << ",";
	}
	std::cout << "}\n";

	for (auto c_i : in_c_occ) {
		if (leaves.contains(in_c[c_i].left) && leaves.contains(in_c[c_i].shared) &&
		    leaves.contains(in_c[c_i].right)) {
			c_occ.insert(c_i);
		}
	}
	c_occ.finalize_edit(); // TODO unnecessary!
	std::cout << prefix << "constraints {";
	for (auto cc : c_occ) {
		std::cout << in_c[cc] << ",";
	}
	std::cout << "}\n";

	if (leaves.size() == 2) {
		std::cout << prefix << "result: 1 (direct)\n";
		return 1;
	}

	if (c_occ.size() == 0) {
		size_t res = 1;
		for (size_t i = 3; i <= leaves.size() + 1; i++) {
			res *= (2 * i - 5);
		}
		std::cout << prefix << "result: " << res << " (formula)\n";
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
	std::cout << prefix << "sets: " << set_rep.size() << "\n";

	for (bipartition_iterator bip_it(set_rep.size()); bip_it.is_valid(); bip_it.increase()) {
		std::cout << prefix << "subcall for bipartition " << bip_it << "{";
		fast_index_set subleaves{leaves.max_size()};
		index ii = 0;
		for (auto i : leaves) {
			if (bip_it.get(set_rep.rank(find(sets, ii)))) {
				subleaves.insert(i);
			}
			++ii;
		}
		subleaves.finalize_edit();
		for (auto el : subleaves) {
			std::cout << el << ",";
		}
		std::cout << "}, {";
		for (auto el : leaves) {
			if (!subleaves.contains(el)) {
				std::cout << el << ",";
			}
		}
		std::cout << "}\n";

		prefix += "  ";
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
		prefix = std::string{prefix.begin() + 2, prefix.end()};
		std::cout << prefix << "subcalls returned " << count_left << " and " << count_right
		          << "\n";

		number += count_left * count_right;
		std::cout << prefix << "---------------------------------\n";
	}
	std::cout << prefix << "result: " << number << " (recursive)\n";

	return number;
}

} // namespace terraces
