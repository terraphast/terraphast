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
	result_type run(const tree& t, const constraints& constraints, index root_species);
	result_type run(index count, const constraints& constraints);
	result_type run(const fast_index_set& leaves, const fast_index_set& constraint_occ,
	                const constraints& constraints);
};

template <typename Callback>
auto tree_enumerator<Callback>::run(index count, const constraints& constraints) -> result_type {
	auto leaves = full_set(count);
	auto c_occ = full_set(constraints.size());
	return run(leaves, c_occ, constraints);
}

template <typename Callback>
auto tree_enumerator<Callback>::run(const tree& tree, const constraints& constraints,
                                    index root_species) -> result_type {
	auto leaves = leave_occ(tree);
	auto mapped_constraints = constraints; // map_constraints(leaves, constraints);
	auto c_occ = full_set(constraints.size());
	auto mapped_root = root_species; // leaves.rank(root_species);
	// leaves = full_set(leaves.size());
	leaves.remove(mapped_root);
	leaves.finalize_edit();
	return run(leaves, c_occ, mapped_constraints);
}

template <typename Callback>
auto tree_enumerator<Callback>::run(const fast_index_set& leaves,
                                    const fast_index_set& constraint_occ,
                                    const constraints& constraints) -> result_type {
	result_type result = cb.init_result();
	auto exit_lambda = [&]() { cb.exit(result); };
	utils::scope_guard<decltype(exit_lambda)> guard(exit_lambda);
	cb.enter(leaves);

	// base cases: only a few leaves
	if (leaves.size() == 1) {
		result = cb.base_one_leaf(*leaves.begin());
		return result;
	}

	if (leaves.size() == 2) {
		auto it = leaves.begin();
		auto fst = *it;
		auto snd = *(++it);
		result = cb.base_two_leaves(fst, snd);
		return result;
	}

	fast_index_set new_constraint_occ = filter_constraints(leaves, constraint_occ, constraints);
	// base case: no constraints left
	if (new_constraint_occ.size() == 0) {
		result = cb.base_unconstrained(leaves);
		return result;
	}

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

	return result;
}

} // namespace terraces

#endif // SUPERTREE_ENUMERATOR_HPP
