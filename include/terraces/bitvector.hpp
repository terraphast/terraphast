#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <cstdint>
#include <terraces/intrinsics.hpp>
#include <terraces/trees.hpp>
#include <vector>

#include "stack_allocator.hpp"

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

namespace bits {

static_assert(std::numeric_limits<index>::radix == 2, "Our integers must be of base 2");
constexpr index word_bits = std::numeric_limits<index>::digits;

inline index block_index(index i) { return i / word_bits; }
inline index base_index(index block) { return block * word_bits; }
inline uint8_t shift_index(index i) { return i % word_bits; }
inline index set_mask(index i) { return 1ull << (i & word_bits - 1); }
inline index clear_mask(index i) { return ~set_mask(i); }
inline index prefix_mask(index i) { return set_mask(i) - 1; }
inline index next_bit(uint64_t block, index i) { return i + bitscan(block >> shift_index(i)); }
inline index next_bit0(index block, index i) { return i + bitscan(block); }
inline bool has_next_bit(index block, index i) { return (block >> shift_index(i)) != 0; }
inline bool has_next_bit0(index block) { return block != 0; }
inline index partial_popcount(index block, index i) { return popcount(block & prefix_mask(i)); }
} // namespace bits

class bitvector_iterator;

template <typename Allocator = std::allocator<index>>
class basic_bitvector {
public:
	using value_type = index;
	using iterator = bitvector_iterator;

private:
	index m_size;
	// TODO align blocks for SSE
	std::vector<value_type, Allocator> m_blocks;
	std::vector<value_type, Allocator> m_ranks;
	index m_count;

	debug_var(bool m_ranks_dirty);

	void add_sentinel();

public:
	/** Initializes a bitvector with given size. */
	basic_bitvector(index size, Allocator alloc = {});
	/** Sets a bit in the bitvector. */
	void set(index i) {
		assert(i < m_size);
		m_blocks[bits::block_index(i)] |= bits::set_mask(i);
		debug_code(m_ranks_dirty = true);
	}
	/** Clears a bit in the bitvector. */
	void clr(index i) {
		assert(i < m_size);
		m_blocks[bits::block_index(i)] &= bits::clear_mask(i);
		debug_code(m_ranks_dirty = true);
	}
	/** Flips a bit in the bitvector. */
	void flip(index i) {
		assert(i < m_size);
		m_blocks[bits::block_index(i)] ^= bits::set_mask(i);
		debug_code(m_ranks_dirty = true);
	}
	/** Returns a bit from the bitvector. */
	bool get(index i) const {
		assert(i < m_size);
		return ((m_blocks[bits::block_index(i)] >> bits::shift_index(i)) & 1) != 0u;
	}
	/** Returns the size of the bitvector. */
	index size() const { return m_size; }

	/** Clears all bits in the bitvector. */
	void blank();
	/** Inverts all bits in the bitvector. */
	void invert();
	/** Applies element-wise xor from another bitvector. */
	void bitwise_xor(const basic_bitvector& other);
	/** Sets the values of this bitvector to the bitwise or of two bitvectors. */
	void set_bitwise_or(const basic_bitvector& fst, const basic_bitvector& snd);

	/** Updates the internal data structures after editing the vector. */
	void update_ranks();
	/** Returns the rank of an index, i.e. the number of set bits in the range [0..i) */
	index rank(index i) const {
		assert(!m_ranks_dirty);
		assert(i <= m_size);
		index b = bits::block_index(i);
		return m_ranks[b] + bits::partial_popcount(m_blocks[b], bits::shift_index(i));
	}
	index select(index i) const;
	/** Returns the number of set bits. */
	index count() const {
		assert(!m_ranks_dirty);
		return m_count - 1;
	}
	/** Returns the index of the first set bit or size() if no bit is set. */
	index first_set() const {
		index b = 0;
		while (!bits::has_next_bit0(m_blocks[b])) {
			++b;
		}
		return bits::next_bit0(m_blocks[b], bits::base_index(b));
	}
	/** Returns the index of the next set bit after the index or size() if no bit is set. */
	index next_set(index i) const {
		++i;
		index b = bits::block_index(i);
		if (bits::has_next_bit(m_blocks[b], i)) {
			// the next bit is in the current block
			return bits::next_bit(m_blocks[b], i);
		}
		// the next bit is in a far-away block
		do {
			++b;
		} while (!bits::has_next_bit0(m_blocks[b]));
		return bits::next_bit0(m_blocks[b], bits::base_index(b));
	}
	/** Returns the index one past the last element. */
	index last_set() const { return m_size; }

	iterator begin() const;
	iterator end() const;

	bool operator<(const basic_bitvector& other) const;
	bool operator==(const basic_bitvector& other) const;
	bool operator!=(const basic_bitvector& other) const { return !(*this == other); }
};

using bitvector = basic_bitvector<>;

class bitvector_iterator {
public:
	using value_type = bitvector::value_type;

private:
	const bitvector& m_set;
	index m_index;

public:
	bitvector_iterator(const bitvector& set, index i) : m_set{set}, m_index{i} {}
	bitvector_iterator& operator++() {
		m_index = m_set.next_set(m_index);
		return *this;
	}
	bool operator==(const bitvector_iterator& other) const { return m_index == other.m_index; }
	bool operator!=(const bitvector_iterator& other) const { return !(*this == other); }
	const index& operator*() const { return m_index; }
};

template <typename Alloc>
inline auto basic_bitvector<Alloc>::begin() const -> iterator {
	return {*this, first_set()};
}

template <typename Alloc>
inline auto basic_bitvector<Alloc>::end() const -> iterator {
	return {*this, last_set()};
}

/** Returns a basic_bitvector<Alloc> containing size elements. */
template <typename Alloc>
inline basic_bitvector<Alloc> full_set(index size, Alloc a) {
	(void)a;
	basic_bitvector<Alloc> set{size, a};
	set.invert();
	set.update_ranks();
	return set;
}
inline bitvector full_set(index size) { return full_set(size, std::allocator<index>{}); }

template <typename Alloc>
inline index basic_bitvector<Alloc>::select(index i) const {
	assert(i < count());
	auto it = first_set();
	for (index j = 0; j < i; ++j) {
		++it;
	}
	return it;
}

template <typename Alloc>
basic_bitvector<Alloc>::basic_bitvector(index size, Alloc a)
        : m_size{size}, m_blocks(size / 64 + 1, a), m_ranks(m_blocks.size() + 1, a) {
	add_sentinel();
	debug_code(m_ranks_dirty = false);
}

template <typename Alloc>
void basic_bitvector<Alloc>::add_sentinel() {
	// add sentinel bit for iteration
	m_blocks[bits::block_index(m_size)] |= bits::set_mask(m_size);
}

template <typename Alloc>
void basic_bitvector<Alloc>::blank() {
	for (auto& el : m_blocks) {
		el = 0;
	}
	add_sentinel();
	debug_code(m_ranks_dirty = true);
}

template <typename Alloc>
void basic_bitvector<Alloc>::bitwise_xor(const basic_bitvector<Alloc>& other) {
	assert(size() == other.size());
	for (index b = 0; b < m_blocks.size(); ++b) {
		m_blocks[b] ^= other.m_blocks[b];
	}
	add_sentinel();
	debug_code(m_ranks_dirty = true);
}

template <typename Alloc>
void basic_bitvector<Alloc>::invert() {
	for (index b = 0; b < m_blocks.size() - 1; ++b) {
		m_blocks[b] = ~m_blocks[b];
	}
	m_blocks[m_blocks.size() - 1] ^= bits::prefix_mask(bits::shift_index(m_size));
	debug_code(m_ranks_dirty = true);
}

template <typename Alloc>
void basic_bitvector<Alloc>::set_bitwise_or(const basic_bitvector<Alloc>& fst,
                                            const basic_bitvector<Alloc>& snd) {
	assert(size() == fst.size() && size() == snd.size());
	for (index b = 0; b < m_blocks.size(); ++b) {
		m_blocks[b] = fst.m_blocks[b] | snd.m_blocks[b];
	}
	debug_code(m_ranks_dirty = true);
}

template <typename Alloc>
void basic_bitvector<Alloc>::update_ranks() {
	m_count = 0;
	for (index b = 0; b < m_blocks.size(); ++b) {
		m_count += bits::popcount(m_blocks[b]);
		m_ranks[b + 1] = m_count;
	}
	assert(m_count > 0);
	debug_code(m_ranks_dirty = false);
}

template <typename Alloc>
bool basic_bitvector<Alloc>::operator<(const basic_bitvector<Alloc>& other) const {
	assert(size() == other.size());
	return m_blocks < other.m_blocks;
}

template <typename Alloc>
bool basic_bitvector<Alloc>::operator==(const basic_bitvector<Alloc>& other) const {
	assert(size() == other.size());
	return m_blocks == other.m_blocks;
}

} // namespace terraces

#endif // BITVECTOR_H
