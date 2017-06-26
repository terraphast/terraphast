#ifndef UNCONSTRAINED_ENUMERATOR_CPP
#define UNCONSTRAINED_ENUMERATOR_CPP

#include <terraces/intrinsics.hpp>
#include <terraces/multitree.hpp>

namespace terraces {

using bits::bitscan;
using bits::rbitscan;
using bits::popcount;

struct small_bipartition {
	const index m_mask;
	const index m_end_bip;
	index m_cur_bip;

	small_bipartition(index mask)
	        : m_mask{mask}, m_end_bip{1ull << rbitscan(mask)},
	          m_cur_bip{1ull << bitscan(mask)} {}

	bool is_valid() const { return m_cur_bip < m_end_bip; }
	void next() {
		assert(is_valid());
		// credit goes to zch
		// (see stackoverflow.com/questions/44767080/incrementing-masked-bitsets)
		m_cur_bip = ((m_cur_bip | ~m_mask) + 1) & m_mask;
	}
	index mask() const { return m_mask; }
	index left_mask() const { return m_cur_bip; }
	index right_mask() const { return m_cur_bip ^ m_mask; }
	index leftmost_leaf() const { return bitscan(m_mask); }
	index rightmost_leaf() const { return rbitscan(m_mask); }
	index num_leaves() const { return popcount(m_mask); }
};
}

#endif // UNCONSTRAINED_ENUMERATOR_CPP
