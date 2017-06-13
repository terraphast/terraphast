#ifndef FAST_SET_H
#define FAST_SET_H

#include <terraces/bitvector.hpp>

namespace terraces {

class fast_index_set_iterator;

class fast_index_set {
	using iterator = fast_index_set_iterator;
	friend class fast_index_set_iterator;

private:
	efficient::bitvector m_vector;

public:
	fast_index_set(index max_size);

	bool contains(index i) const;
	index rank(index i) const;
	index max_size() const;
	index size() const;

	iterator begin() const;
	iterator end() const;

	void insert(index i);
	void remove(index i);
	void toggle(index i);
	void symm_difference(const fast_index_set& other);
	void clear();
	void finalize_edit();
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
