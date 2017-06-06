#include <terraces/bitvector.hpp>

#include <algorithm>

#include <smmintrin.h>

namespace terraces {

namespace efficient {

struct byte_select_table {
	uint8_t table[8][256];

	constexpr byte_select_table();
	constexpr uint8_t byte_select(uint8_t byte, uint8_t i);
};

constexpr byte_select_table::byte_select_table() : table() {
	for (uint8_t i = 0; i < 8; ++i) {
		for (uint16_t j = 0; j < 256; ++j) {
			table[i][j] = byte_select((uint8_t)j, i);
		}
	}
}

constexpr uint8_t byte_select_table::byte_select(uint8_t byte, uint8_t i) {
	uint8_t count = 0;
	uint8_t k = 0;
	for (; k < 8; ++k) {
		count += byte & 1;
		byte >>= 1;
		if (count > i)
			break;
	}
	return k == 8 ? 7 : k;
}

constexpr byte_select_table select_table = byte_select_table();

index block_index(index i) { return i / 64; }

index base_index(index block) { return block * 64; }

uint8_t shift_index(index i) { return i % 64; }

uint64_t set_mask(index i) { return 1ull << (i & 63); }

uint64_t clear_mask(index i) { return ~set_mask(i); }

uint64_t prefix_mask(index i) { return set_mask(i) - 1; }

uint8_t popcount(uint64_t block) { return (uint8_t)__builtin_popcountll(block); }

uint8_t partial_popcount(uint64_t block, index i) { return popcount(block & prefix_mask(i)); }

uint8_t byte_select(uint8_t byte, uint8_t i) {
	assert(i < 8);
	return select_table.table[i][byte];
}

uint8_t block_select(uint64_t block, uint8_t i) {
	// store lower and upper nipples
	__m128i lower = _mm_cvtsi64_si128(block & 0x0F0F0F0F0F0F0F0Full);
	__m128i upper = _mm_cvtsi64_si128((block & 0xF0F0F0F0F0F0F0F0ull) >> 4);
	// lookup popcounts for each nibble
	__m128i lookup = _mm_set_epi8(4, 3, 3, 2, 3, 2, 2, 1, 3, 2, 2, 1, 2, 1, 1, 0);
	lower = _mm_shuffle_epi8(lookup, lower);
	upper = _mm_shuffle_epi8(lookup, upper);
	// compute prefix popcounts
	__m128i byte_counts = _mm_cvtsi64_si128((int64_t)(
	        0x0101010101010101ull * (uint64_t)_mm_cvtsi128_si64(_mm_add_epi8(lower, upper))));
	// populate comparison values
	__m128i compare_counts = _mm_cvtsi64_si128(0x0101010101010101ull * (i + 1));
	// simultaneous comparison
	uint8_t skip =
	        popcount((uint64_t)_mm_cvtsi128_si64(_mm_cmpgt_epi8(compare_counts, byte_counts)));
	// substract popcount of the partial byte
	i -= ((_mm_cvtsi128_si64(byte_counts) << 8) >> skip) & 0xFF;
	return skip + byte_select((block >> skip) & 0xFF, i);
}

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

index bitvector::select(index j) const {
	assert(!m_ranks_dirty);
	assert(j < rank(m_size));
	index b = 0; // TODO this does not work yet for multiple blocks
	return base_index(b) + block_select(m_blocks[b], (uint8_t)j);
}

index bitvector::next(index i) const {
	assert(!m_ranks_dirty);
	assert(i < m_size);
	return select(rank(i + 1));
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

index bitvector::select(index i) const {
	index n = m_vector.size();
	assert(i < rank(n));
	++i;
	index rank = 0;
	index j;
	for (j = 0; j < n; ++j) {
		rank += get(j);
		if (rank == i) {
			break;
		}
	}
	return j;
}

index bitvector::next(index i) const {
	index j;
	for (j = i + 1; j < m_vector.size(); ++j) {
		if (m_vector[j]) {
			break;
		}
	}
	return j;
}
} // namespace naive

} // namespace terraces
