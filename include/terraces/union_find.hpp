#ifndef TERRACES_UNION_FIND_HPP
#define TERRACES_UNION_FIND_HPP

#include <algorithm>

#include "trees.hpp"

namespace terraces {

struct union_find {
	mutable std::vector<index> id;
	std::vector<index> size;
};

struct is_empty {
	bool operator()(const std::vector<std::size_t>& v) { return v.size() == 0; }
};

/**
 * Generate a union-find structure with the given size
 */
union_find make_set(std::size_t);

/**
 * Return a representational index which is in the same subset as the given one
 */
index find(const union_find&, index);

/**
 * Merge the given indices
 */
void merge(union_find&, index, index);

/**
 * Convert the given union-find structure to a set of sets of indices
 */
std::vector<std::vector<index>> to_set_of_sets(union_find&);

} // namespace terraces

#endif
