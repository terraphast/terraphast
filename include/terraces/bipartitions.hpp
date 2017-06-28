#ifndef TERRACES_BIPARTITIONS_HPP
#define TERRACES_BIPARTITIONS_HPP

#include <cassert>
#include <cmath>
#include <ostream>

#include "bitvector.hpp"
#include "trees.hpp"
#include "union_find.hpp"

namespace terraces {

template <typename Alloc>
class basic_bipartition_iterator {
public:
	using bitvector = basic_bitvector<Alloc>;

private:
	Alloc m_alloc;
	const bitvector& m_leaves;
	const union_find& m_sets;
	const bitvector m_set_rep;
	bitvector m_subleaves;

	index m_bip;
	index m_end;

	bool in_left_partition(index i) const;
	bitvector find_set_reps() const;

public:
	basic_bipartition_iterator(const bitvector& leaves, const union_find& sets, Alloc);
	/** Moves to the next bipartition. */
	void increase();
	/** Returns true if this bipartition is valid. */
	bool is_valid() const;
	/** Returns the current partition. Switching to the other partition can be done via flip().
	 */
	const bitvector& get_current_set() const;
	/** Switches the current partition with the other partition. */
	void flip_sets();

	index cur_bip() const { return m_bip; }
	index end_bip() const { return m_end; }
	const union_find& sets() const { return m_sets; }
	const bitvector& leaves() const { return m_leaves; }
};

template <typename Alloc>
std::ostream& operator<<(std::ostream& stream, const basic_bipartition_iterator<Alloc>& it);

// using bipartition_iterator = basic_bipartition_iterator<std::allocator<index>>;
using bipartition_iterator = basic_bipartition_iterator<utils::stack_allocator<index>>;

template <typename Alloc>
basic_bipartition_iterator<Alloc>::basic_bipartition_iterator(const bitvector& leaves,
                                                              const union_find& sets, Alloc a)
        : m_alloc{a}, m_leaves{leaves}, m_sets{sets}, m_set_rep{find_set_reps()},
          m_subleaves{leaves.size(), a}, m_bip{0}, m_end{(1ull << (m_set_rep.count() - 1))} {
	assert(m_set_rep.count() < 64);
	increase();
}

template <typename Alloc>
bool basic_bipartition_iterator<Alloc>::in_left_partition(index i) const {
	return (m_bip & (1ull << (i - 1))) != 0;
}

template <typename Alloc>
basic_bitvector<Alloc> basic_bipartition_iterator<Alloc>::find_set_reps() const {
	// TODO ugly style :)
	bitvector set_rep(m_leaves.count(), m_alloc);
	for (index i = 0; i < m_leaves.count(); ++i) {
		set_rep.set(m_sets.find(i));
	}
	set_rep.update_ranks();
	return set_rep;
}

template <typename Alloc>
bool basic_bipartition_iterator<Alloc>::is_valid() const {
	return m_bip < m_end;
}

template <typename Alloc>
void basic_bipartition_iterator<Alloc>::increase() {
	m_bip++;
	// TODO can we do better?
	if (is_valid()) {
		m_subleaves.blank();
		index ii = 0;
		for (auto i = m_leaves.first_set(); i < m_leaves.last_set();
		     i = m_leaves.next_set(i)) {
			if (in_left_partition(m_set_rep.rank(m_sets.find(ii)))) {
				m_subleaves.set(i);
			}
			++ii;
		}
		m_subleaves.update_ranks();
	}
}

template <typename Alloc>
const basic_bitvector<Alloc>& basic_bipartition_iterator<Alloc>::get_current_set() const {
	return m_subleaves;
}

template <typename Alloc>
void basic_bipartition_iterator<Alloc>::flip_sets() {
	m_subleaves.bitwise_xor(m_leaves);
	m_subleaves.update_ranks();
}

template <typename Alloc>
std::ostream& operator<<(std::ostream& stream, const basic_bipartition_iterator<Alloc>& it) {
	for (index b = it.end_bip(), c = it.cur_bip(); b != 0u; b >>= 1, c >>= 1) {
		stream << (index)(c & 1);
	}
	stream << '/';
	for (index b = it.end_bip(); b != 0u; b >>= 1) {
		stream << (index)(b & 1);
	}
	return stream;
}

} // namespace terraces

#endif
