#ifndef FAST_SET_H
#define FAST_SET_H

#include <set>
#include <vector>

#include "trees.hpp"

namespace terraces {

class fast_index_set
{
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

}

#endif // FAST_SET_H
