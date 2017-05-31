#ifndef TERRACES_SUPERTREE_HPP
#define TERRACES_SUPERTREE_HPP

#include "constraints.hpp"
#include "trees.hpp"

namespace terraces {

/**
 * Map the given constraints according to the given vector of indices.
 * If the vector of indices is {2, 5, 6} for example, we get the mapping 2->0, 5->1, 6->2.
 */
constraints map_constraints(const std::vector<index>&, constraints);

/**
 * Contrary function to map_constraints.
 */
std::vector<std::vector<index>> map_sets(std::vector<index>, std::vector<std::vector<index>>);

/**
 * Check if on a terrace given a number and a vector of constraints. Entry point for terrace
 * checking.
 */
bool check_supertree(const tree&, const constraints&);

/**
 * Check if on a terrace given a number and a vector of constraints.
 */
bool check_supertree(const std::vector<index>&, const constraints&);

/**
 * Count trees given a number and a vector of constraints. Entry point for tree counting.
 */
size_t count_supertree(const tree&, const constraints&);

/**
 * Count trees.
 */
size_t count_supertree(const std::vector<index>&, const constraints&);

} // namespace terraces

#endif
