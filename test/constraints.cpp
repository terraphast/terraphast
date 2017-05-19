#include <catch.hpp>

#include <iostream>

#include <terraces/constraints.hpp>

namespace terraces {
namespace tests {

TEST_CASE("constraints", "[constraints]") {
	std::vector<index> leaves1 = {0, 1, 2};
	std::vector<index> leaves2 = {0, 1, 2, 3};
	constraints c = {constraint{7, 8, 9}, constraint{0, 1, 2}, constraint{1, 3, 2}};

	std::vector<std::vector<index>> target_leaves1 = {{0, 1}, {2}};
	std::vector<std::vector<index>> target_leaves2 = {{0, 1, 3}, {2}};

	CHECK(apply_constraints(leaves1, filter_constraints(leaves1, c)) == target_leaves1);
	CHECK(apply_constraints(leaves2, filter_constraints(leaves2, c)) == target_leaves2);
}

} // namespace tests
} // namespace terraces
