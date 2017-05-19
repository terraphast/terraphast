#include <catch.hpp>

#include <iostream>

#include <terraces/constraints.hpp>

namespace terraces {
namespace tests {

TEST_CASE("constraints", "[constraints]") {
        std::vector<index> leaves = { 0, 1, 2 };
        constraints c = { constraint {0, 1, 2} };
        std::vector<std::vector<index> > res = { {0, 1}, {2} };
	CHECK(apply_constraints(leaves, c) == res);
}

} // namespace tests
} // namespace terraces
