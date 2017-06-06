#include <terraces/bitvector.hpp>

#include <algorithm>

#include <smmintrin.h>

namespace terraces {

namespace efficient {

index block_index(index i) { return i / 64; }

index base_index(index block) { return block * 64; }

uint8_t shift_index(index i) { return i % 64; }

uint64_t set_mask(index i) { return 1ull << (i & 63); }

uint64_t clear_mask(index i) { return ~set_mask(i); }

uint64_t prefix_mask(index i) { return set_mask(i) - 1; }

uint8_t popcount(uint64_t block) { return (uint8_t)__builtin_popcountll(block); }

uint8_t partial_popcount(uint64_t block, index i) { return popcount(block & prefix_mask(i)); }

bitvector::bitvector(index size)
        : m_size{size}, m_blocks((size + 63) / 64), m_ranks((size + 63) / 64 + 1),
          m_ranks_dirty{false} {}

bool bitvector::get(index i) const {
	assert(i < m_size);
	return (m_blocks[block_index(i)] >> shift_index(i)) & 1;
}

void bitvector::clr(index i) {
	assert(i < m_size);
	m_blocks[block_index(i)] &= clear_mask(i);
	m_ranks_dirty = true;
}

void bitvector::set(index i) {
	assert(i < m_size);
	m_blocks[block_index(i)] |= set_mask(i);
	m_ranks_dirty = true;
}

void bitvector::update_ranks() {
	index sum = 0;
	for (index i = 0; i < m_blocks.size(); ++i) {
		sum += popcount(m_blocks[i]);
		m_ranks[i + 1] = sum;
	}
	m_ranks_dirty = false;
}

index bitvector::rank(index i) const {
	assert(!m_ranks_dirty);
	assert(i <= m_size);
	index b = block_index(i);
	return m_ranks[b] + partial_popcount(m_blocks[b], shift_index(i));
}

} // namespace efficient

namespace naive {
bitvector::bitvector(index size) : m_vector(size) {}

bool bitvector::get(index i) const {
	assert(i < m_vector.size());
	return m_vector[i];
}

void bitvector::set(index i) {
	assert(i < m_vector.size());
	m_vector[i] = true;
}

void bitvector::clr(index i) {
	assert(i < m_vector.size());
	m_vector[i] = false;
}

index bitvector::rank(index i) const {
	assert(i <= m_vector.size());
	return (index)std::count(m_vector.begin(),
	                         m_vector.begin() + (std::vector<bool>::difference_type)i, true);
}

} // namespace naive

} // namespace terraces
