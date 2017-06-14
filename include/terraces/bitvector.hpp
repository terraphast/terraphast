#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <cstdint>
#include <terraces/trees.hpp>
#include <vector>

#ifdef NDEBUG
#define debug_code(x)                                                                              \
	do {                                                                                       \
	} while (0)
#define debug_var(x) static_assert(true, "")
#else
#define debug_code(x) x
#define debug_var(x) x
#endif

namespace terraces {
namespace efficient {

inline index block_index(index i) { return i / 64; }
inline index base_index(index block) { return block * 64; }
inline uint8_t shift_index(index i) { return i % 64; }
inline uint64_t set_mask(index i) { return 1ull << (i & 63); }
inline uint64_t clear_mask(index i) { return ~set_mask(i); }
inline uint64_t prefix_mask(index i) { return set_mask(i) - 1; }
inline index next_bit(uint64_t block, index i) {
	unsigned long idx;
	_BitScanForward64(&idx, block >> shift_index(i));
	return i + (index)idx;
}
inline index next_bit0(uint64_t block, index i) { return i + (index)__builtin_ctzll(block); }
inline bool has_next_bit(uint64_t block, index i) { return (block >> shift_index(i)) != 0; }
inline bool has_next_bit0(uint64_t block) { return block != 0; }
inline uint8_t popcount(uint64_t block) { return (uint8_t)__popcnt64(block); }
inline uint8_t partial_popcount(uint64_t block, index i) {
	return popcount(block & prefix_mask(i));
}

class bitvector {
private:
	index m_size;
	// TODO align blocks for SSE
	std::vector<uint64_t> m_blocks;
	std::vector<index> m_ranks;
	index m_count;

	debug_var(bool m_ranks_dirty);

	void add_sentinel();

public:
	/** Initializes a bitvector with given size. */
	bitvector(index size);
	/** Sets a bit in the bitvector. */
	void set(index i) {
		assert(i < m_size);
		m_blocks[block_index(i)] |= set_mask(i);
		debug_code(m_ranks_dirty = true);
	}
	/** Clears a bit in the bitvector. */
	void clr(index i) {
		assert(i < m_size);
		m_blocks[block_index(i)] &= clear_mask(i);
		debug_code(m_ranks_dirty = true);
	}
	/** Flips a bit in the bitvector. */
	void flip(index i) {
		assert(i < m_size);
		m_blocks[block_index(i)] ^= set_mask(i);
		debug_code(m_ranks_dirty = true);
	}
	/** Returns a bit from the bitvector. */
	bool get(index i) const {
		assert(i < m_size);
		return (m_blocks[block_index(i)] >> shift_index(i)) & 1;
	}
	/** Returns the size of the bitvector. */
	index size() const { return m_size; }

	/** Clears all bits in the bitvector. */
	void blank();
	/** Applies element-wise xor from another bitvector. */
	void bitwise_xor(const bitvector& other);

	/** Updates the internal data structures after editing the vector. */
	void update_ranks();
	/** Returns the rank of an index, i.e. the number of set bits in the range [0..i) */
	index rank(index i) const {
		assert(!m_ranks_dirty);
		assert(i <= m_size);
		index b = block_index(i);
		return m_ranks[b] + partial_popcount(m_blocks[b], shift_index(i));
	}
	/** Returns the number of set bits. */
	index count() const {
		assert(!m_ranks_dirty);
		return m_count - 1;
	}
	/** Returns the index of the first set bit or size() if no bit is set. */
	index begin() const {
		index b = 0;
		while (!has_next_bit0(m_blocks[b])) {
			++b;
		}
		return next_bit0(m_blocks[b], base_index(b));
	}
	/** Returns the index of the next set bit after the index or size() if no bit is set. */
	index next(index i) const {
		++i;
		index b = block_index(i);
		if (has_next_bit(m_blocks[b], i)) {
			// the next bit is in the current block
			return next_bit(m_blocks[b], i);
		} else {
			// the next bit is in a far-away block
			do {
				++b;
			} while (!has_next_bit0(m_blocks[b]));
			return next_bit0(m_blocks[b], base_index(b));
		}
	}
	/** Returns the index one past the last element. */
	index end() const { return m_size; }
};

} // namespace efficient
} // namespace terraces

#endif // BITVECTOR_H
