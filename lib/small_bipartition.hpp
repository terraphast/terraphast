#ifndef UNCONSTRAINED_ENUMERATOR_CPP
#define UNCONSTRAINED_ENUMERATOR_CPP

#include <terraces/multitree.hpp>

#include <bitset>
#include <cassert>
#include <vector>

#include "bits.hpp"

namespace terraces {

using bits::bitscan;
using bits::rbitscan;
using bits::popcount;

struct small_bipartition {
	index m_mask;
	index m_cur_bip;

	small_bipartition(index mask = 1) : m_mask{mask} { reset(); }

	// credit goes to nglee
	// (see stackoverflow.com/questions/44767080/incrementing-masked-bitsets)
	index masked_increment(index bip) const { return -(bip ^ m_mask) & m_mask; }

	bool is_valid() const { return (m_cur_bip >> rbitscan(m_mask)) == 0; }

	void next() {
		assert(is_valid());
		m_cur_bip = masked_increment(m_cur_bip);
	}
	void reset() { m_cur_bip = 1ull << bitscan(m_mask); }

	index mask() const { return m_mask; }
	index left_mask() const { return m_cur_bip; }
	index right_mask() const { return m_cur_bip ^ m_mask; }
	index leftmost_leaf() const { return bitscan(m_mask); }
	index rightmost_leaf() const { return rbitscan(m_mask); }
	index num_leaves() const { return popcount(m_mask); }

	static small_bipartition full_set(index num_leaves) {
		assert(num_leaves < bits::word_bits);
		return {(1ull << num_leaves) - 1};
	}
};
} // namespace terraces

#endif // UNCONSTRAINED_ENUMERATOR_CPP
