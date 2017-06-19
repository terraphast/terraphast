#ifndef SUPERTREE_ENUMERATOR_HPP
#define SUPERTREE_ENUMERATOR_HPP

#include <terraces/bipartitions.hpp>
#include <terraces/supertree.hpp>
#include <terraces/union_find.hpp>

#include "supertree_helpers.hpp"
#include "utils.hpp"

namespace terraces {

template <typename Callback>
class tree_enumerator {
	using result_type = typename Callback::result_type;

private:
	Callback cb;

public:
	tree_enumerator(Callback cb) : cb{cb} {}
	result_type run(index num_leaves, const constraints& constraints, index root_leaf);
	result_type run(index num_leaves, const constraints& constraints);
	result_type run(const fast_index_set& leaves, const fast_index_set& constraint_occ,
	                const constraints& constraints);
};

template <typename Callback>
auto tree_enumerator<Callback>::run(index num_leaves, const constraints& constraints)
        -> result_type {
	auto leaves = full_set(num_leaves);
	auto c_occ = full_set(constraints.size());
	return run(leaves, c_occ, constraints);
}

template <typename Callback>
auto tree_enumerator<Callback>::run(index num_leaves, const constraints& constraints,
                                    index root_leaf) -> result_type {
	auto leaves = full_set(num_leaves);
	auto c_occ = full_set(constraints.size());
	leaves.remove(root_leaf);
	leaves.finalize_edit();
	return run(leaves, c_occ, constraints);
}

template <typename Callback>
auto tree_enumerator<Callback>::run(const fast_index_set& leaves,
                                    const fast_index_set& constraint_occ,
                                    const constraints& constraints) -> result_type {
	cb.enter(leaves);

	// base cases: only a few leaves
	if (leaves.size() == 1) {
		return cb.exit(cb.base_one_leaf(*leaves.begin()));
	}

	if (leaves.size() == 2) {
		auto it = leaves.begin();
		auto fst = *it;
		auto snd = *(++it);
		return cb.exit(cb.base_two_leaves(fst, snd));
	}

	fast_index_set new_constraint_occ = filter_constraints(leaves, constraint_occ, constraints);
	// base case: no constraints left
	if (new_constraint_occ.size() == 0) {
		return cb.exit(cb.base_unconstrained(leaves));
	}

	result_type result = cb.init_result();

	union_find sets = apply_constraints(leaves, new_constraint_occ, constraints);
	bipartition_iterator bip_it(leaves, sets);
	cb.begin_iteration(bip_it, new_constraint_occ, constraints);
	// iterate over all possible bipartitions
	while (bip_it.is_valid() && cb.continue_iteration(bip_it)) {
		cb.step_iteration(bip_it);
		cb.left_subcall();
		auto left = run(bip_it.get_current_set(), new_constraint_occ, constraints);

		bip_it.flip_sets();
		cb.right_subcall();
		auto right = run(bip_it.get_current_set(), new_constraint_occ, constraints);

		// accumulate result
		result = cb.accumulate(result, cb.combine(left, right));

		bip_it.increase();
	}
	cb.finish_iteration();

	return cb.exit(result);
}

} // namespace terraces

#endif // SUPERTREE_ENUMERATOR_HPP
