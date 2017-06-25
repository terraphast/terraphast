#ifndef SUPERTREE_ENUMERATOR_HPP
#define SUPERTREE_ENUMERATOR_HPP

#include <terraces/bipartitions.hpp>
#include <terraces/multitree.hpp>
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

	result_type iterate(bipartition_iterator& bip_it, const bitvector& new_constraint_occ,
	                    const constraints& constraints);

public:
	tree_enumerator(Callback cb) : cb{cb} {}
	result_type run(index num_leaves, const constraints& constraints, index root_leaf);
	result_type run(index num_leaves, const constraints& constraints);
	result_type run(const bitvector& leaves, const bitvector& constraint_occ,
	                const constraints& constraints);
};

template <typename Callback>
auto tree_enumerator<Callback>::run(index num_leaves, const constraints& constraints)
        -> result_type {
	auto leaves = full_set(num_leaves);
	auto c_occ = full_set(constraints.size());
	assert(filter_constraints(leaves, c_occ, constraints) == c_occ);
	return run(leaves, c_occ, constraints);
}

template <typename Callback>
auto tree_enumerator<Callback>::run(index num_leaves, const constraints& constraints,
                                    index root_leaf) -> result_type {
	auto leaves = full_set(num_leaves);
	auto c_occ = full_set(constraints.size());
	assert(filter_constraints(leaves, c_occ, constraints) == c_occ);
	// enter the call
	cb.enter(leaves);
	// no base cases
	assert(num_leaves > 2);
	assert(!constraints.empty());
	// build bipartition iterator:
	auto sets = union_find{num_leaves};
	index rep = root_leaf == 0 ? 1 : 0;
	// merge all non-root leaves into one set
	for (index i = rep + 1; i < num_leaves; ++i) {
		if (i != root_leaf) {
			sets.merge(rep, i);
		}
	}
	auto bip_it = bipartition_iterator{leaves, sets};
	return cb.exit(iterate(bip_it, c_occ, constraints));
}

template <typename Callback>
auto tree_enumerator<Callback>::run(const bitvector& leaves, const bitvector& constraint_occ,
                                    const constraints& constraints) -> result_type {
	cb.enter(leaves);

	// base cases: only a few leaves
	assert(leaves.count() > 0);
	if (leaves.count() == 1) {
		return cb.exit(cb.base_one_leaf(leaves.first_set()));
	}

	if (leaves.count() == 2) {
		auto fst = leaves.first_set();
		auto snd = leaves.next_set(fst);
		return cb.exit(cb.base_two_leaves(fst, snd));
	}

	bitvector new_constraint_occ = filter_constraints(leaves, constraint_occ, constraints);
	// base case: no constraints left
	if (new_constraint_occ.count() == 0) {
		return cb.exit(cb.base_unconstrained(leaves));
	}

	union_find sets = apply_constraints(leaves, new_constraint_occ, constraints);
	bipartition_iterator bip_it(leaves, sets);

	return iterate(bip_it, new_constraint_occ, constraints);
}

template <typename Callback>
auto tree_enumerator<Callback>::iterate(bipartition_iterator& bip_it,
                                        const bitvector& new_constraint_occ,
                                        const constraints& constraints) -> result_type {
	if (cb.fast_return(bip_it)) {
		return cb.fast_return_value(bip_it);
	}

	auto result = cb.begin_iteration(bip_it, new_constraint_occ, constraints);
	// iterate over all possible bipartitions
	while (bip_it.is_valid() && cb.continue_iteration(result)) {
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
