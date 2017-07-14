#ifndef SUPERTREE_VARIANTS_HPP
#define SUPERTREE_VARIANTS_HPP

#include <terraces/constraints.hpp>
#include <terraces/io_utils.hpp>
#include <terraces/trees.hpp>

#include <gmpxx.h>

#include "bipartitions.hpp"

namespace terraces {
namespace variants {

template <typename Result>
class abstract_callback {
public:
	using result_type = Result;

	/** Called when a (sub)call begins. */
	void enter(const ranked_bitvector&) {}
	/** Called when a (sub)call finishes. */
	Result exit(Result val) { return val; }

	/** Returns the result for a single leaf. */
	Result base_one_leaf(index);
	/** Returns the result for two leaves. */
	Result base_two_leaves(index, index);
	/** Returns the result for multiple leaves without constraints. */
	Result base_unconstrained(const ranked_bitvector&);
	/** Returns an empty result. */
	Result null_result();

	bool fast_return(const bipartition_iterator&) { return false; }
	Result fast_return_value(const bipartition_iterator&);

	/** Called when we begin iterating over the bipartitions. */
	Result begin_iteration(const bipartition_iterator&, const bitvector&, const constraints&) {
		return Result{};
	}
	/** Returns true iff the iteration should continue. */
	bool continue_iteration(Result) { return true; }
	/** Called when an iteration step begins. */
	void step_iteration(const bipartition_iterator&) {}
	/** Called when the last iteration step has finished. */
	void finish_iteration() {}
	/** Called before descending into the left subset. */
	void left_subcall() {}
	/** Called before descending into the right subset. */
	void right_subcall() {}
	/** Accumulates the results from multiple bipartitions. */
	Result accumulate(Result, Result);
	/** Combines the results from two subcalls. */
	Result combine(Result, Result);
};

class count_callback : public abstract_callback<mpz_class> {
public:
	using return_type = abstract_callback::result_type;
	return_type base_one_leaf(index) { return 1; }
	return_type base_two_leaves(index, index) { return 1; }
	return_type base_unconstrained(const ranked_bitvector& leaves) {
		return count_unrooted_trees<return_type>(leaves.count());
	}
	return_type null_result() { return 0; }

	index fast_return_value(const bipartition_iterator& bip_it) { return bip_it.num_bip(); }

	return_type accumulate(return_type acc, return_type val) { return acc + val; }
	return_type combine(return_type left, return_type right) { return left * right; }
};

class check_callback : public abstract_callback<index> {
public:
	using return_type = index;

	index base_one_leaf(index) { return 1; }
	index base_two_leaves(index, index) { return 1; }
	index base_unconstrained(const ranked_bitvector&) { return 2; }
	return_type null_result() { return 0; }

	index fast_return_value(const bipartition_iterator& bip_it) { return bip_it.num_bip(); }

	bool continue_iteration(index acc) { return acc < 2; }

	index accumulate(index acc, index val) { return acc + val; }
	index combine(index left, index right) { return left * right; }
};

template <typename Callback>
class fast_check_decorator : public Callback {
public:
	using return_type = typename Callback::result_type;

	bool fast_return(const bipartition_iterator& bip_it) { return bip_it.num_bip() > 1; }
};

} // namespace variants
} // namespace terraces

#endif // SUPERTREE_VARIANTS_HPP
