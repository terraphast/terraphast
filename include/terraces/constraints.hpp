#ifndef TERRACES_CONSTRAINTS_HPP
#define TERRACES_CONSTRAINTS_HPP

#include <iosfwd>
#include <tuple>
#include <vector>

#include "trees.hpp"

namespace terraces {

struct constraint {
	index shared;
	index left;
	index right;
};

bool operator==(const constraint& c1, const constraint& c2);

std::ostream& operator<<(std::ostream& s, const constraint& c);

using constraints = std::vector<constraint>;

/**
 * Filter out all constraints which do not make sense of the given vector of indices.
 */
constraints filter_constraints(std::vector<index>, constraints);

/**
 * Apply all given contraints to the given vector of indices. This is done via a union-find
 * structure.
 */
std::vector<std::vector<index>> apply_constraints(index, constraints);

} // namespace terraces

#endif
