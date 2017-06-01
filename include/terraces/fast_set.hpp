#ifndef FAST_SET_H
#define FAST_SET_H

#include <set>
#include <vector>

#include "trees.hpp"

namespace terraces {

class bitvector {
private:
	std::vector<bool> m_vector;

public:
	bitvector(index size);
	bool get(index i) const;
	void set(index i, bool b);
	/** Returns the number of set bits in the range [0, i). */
	index rank(index i) const;
	/** Returns the smallest value j such that rank(j + 1) = i */
	index select(index i) const;
	/** Returns the index of the first set bit in the range (i, size). */
	index next_set_bit(index i) const;
};

class fast_index_set {
	using const_iterator = std::set<index>::const_iterator;

private:
	bool m_dirty;
	std::set<index> m_set;
	std::vector<bool> m_occ;

public:
	fast_index_set(index m_size);

	bool contains(index i) const;
	index size() const;

	const_iterator begin() const;
	const_iterator find(index i) const;
	const_iterator end() const;

	void fill(bool val);
	void reset_and_resize(index size);
	void insert_element(index i);
	void delete_element(index i);
	void finalize_edit();
};
} // namespace terraces

#endif // FAST_SET_H
