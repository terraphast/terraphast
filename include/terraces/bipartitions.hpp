#ifndef TERRACES_BIPARTITIONS_HPP
#define TERRACES_BIPARTITIONS_HPP

#include <cmath>
#include <iosfwd>

#include "ranked_bitvector.hpp"
#include "trees.hpp"
#include "union_find.hpp"

namespace terraces {

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
	ranked_bitvector find_set_reps() const;

public:
	bipartition_iterator(const ranked_bitvector& leaves, const union_find& sets,
	                     utils::stack_allocator<index>);
	/** Moves to the next bipartition. */
	void increase();
	/** Returns true if this bipartition is valid. */
	bool is_valid() const;
	/** Returns the current partition. Switching to the other partition can be done via flip().
	 */
	const ranked_bitvector& get_current_set() const;
	/** Switches the current partition with the other partition. */
	void flip_sets();

	index cur_bip() const { return m_bip; }
	index end_bip() const { return m_end; }
	index num_bip() const { return m_end - 1; }
	const union_find& sets() const { return m_sets; }
	const ranked_bitvector& leaves() const { return m_leaves; }
};

std::ostream& operator<<(std::ostream& stream, const bipartition_iterator& it);

} // namespace terraces

#endif
