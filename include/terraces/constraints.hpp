#ifndef TERRACES_CONSTRAINTS_HPP
#define TERRACES_CONSTRAINTS_HPP

#include <iosfwd>
#include <tuple>
#include <vector>

#include "trees.hpp"

namespace terraces {

struct constraint {
	index left;
	index shared;
	index right;

	constraint(index left, index shared, index right)
	        : left{left}, shared{shared}, right{right} {}

	bool operator==(const constraint& o) const {
		return std::tie(left, shared, right) == std::tie(o.left, o.shared, o.right);
	}

	bool operator!=(const constraint& o) const { return !(o == *this); }
};

std::ostream& operator<<(std::ostream& s, const constraint& c);

using constraints = std::vector<constraint>;

std::ostream& operator<<(std::ostream& s, const constraint& c);

using constraints = std::vector<constraint>;

/**
 * Filter out all constraints which do not make sense of the given vector of indices.
 */
constraints filter_constraints(const std::vector<index>&, const constraints&);

/**
 * Apply all given contraints to the given vector of indices. This is done via a union-find
 * structure.
 */
std::vector<std::vector<index>> apply_constraints(index, const constraints&);

constraints compute_constraints(const std::vector<tree>& trees);

} // namespace terraces

#endif
