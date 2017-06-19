#ifndef SUPERTREE_VARIANTS_HPP
#define SUPERTREE_VARIANTS_HPP

#include <terraces/bipartitions.hpp>
#include <terraces/constraints.hpp>
#include <terraces/trees.hpp>

#include <gmpxx.h>

#include "utils.hpp"

namespace terraces {

template <typename Result>
class enumeration_callback {
public:
	using result_type = Result;

	/** Called when a (sub)call begins. */
	void enter(const fast_index_set&) {}
	/** Called when a (sub)call finishes. */
	Result exit(Result val) { return val; }

	/** Returns the initial value of the accumulator. */
	Result init_result() { return Result{}; }
	/** Returns the result for a single leaf. */
	Result base_one_leaf(index);
	/** Returns the result for two leaves. */
	Result base_two_leaves(index, index);
	/** Returns the result for multiple leaves without constraints. */
	Result base_unconstrained(const fast_index_set&);

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
	Result accumulate(Result, Result);
	/** Combines the results from two subcalls. */
	Result combine(Result, Result);
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

class multitree_callback : public enumeration_callback<std::ostream*> {
private:
	std::ostream* m_stream;
	const name_map& m_names;
	bool m_first_iteration;

public:
	multitree_callback(std::ostream& stream, const name_map& names)
	        : m_stream{&stream}, m_names{names} {}

	std::ostream* base_one_leaf(index i) { return &(*m_stream << m_names[i]); }
	std::ostream* base_two_leaves(index i, index j) {
		return &(*m_stream << "(" << m_names[i] << "," << m_names[j] << ")");
	}
	std::ostream* base_unconstrained(const fast_index_set& leaves) {
		return &(*m_stream << "{" << utils::as_comma_separated_output(leaves, m_names)
		                   << "}");
	}

	void right_subcall() { *m_stream << ","; }

	void begin_iteration(const bipartition_iterator&, const fast_index_set&,
	                     const constraints&) {
		m_first_iteration = true;
	}

	void step_iteration(const bipartition_iterator&) {
		if (not m_first_iteration) {
			*m_stream << "|";
		}

		m_first_iteration = false;
	}

	void left_subcall() { *m_stream << "("; }

	std::ostream* accumulate(std::ostream*, std::ostream*) { return m_stream; }

	std::ostream* combine(std::ostream*, std::ostream*) { return &(*m_stream << ")"); }
};

} // namespace terraces

#endif // SUPERTREE_VARIANTS_HPP
