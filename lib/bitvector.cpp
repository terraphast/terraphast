#include <terraces/bitvector.hpp>

#include <algorithm>

namespace terraces {

namespace efficient {

index block_index(index i) { return i / 64; }

index base_index(index block) { return block * 64; }

uint8_t shift_index(index i) { return i % 64; }

uint64_t set_mask(index i) { return 1ull << (i & 63); }

uint64_t clear_mask(index i) { return ~set_mask(i); }

uint64_t prefix_mask(index i) { return set_mask(i) - 1; }

index next_bit(uint64_t block, index i) {
	return i + (index)__builtin_ctzll(block >> shift_index(i));
}

bool has_next_bit(uint64_t block, index i) { return (block >> shift_index(i)) != 0; }

uint8_t popcount(uint64_t block) { return (uint8_t)__builtin_popcountll(block); }

uint8_t partial_popcount(uint64_t block, index i) { return popcount(block & prefix_mask(i)); }

bitvector::bitvector(index size)
        : m_size{size}, m_blocks(size / 64 + 1), m_ranks(m_blocks.size() + 1),
          m_ranks_dirty{false} {
	// add sentinel bit for iteration
	m_blocks[block_index(size)] |= set_mask(size);
}

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

void bitvector::blank() {
	for (auto& el : m_blocks)
		el = 0;
	m_blocks[block_index(m_size)] |= set_mask(m_size);
	m_ranks_dirty = true;
}

void bitvector::update_ranks() {
	m_count = 0;
	for (index i = 0; i < m_blocks.size(); ++i) {
		m_count += popcount(m_blocks[i]);
		m_ranks[i + 1] = m_count;
	}
	assert(m_count > 0);
	m_ranks_dirty = false;
}

index bitvector::rank(index i) const {
	assert(!m_ranks_dirty);
	assert(i <= m_size);
	index b = block_index(i);
	return m_ranks[b] + partial_popcount(m_blocks[b], shift_index(i));
}

index bitvector::count() const {
	assert(!m_ranks_dirty);
	return m_count - 1;
}

index bitvector::size() const { return m_size; }

index bitvector::begin() const {
	index b = 0;
	while (!has_next_bit(m_blocks[b], 0)) {
		++b;
	}
	return next_bit(m_blocks[b], base_index(b));
}

index bitvector::next(index i) const {
	++i;
	index b = block_index(i);
	while (!has_next_bit(m_blocks[b], i)) {
		++b;
		i = base_index(b);
	}
	return next_bit(m_blocks[b], i);
}

index bitvector::end() const { return m_size; }

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

index bitvector::size() const { return m_vector.size(); }

index bitvector::rank(index i) const {
	assert(i <= m_vector.size());
	return (index)std::count(m_vector.begin(),
	                         m_vector.begin() + (std::vector<bool>::difference_type)i, true);
}

index bitvector::count() const { return rank(m_vector.size()); }

index bitvector::begin() const {
	return (index)std::distance(m_vector.begin(),
	                            std::find(m_vector.begin(), m_vector.end(), true));
}

index bitvector::next(index i) const {
	return (index)std::distance(m_vector.begin(),
	                            std::find(m_vector.begin() + (int)i + 1, m_vector.end(), true));
}

index bitvector::end() const { return m_vector.size(); }

} // namespace naive

} // namespace terraces
