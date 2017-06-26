#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <cstdint>
#include <terraces/bits.hpp>
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

namespace bits {
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

class bitvector {
public:
	using value_type = index;
	using iterator = bitvector_iterator;

private:
	index m_size;
	// TODO align blocks for SSE
	std::vector<value_type> m_blocks;
	std::vector<value_type> m_ranks;
	index m_count;

	debug_var(bool m_ranks_dirty);

	void add_sentinel();

public:
	/** Initializes a bitvector with given size. */
	bitvector(index size);
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
	void bitwise_xor(const bitvector& other);
	/** Sets the values of this bitvector to the bitwise or of two bitvectors. */
	void set_bitwise_or(const bitvector& fst, const bitvector& snd);

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

	bool operator<(const bitvector& other) const;
	bool operator==(const bitvector& other) const;
	bool operator!=(const bitvector& other) const { return !(*this == other); }
};

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

inline auto bitvector::begin() const -> iterator { return {*this, first_set()}; }

inline auto bitvector::end() const -> iterator { return {*this, last_set()}; }

/** Returns a bitvector containing size elements. */
inline bitvector full_set(index size) {
	bitvector set{size};
	set.invert();
	set.update_ranks();
	return set;
}

inline index bitvector::select(index i) const {
	assert(i < count());
	auto it = first_set();
	for (index j = 0; j < i; ++j) {
		++it;
	}
	return it;
}

} // namespace terraces

#endif // BITVECTOR_H
