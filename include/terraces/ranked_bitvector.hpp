#ifndef RANKED_BITVECTOR_HPP
#define RANKED_BITVECTOR_HPP

#include <terraces/bitvector.hpp>

namespace terraces {

template <typename Alloc>
class basic_ranked_bitvector : public basic_bitvector<Alloc> {
public:
	using value_type = typename basic_bitvector<Alloc>::value_type;

private:
	std::vector<value_type, Alloc> m_ranks;
	index m_count;
#ifndef NDEBUG
	bool m_ranks_dirty;
#endif

public:
	basic_ranked_bitvector(index size, Alloc alloc)
	        : basic_bitvector<Alloc>{size, alloc},
	          m_ranks(basic_bitvector<Alloc>::m_blocks.size() + 1, alloc) {
		basic_bitvector<Alloc>::add_sentinel();
#ifndef NDEBUG
		m_ranks_dirty = true;
#endif
	}

	/** Sets a bit in the bitvector. */
	void set(index i) {
		basic_bitvector<Alloc>::set(i);
#ifndef NDEBUG
		m_ranks_dirty = true;
#endif
	}
	/** Clears a bit in the bitvector. */
	void clr(index i) {
		basic_bitvector<Alloc>::clr(i);
#ifndef NDEBUG
		m_ranks_dirty = true;
#endif
	}
	/** Flips a bit in the bitvector. */
	void flip(index i) {
		basic_bitvector<Alloc>::flip(i);
#ifndef NDEBUG
		m_ranks_dirty = true;
#endif
	}

	/** Clears all bits in the bitvector. */
	void blank();
	/** Inverts all bits in the bitvector. */
	void invert();
	/** Applies element-wise xor from another bitvector. */
	void bitwise_xor(const basic_bitvector<Alloc>& other);
	/** Sets the values of this bitvector to the bitwise or of two bitvectors. */
	void set_bitwise_or(const basic_bitvector<Alloc>& fst, const basic_bitvector<Alloc>& snd);

	/** Returns the number of set bits. */
	index count() const {
		assert(!m_ranks_dirty);
		return m_count - 1;
	}

	/** Updates the internal data structures after editing the vector. */
	void update_ranks();
	/** Returns the rank of an index, i.e. the number of set bits in the range [0..i) */
	index rank(index i) const {
		assert(!m_ranks_dirty);
		assert(i <= basic_bitvector<Alloc>::m_size);
		index b = bits::block_index(i);
		return m_ranks[b] + bits::partial_popcount(basic_bitvector<Alloc>::m_blocks[b],
		                                           bits::shift_index(i));
	}
	index select(index i) const;
};

using ranked_bitvector = basic_ranked_bitvector<utils::stack_allocator<index>>;

/** Returns a basic_ranked_bitvector<Alloc> containing size elements. */
template <typename Alloc>
inline basic_ranked_bitvector<Alloc> full_ranked_set(index size, Alloc a) {
	basic_ranked_bitvector<Alloc> set{size, a};
	set.invert();
	set.update_ranks();
	return set;
}

template <typename Alloc>
inline index basic_ranked_bitvector<Alloc>::select(index i) const {
	assert(i < count());
	auto it = basic_bitvector<Alloc>::first_set();
	for (index j = 0; j < i; ++j) {
		++it;
	}
	return it;
}
template <typename Alloc>
void basic_ranked_bitvector<Alloc>::blank() {
	basic_bitvector<Alloc>::blank();
#ifndef NDEBUG
	m_ranks_dirty = true;
#endif
}

template <typename Alloc>
void basic_ranked_bitvector<Alloc>::bitwise_xor(const basic_bitvector<Alloc>& other) {
	basic_bitvector<Alloc>::bitwise_xor(other);
#ifndef NDEBUG
	m_ranks_dirty = true;
#endif
}

template <typename Alloc>
void basic_ranked_bitvector<Alloc>::invert() {
	basic_bitvector<Alloc>::invert();
#ifndef NDEBUG
	m_ranks_dirty = true;
#endif
}

template <typename Alloc>
void basic_ranked_bitvector<Alloc>::set_bitwise_or(const basic_bitvector<Alloc>& fst,
                                                   const basic_bitvector<Alloc>& snd) {
	basic_bitvector<Alloc>::set_bitwise_or(fst, snd);
#ifndef NDEBUG
	m_ranks_dirty = true;
#endif
}

template <typename Alloc>
void basic_ranked_bitvector<Alloc>::update_ranks() {
	m_count = 0;
	for (index b = 0; b < basic_bitvector<Alloc>::m_blocks.size(); ++b) {
		m_count += bits::popcount(basic_bitvector<Alloc>::m_blocks[b]);
		m_ranks[b + 1] = m_count;
	}
	assert(m_count > 0);
#ifndef NDEBUG
	m_ranks_dirty = false;
#endif
}

} // namespace terraces

#endif // RANKED_BITVECTOR_HPP
