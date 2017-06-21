#include "supertree_helpers.hpp"

namespace terraces {

fast_index_set filter_constraints(const fast_index_set& leaves, const fast_index_set& c_occ,
                                  const constraints& c) {
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

union_find apply_constraints(const fast_index_set& leaves, const fast_index_set& c_occ,
                             const constraints& c) {
	auto sets = union_find(leaves.size());
	for (auto c_i : c_occ) {
		auto& cons = c[c_i];
		sets.merge(leaves.rank(cons.left), leaves.rank(cons.shared));
	}
	return sets;
}

constraints map_constraints(const fast_index_set& leaves, const constraints& cs) {
	auto result = cs;
	for (auto& c : result) {
		c.left = leaves.rank(c.left);
		c.shared = leaves.rank(c.shared);
		c.right = leaves.rank(c.right);
	}
	return result;
}

fast_index_set leave_occ(const tree& tree) {
	fast_index_set leaves{tree.size()};
	for (index i = 0; i < tree.size(); i++) {
		if (is_leaf(tree[i])) {
			leaves.insert(i);
		}
	}
	leaves.finalize_edit();
	return leaves;
}

} // namespace terraces
