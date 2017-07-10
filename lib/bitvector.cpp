#include <terraces/bitvector.hpp>

#include <algorithm>

namespace terraces {

bitvector::bitvector(index size) : m_size{size}, m_blocks(size / 64 + 1) { add_sentinel(); }

ranked_bitvector::ranked_bitvector(index size) : bitvector{size}, m_ranks(m_blocks.size() + 1) {
	add_sentinel();
#ifndef NDEBUG
	m_ranks_dirty = true;
#endif
}

void bitvector::add_sentinel() {
	// add sentinel bit for iteration
	m_blocks[bits::block_index(m_size)] |= bits::set_mask(m_size);
}

bool bitvector::empty() const {
	for (index b = 0; b < m_blocks.size() - 1; ++b) {
		if (m_blocks[b]) {
			return false;
		}
	}
	return !(m_blocks[m_blocks.size() - 1] & bits::prefix_mask(bits::shift_index(m_size)));
}

void bitvector::blank() {
	for (auto& el : m_blocks) {
		el = 0;
	}
	add_sentinel();
}

void bitvector::bitwise_xor(const bitvector& other) {
	assert(size() == other.size());
	for (index b = 0; b < m_blocks.size(); ++b) {
		m_blocks[b] ^= other.m_blocks[b];
	}
	add_sentinel();
}

void bitvector::invert() {
	for (index b = 0; b < m_blocks.size() - 1; ++b) {
		m_blocks[b] = ~m_blocks[b];
	}
	m_blocks[m_blocks.size() - 1] ^= bits::prefix_mask(bits::shift_index(m_size));
}

void bitvector::set_bitwise_or(const bitvector& fst, const bitvector& snd) {
	assert(size() == fst.size() && size() == snd.size());
	for (index b = 0; b < m_blocks.size(); ++b) {
		m_blocks[b] = fst.m_blocks[b] | snd.m_blocks[b];
	}
}

void ranked_bitvector::blank() {
	bitvector::blank();
#ifndef NDEBUG
	m_ranks_dirty = true;
#endif
}

void ranked_bitvector::bitwise_xor(const bitvector& other) {
	bitvector::bitwise_xor(other);
#ifndef NDEBUG
	m_ranks_dirty = true;
#endif
}

void ranked_bitvector::invert() {
	bitvector::invert();
#ifndef NDEBUG
	m_ranks_dirty = true;
#endif
}

void ranked_bitvector::set_bitwise_or(const bitvector& fst, const bitvector& snd) {
	bitvector::set_bitwise_or(fst, snd);
#ifndef NDEBUG
	m_ranks_dirty = true;
#endif
}

void ranked_bitvector::update_ranks() {
	m_count = 0;
	for (index b = 0; b < m_blocks.size(); ++b) {
		m_count += bits::popcount(m_blocks[b]);
		m_ranks[b + 1] = m_count;
	}
	assert(m_count > 0);
#ifndef NDEBUG
	m_ranks_dirty = false;
#endif
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

} // namespace terraces
