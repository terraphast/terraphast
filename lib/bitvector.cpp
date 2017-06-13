#include <terraces/bitvector.hpp>

#include <algorithm>

namespace terraces {

namespace efficient {

bitvector::bitvector(index size)
        : m_size{size}, m_blocks(size / 64 + 1), m_ranks(m_blocks.size() + 1) {
	add_sentinel();
	debug_code(m_ranks_dirty = false);
}

void bitvector::add_sentinel() {
	// add sentinel bit for iteration
	m_blocks[block_index(m_size)] |= set_mask(m_size);
}

void bitvector::blank() {
	for (auto& el : m_blocks) {
		el = 0;
	}
	add_sentinel();
	debug_code(m_ranks_dirty = true);
}

void bitvector::bitwise_xor(const bitvector& other) {
	assert(size() == other.size());
	for (index b = 0; b < m_blocks.size(); ++b) {
		m_blocks[b] ^= other.m_blocks[b];
	}
	add_sentinel();
	debug_code(m_ranks_dirty = true);
}

void bitvector::update_ranks() {
	m_count = 0;
	for (index b = 0; b < m_blocks.size(); ++b) {
		m_count += popcount(m_blocks[b]);
		m_ranks[b + 1] = m_count;
	}
	assert(m_count > 0);
	debug_code(m_ranks_dirty = false);
}

} // namespace efficient

} // namespace terraces
