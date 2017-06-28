#ifndef SUPERTREE_ENUMERATOR_HPP
#define SUPERTREE_ENUMERATOR_HPP

#include <terraces/bipartitions.hpp>
#include <terraces/stack_allocator.hpp>
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

	utils::free_list fl1;
	utils::free_list fl2;
	utils::stack_allocator<index> m_leave_allocator;
	utils::stack_allocator<index> m_c_occ_allocator;

	result_type iterate(bipartition_iterator& bip_it, const bitvector& new_constraint_occ,
	                    const constraints& constraints);

public:
	// TODO: use real sizes (otherwise this just falls back to what std::allocator does
	tree_enumerator(Callback cb)
	        : cb{cb}, m_leave_allocator{fl1, 3}, m_c_occ_allocator{fl2, 4} {}
	result_type run(index num_leaves, const constraints& constraints, index root_leaf);
	result_type run(index num_leaves, const constraints& constraints);
	result_type run(const bitvector& leaves, const bitvector& constraint_occ,
	                const constraints& constraints);
};

template <typename Callback>
auto tree_enumerator<Callback>::run(index num_leaves, const constraints& constraints)
        -> result_type {
	auto leaves = full_set(num_leaves, m_leave_allocator);
	auto c_occ = full_set(constraints.size(), m_c_occ_allocator);
	assert(filter_constraints(leaves, c_occ, constraints) == c_occ);
	return run(leaves, c_occ, constraints);
}

template <typename Callback>
auto tree_enumerator<Callback>::run(index num_leaves, const constraints& constraints,
                                    index root_leaf) -> result_type {
	auto leaves = full_set(num_leaves, m_leave_allocator);
	auto c_occ = full_set(constraints.size(), m_c_occ_allocator);
	assert(filter_constraints(leaves, c_occ, constraints) == c_occ);
	// enter the call
	cb.enter(leaves);
	// no base cases
	assert(num_leaves > 2);
	assert(!constraints.empty());
	// build bipartition iterator:
	// TODO: replace those by a centralized free_list:
	auto fl = utils::free_list{};
	auto alloc = utils::stack_allocator<index>{fl, num_leaves};
	auto sets = union_find{num_leaves, alloc};
	index rep = root_leaf == 0 ? 1 : 0;
	// merge all non-root leaves into one set
	for (index i = rep + 1; i < num_leaves; ++i) {
		if (i != root_leaf) {
			sets.merge(rep, i);
		}
	}
	auto bip_it = bipartition_iterator{leaves, sets, m_leave_allocator};
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

	union_find sets =
	        apply_constraints(leaves, new_constraint_occ, constraints, m_leave_allocator);
	bipartition_iterator bip_it(leaves, sets, m_leave_allocator);

	return iterate(bip_it, new_constraint_occ, constraints);
}

template <typename Callback>
auto tree_enumerator<Callback>::iterate(bipartition_iterator& bip_it,
                                        const bitvector& new_constraint_occ,
                                        const constraints& constraints) -> result_type {
	result_type result = cb.init_result();

	cb.begin_iteration(bip_it, new_constraint_occ, constraints);
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
