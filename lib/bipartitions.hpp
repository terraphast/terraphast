#ifndef TERRACES_BIPARTITIONS_HPP
#define TERRACES_BIPARTITIONS_HPP

#include <cmath>
#include <iosfwd>

#include <terraces/trees.hpp>

#include "ranked_bitvector.hpp"
#include "union_find.hpp"

namespace terraces {

/**
 * An iterator enumerating all possible bipartition from a given union-find representation of leaf
 * sets.
 */
class bipartition_iterator {
private:
	utils::stack_allocator<index> m_alloc;
	const ranked_bitvector& m_leaves;
	const union_find& m_sets;
	const ranked_bitvector m_set_rep;
	ranked_bitvector m_subleaves;

	index m_bip;
	index m_end;

	bool in_left_partition(index i) const;
	/** Returns a bitvector containing a 1 for every set representative in the union-find
	 * structure. */
	ranked_bitvector find_set_reps() const;

public:
	bipartition_iterator(const ranked_bitvector& leaves, const union_find& sets,
	                     utils::stack_allocator<index>);
	/** Moves to the next bipartition. */
	void increase();
	/** Returns true if and only if this bipartition is valid. */
	bool is_valid() const;
	/**
	 * Returns the elements of the current partition.
	 * Switching to the other partition can be done via \ref flip_sets.
	 * After a call to \ref increase this always returns the left set.
	     */
	const ranked_bitvector& get_current_set() const;
	/** Switches the current partition with the other partition. */
	void flip_sets();

	/**
	 * Returns a binary representation of the current bipartition.
	 * A 1 at the (i - 1)th position means that the ith leaf is in the left set.
	 * The 0th leaf is always in the right set.
	 */
	index cur_bip() const { return m_bip; }
	/**
	 * Returns a binary representation of the first invalid bipartition.
	 */
	index end_bip() const { return m_end; }
	/** Returns the number of bipartitions. */
	index num_bip() const { return m_end - 1; }
	/** Returns the union-find representation of the leaf sets. */
	const union_find& sets() const { return m_sets; }
	/** Returns the leaves on which the union-find representation is based. */
	const ranked_bitvector& leaves() const { return m_leaves; }
};

/** Prints a binary representation of the given bipartition to the given stream. */
std::ostream& operator<<(std::ostream& stream, const bipartition_iterator& it);

} // namespace terraces

#endif
