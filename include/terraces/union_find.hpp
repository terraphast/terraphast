#ifndef TERRACES_UNION_FIND_HPP
#define TERRACES_UNION_FIND_HPP

#include <algorithm>

#include "trees.hpp"

namespace terraces {

class union_find {
public:
	using value_type = index;

private:
	std::vector<index> m_parent;
	bool m_compressed;

public:
	union_find(index);
	index find(index);
	index simple_find(index) const;
	index size() const;
	void compress();
	void merge(index, index);
	bool is_representative(index) const;
};

} // namespace terraces

#endif
