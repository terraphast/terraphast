#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <cstdint>
#include <terraces/trees.hpp>
#include <vector>

namespace terraces {
namespace efficient {

index block_index(index i);
index base_index(index block);
uint8_t shift_index(index i);
uint64_t set_mask(index i);
uint64_t clear_mask(index i);
uint64_t prefix_mask(index i);
uint8_t popcount(uint64_t block);
uint8_t partial_popcount(uint64_t block, index i);
uint8_t byte_select(uint8_t byte, uint8_t i);
uint8_t block_select(uint64_t block, uint8_t i);

class bitvector {
private:
	index m_size;
	// TODO align blocks for SSE
	std::vector<uint64_t> m_blocks;
	std::vector<index> m_ranks;
	bool m_ranks_dirty;

public:
	/** Initializes a bitvector with given size. */
	bitvector(index size);
	/** Sets a bit in the bitvector. */
	void set(index i);
	/** Clears a bit in the bitvector. */
	void clr(index i);
	/** Returns a bit from the bitvector. */
	bool get(index i) const;

	/** Updates the internal data structures after editing the vector. */
	void update_ranks();
	/** Returns the rank of an index, i.e. the number of set bits in the range [0..i) */
	index rank(index i) const;
	/** Return the index of the j-th set bit (0-based), i.e. the lowest i s.t. rank(i + 1) = j -
	 * 1. */
	index select(index j) const;
	/** Return the index of the next set bit, i.e. select(rank(i + 1)). */
	index next(index i) const;
};

} // namespace efficient

namespace naive {

class bitvector {
private:
	std::vector<bool> m_vector;

public:
	/** Initializes a bitvector with given size. */
	bitvector(index size);
	/** Sets a bit in the bitvector. */
	void set(index i);
	/** Clears a bit in the bitvector. */
	void clr(index i);
	/** Returns a bit from the bitvector. */
	bool get(index i) const;

	/** Updates the internal data structures after editing the vector. */
	void update_ranks();
	/** Returns the rank of an index, i.e. the number of set bits in the range [0..i) */
	index rank(index i) const;
	/** Return the index of the j-th set bit (0-based), i.e. the lowest i s.t. rank(i + 1) = j -
	 * 1. */
	index select(index j) const;
	/** Return the index of the next set bit, i.e. select(rank(i + 1)). */
	index next(index i) const;
};

} // namespace naive
} // namespace terraces

#endif // BITVECTOR_H
