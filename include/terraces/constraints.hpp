#ifndef TERRACES_CONSTRAINTS_HPP
#define TERRACES_CONSTRAINTS_HPP

#include <vector>

#include "trees.hpp"

namespace terraces {

struct constraint {
	index shared;
	index left;
	index right;
};

using constraints = std::vector<constraint>;

constraints filter_constraints(std::vector<index>, constraints);

std::vector<std::vector<index>> apply_constraints(std::vector<index>, constraints);

} // namespace terraces

#endif
