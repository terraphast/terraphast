#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <cstdint>
#include <terraces/trees.hpp>
#include <vector>

namespace terraces {
namespace efficient {

index block_index(index i);
uint8_t shift_index(index i);
uint64_t set_mask(index i);
uint64_t clear_mask(index i);
uint64_t prefix_mask(index i);
index next_bit(uint64_t block, index i);
bool has_next_bit(uint64_t block, index i);
uint8_t popcount(uint64_t block);
uint8_t partial_popcount(uint64_t block, index i);

class bitvector {
private:
	index m_size;
	// TODO align blocks for SSE
	std::vector<uint64_t> m_blocks;
	std::vector<index> m_ranks;
	index m_count;
	bool m_ranks_dirty;

public:
	/** Initializes a bitvector with given size. */
	bitvector(index size);
	/** Sets a bit in the bitvector. */
	void set(index i);
	/** Clears a bit in the bitvector. */
	void clr(index i);
	/** Clears all bit in the bitvector. */
	void blank();
	/** Returns a bit from the bitvector. */
	bool get(index i) const;
	/** Returns the size of the bitvector. */
	index size() const;

	/** Updates the internal data structures after editing the vector. */
	void update_ranks();
	/** Returns the rank of an index, i.e. the number of set bits in the range [0..i) */
	index rank(index i) const;
	/** Returns the number of set bits. */
	index count() const;
	/** Returns the index of the first set bit or size() if no bit is set. */
	index begin() const;
	/** Returns the index of the next set bit after the index or size() if no bit is set. */
	index next(index i) const;
	/** Returns the index one past the last element. */
	index end() const;
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
	/** Returns the size of the bitvector. */
	index size() const;

	/** Updates the internal data structures after editing the vector. */
	void update_ranks();
	/** Returns the rank of an index, i.e. the number of set bits in the range [0..i) */
	index rank(index i) const;
	/** Returns the number of set bits. */
	index count() const;
	/** Returns the index of the first set bit or size() if no bit is set. */
	index begin() const;
	/** Returns the index of the next set bit after the index or size() if no bit is set. */
	index next(index i) const;
	/** Returns the index one past the last element. */
	index end() const;
};

} // namespace naive
} // namespace terraces

#endif // BITVECTOR_H
