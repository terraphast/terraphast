#ifndef FAST_SET_H
#define FAST_SET_H

#include <cassert>
#include <terraces/bitvector.hpp>

namespace terraces {

class fast_index_set_iterator;

class fast_index_set {
	using iterator = fast_index_set_iterator;
	friend class fast_index_set_iterator;

private:
	efficient::bitvector m_vector;

public:
	fast_index_set(index max_size) : m_vector{max_size} {}

	inline bool contains(index i) const { return m_vector.get(i); }
	inline index rank(index i) const {
		assert(contains(i));
		return m_vector.rank(i);
	}

	inline index max_size() const { return m_vector.size(); }
	inline index size() const { return m_vector.count(); }

	iterator begin() const;
	iterator end() const;

	inline void insert(index i) { m_vector.set(i); }
	inline void remove(index i) { m_vector.clr(i); }
	inline void toggle(index i) { m_vector.flip(i); }
	inline void symm_difference(const fast_index_set& other) {
		m_vector.bitwise_xor(other.m_vector);
	}
	inline void clear() { m_vector.blank(); }
	inline void finalize_edit() { m_vector.update_ranks(); }
};

class fast_index_set_iterator {
private:
	const fast_index_set& m_set;
	index m_index;

public:
	fast_index_set_iterator(const fast_index_set& set, index i) : m_set{set}, m_index{i} {}
	inline fast_index_set_iterator& operator++() {
		m_index = m_set.m_vector.next(m_index);
		return *this;
	}
	inline bool operator==(const fast_index_set_iterator& other) const {
		return m_index == other.m_index;
	}
	inline bool operator!=(const fast_index_set_iterator& other) const {
		return !(*this == other);
	}
	inline const index& operator*() const { return m_index; }
};

inline auto fast_index_set::begin() const -> iterator { return {*this, m_vector.begin()}; }

inline auto fast_index_set::end() const -> iterator { return {*this, m_vector.end()}; }

} // namespace terraces

#endif // FAST_SET_H
