#ifndef TERRACES_UNION_FIND_HPP
#define TERRACES_UNION_FIND_HPP

#include <algorithm>

#include "trees.hpp"

namespace terraces {

class union_find {
private:
	mutable std::vector<index> id;
	std::vector<index> size;

public:
	union_find(std::size_t);
	index find(index) const;
	void merge(index, index);
};

} // namespace terraces

#endif
