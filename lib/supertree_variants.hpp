#ifndef SUPERTREE_VARIANTS_HPP
#define SUPERTREE_VARIANTS_HPP

#include <terraces/bipartitions.hpp>
#include <terraces/constraints.hpp>
#include <terraces/trees.hpp>

#include <gmpxx.h>

namespace terraces {

template <typename Result>
class enumeration_callback {
public:
	using result_type = Result;

	/** Called when a (sub)call begins. */
	void enter(const fast_index_set&) {}
	/** Called when a (sub)call finishes. */
	void exit(Result) {}

	/** Returns the initial value of the accumulator. */
	Result init_result() { return Result{}; }
	/** Returns the result for a single leaf. */
	Result base_one_leaf(index) { return Result{}; }
	/** Returns the result for two leaves. */
	Result base_two_leaves(index, index) { return Result{}; }
	/** Returns the result for multiple leaves without constraints. */
	Result base_unconstrained(const fast_index_set&) { return Result{}; }

	/** Called when we begin iterating over the bipartitions. */
	void begin_iteration(const bipartition_iterator&, const fast_index_set&,
	                     const constraints&) {}
	/** Returns true iff the iteration should continue. */
	bool continue_iteration(const bipartition_iterator&) { return true; }
	/** Called when an iteration step begins. */
	void step_iteration(const bipartition_iterator&) {}
	/** Called when the last iteration step has finished. */
	void finish_iteration() {}
	/** Called before descending into the left subset. */
	void left_subcall() {}
	/** Called before descending into the right subset. */
	void right_subcall() {}
	/** Accumulates the results from multiple bipartitions. */
	Result accumulate(Result, Result) {}
	/** Combines the results from two subcalls. */
	Result combine(Result, Result) {}
};

class tree_count_callback : public enumeration_callback<mpz_class> {
public:
	mpz_class base_one_leaf(index) { return 1; }
	mpz_class base_two_leaves(index, index) { return 1; }
	mpz_class base_unconstrained(const fast_index_set& leaves) {
		index num_leaves = leaves.size();
		mpz_class result = 1;
		for (index i = 3; i <= num_leaves + 1; i++) {
			result *= (2 * i - 5);
		}
		return result;
	}

	mpz_class accumulate(mpz_class acc, mpz_class val) { return acc + val; }
	mpz_class combine(mpz_class left, mpz_class right) { return left * right; }
};

} // namespace terraces

#endif // SUPERTREE_VARIANTS_HPP
