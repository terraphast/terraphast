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
	void clear();
	void finalize_edit();
};

class fast_index_set_iterator {
private:
	const fast_index_set* m_set;
	index m_index;

public:
	fast_index_set_iterator(const fast_index_set* set, index i);
	fast_index_set_iterator& operator++();
	bool operator==(const fast_index_set_iterator& other) const;
	bool operator!=(const fast_index_set_iterator& other) const;
	const index& operator*() const;
};

} // namespace terraces

#endif // FAST_SET_H
