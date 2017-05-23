#ifndef TERRACES_CONSTRAINTS_HPP
#define TERRACES_CONSTRAINTS_HPP

#include <vector>

#include "trees.hpp"

namespace terraces {

struct constraint {
	index left;
	index shared;
	index right;

	constraint(index left, index shared, index right)
	        : left{left}, shared{shared}, right{right} {}
};

using constraints = std::vector<constraint>;

constraints compute_constraints(const std::vector<tree>& trees);

} // namespace terraces

#endif
