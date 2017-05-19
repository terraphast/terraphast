#ifndef TERRACES_UNION_FIND_HPP
#define TERRACES_UNION_FIND_HPP

#include <algorithm>

#include "trees.hpp"

namespace terraces {

struct union_find {
	std::vector<index> id;
	std::vector<index> size;
};

struct is_empty {
	bool operator()(const std::vector<std::size_t>& v) { return v.size() == 0; }
};

union_find make_set(std::size_t);

index find(union_find&, index);

void merge(union_find&, index, index);

std::vector<std::vector<index>> to_set_of_sets(union_find&);

} // namespace terraces

#endif
