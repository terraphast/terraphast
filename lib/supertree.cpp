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

size_t tree_master::count_supertree(const tree& tree, const constraints& c, index root_species) {
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

size_t tree_master::count_supertree(index count, const constraints& c) {
	auto leaves = full_set(count);
	auto c_occ = full_set(c.size());
	leaves.finalize_edit();
	c_occ.finalize_edit();
	m_stack_state.reserve(leaves.size());
	m_stack_state.clear();

	return count_supertree(leaves, c_occ, c);
}

size_t tree_master::count_supertree(const fast_index_set& leaves, const fast_index_set& in_c_occ,
                                    const constraints& in_c) {
	raii_stackstate state{m_stack_state};
	size_t number = 0;

	if (leaves.size() == 2) {
		return 1;
	}
	fast_index_set c_occ = tree_master::filter_constraints(leaves, in_c_occ, in_c);
	if (c_occ.size() == 0) {
		size_t res = 1;
		for (size_t i = 3; i <= leaves.size() + 1; i++) {
			res *= (2 * i - 5);
		}
		return res;
	}
	union_find sets = apply_constraints(leaves, c_occ, in_c);
	bipartition_iterator bip_it(leaves, sets);
	state.init_iteration(bip_it.end_bip());
	while (bip_it.is_valid()) {
		state.update(bip_it.cur_bip(), number);
		index count_left = count_supertree(bip_it.get_current_set(), c_occ, in_c);
		bip_it.flip_sets();
		index count_right = count_supertree(bip_it.get_current_set(), c_occ, in_c);
		number += count_left * count_right;

		bip_it.increase();
	}
	return number;
}

} // namespace terraces
