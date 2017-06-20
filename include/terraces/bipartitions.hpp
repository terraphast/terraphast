#ifndef TERRACES_BIPARTITIONS_HPP
#define TERRACES_BIPARTITIONS_HPP

#include <cmath>

#include "fast_set.hpp"
#include "trees.hpp"
#include "union_find.hpp"

namespace terraces {

class bipartition_iterator {
private:
	const fast_index_set& m_leaves;
	const union_find& m_sets;
	const fast_index_set m_set_rep;
	fast_index_set m_subleaves;

	index m_bip;
	index m_end;

	bool in_left_partition(index i) const;
	fast_index_set find_set_reps() const;

public:
	bipartition_iterator(const fast_index_set& leaves, const union_find& sets);
	/** Moves to the next bipartition. */
	void increase();
	/** Returns true if this bipartition is valid. */
	bool is_valid() const;
	/** Returns the current partition. Switching to the other partition can be done via flip().
	 */
	const fast_index_set& get_current_set() const;
	/** Switches the current partition with the other partition. */
	void flip_sets();

	index cur_bip() const { return m_bip; }
	index end_bip() const { return m_end; }
	const union_find& sets() const { return m_sets; }
	const fast_index_set& leaves() const { return m_leaves; }
};

} // namespace terraces

#endif
