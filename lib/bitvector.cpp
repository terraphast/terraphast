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

void bitvector::invert() {
	for (index b = 0; b < m_blocks.size() - 1; ++b) {
		m_blocks[b] = ~m_blocks[b];
	}
	m_blocks[m_blocks.size() - 1] ^= prefix_mask(shift_index(m_size));
	debug_code(m_ranks_dirty = true);
}

void bitvector::set_bitwise_or(const bitvector& fst, const bitvector& snd) {
	assert(size() == fst.size() && size() == snd.size());
	for (index b = 0; b < m_blocks.size(); ++b) {
		m_blocks[b] = fst.m_blocks[b] | snd.m_blocks[b];
	}
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

bool bitvector::operator<(const bitvector& other) const {
	assert(size() == other.size());
	return std::lexicographical_compare(m_blocks.begin(), m_blocks.end(),
	                                    other.m_blocks.begin(), other.m_blocks.end());
}

bool bitvector::operator==(const bitvector& other) const {
	assert(size() == other.size());
	return std::equal(m_blocks.begin(), m_blocks.end(), other.m_blocks.begin(),
	                  other.m_blocks.end());
}

} // namespace efficient

} // namespace terraces
