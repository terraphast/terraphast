#include <catch.hpp>

#include <iostream>

#include <terraces/constraints.hpp>

namespace terraces {
namespace tests {

TEST_CASE("constraints1", "[constraints]") {
	std::vector<index> leaves = {0, 1, 2};
	constraints c = {constraint{7, 8, 9}};
	std::vector<std::vector<index>> res = {{0}, {1}, {2}};
	CHECK(apply_constraints(leaves.size(), filter_constraints(leaves, c)) == res);
}

TEST_CASE("constraints2", "[constraints]") {
	std::vector<index> leaves = {0, 1, 2};
	constraints c = {constraint{7, 8, 9}, constraint{0, 1, 2}, constraint{1, 3, 2}};
	std::vector<std::vector<index>> res = {{0, 1}, {2}};
	CHECK(apply_constraints(leaves.size(), filter_constraints(leaves, c)) == res);
}

TEST_CASE("constraints3", "[constraints]") {
	std::vector<index> leaves = {0, 1, 2, 3};
	constraints c = {constraint{7, 8, 9}, constraint{0, 1, 2}, constraint{1, 3, 2}};
	std::vector<std::vector<index>> res = {{0, 1, 3}, {2}};
	CHECK(apply_constraints(leaves.size(), filter_constraints(leaves, c)) == res);
}

TEST_CASE("constraints4", "[constraints]") {
	std::vector<index> leaves = {0, 1, 2, 3};
	constraints c = {constraint{0, 1, 2}, constraint{2, 3, 0}};
	std::vector<std::vector<index>> res = {{0, 1}, {2, 3}};
	CHECK(apply_constraints(leaves.size(), filter_constraints(leaves, c)) == res);
}

} // namespace tests
} // namespace terraces
