#ifndef TERRACES_CONSTRAINTS_HPP
#define TERRACES_CONSTRAINTS_HPP

#include <iosfwd>
#include <tuple>
#include <vector>

#include "io_utils.hpp"
#include "trees.hpp"

namespace terraces {

/**
 * Represents a LCA constraint on leaves of a tree.
 * It is of the form \code
 * lca(left, shared) < lca(shared, right)
 * where the LCA's are compared by their height in the tree.
 */
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

using constraints = std::vector<constraint>;

std::ostream& operator<<(std::ostream& s, const constraint& c);
std::ostream& operator<<(std::ostream& stream, utils::named_output<constraints, name_map>);

// TODO move to separate header
constraints compute_constraints(const std::vector<tree>& trees);

index deduplicate_constraints(constraints& in_c);

} // namespace terraces

#endif
