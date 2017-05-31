#include <catch.hpp>

#include <iostream>

#include <terraces/supertree.hpp>

namespace terraces {
namespace tests {

TEST_CASE("mapping", "[mapping]") {
	std::vector<index> leaves = {0, 2, 3, 5, 6, 7, 8};
	constraints c = {{0, 2, 3}, {6, 7, 8}, {2, 5, 8}};
	constraints t = {{0, 1, 2}, {4, 5, 6}, {1, 3, 6}};
	CHECK(map_constraints(leaves, c) == t);
}

TEST_CASE("check_supertree1", "[supertree]") {
	constraints c = {};
	CHECK(check_supertree(2, c) == false);
}

TEST_CASE("check_supertree2", "[supertree]") {
	constraints c = {};
	CHECK(check_supertree(3, c) == true);
}

TEST_CASE("check_supertree3", "[supertree]") {
	constraints c = {};
	CHECK(check_supertree(7, c) == true);
}

TEST_CASE("check_supertree4", "[supertree]") {
	constraints c = {{0, 1, 2}};
	CHECK(check_supertree(3, c) == false);
}

TEST_CASE("check_supertree5", "[supertree]") {
	constraints c = {{0, 1, 2}, {2, 3, 4}};
	CHECK(check_supertree(4, c) == false);
}

TEST_CASE("check_supertree6", "[supertree]") {
	constraints c = {{1, 0, 2}, {3, 4, 1}};
	CHECK(check_supertree(5, c) == true);
}

TEST_CASE("count_supertree1", "[supertree]") {
	constraints c = {};
	CHECK(count_supertree(2, c) == 1);
}

TEST_CASE("count_supertree2", "[supertree]") {
	constraints c = {};
	CHECK(count_supertree(3, c) == 3);
}

TEST_CASE("count_supertree3", "[supertree]") {
	constraints c = {};
	CHECK(count_supertree(7, c) == 10395);
}

TEST_CASE("count_supertree4", "[supertree]") {
	constraints c = {{0, 1, 2}};
	CHECK(count_supertree(3, c) == 1);
}

TEST_CASE("count_supertree5", "[supertree]") {
	constraints c = {{0, 1, 2}, {2, 3, 4}};
	CHECK(count_supertree(4, c) == 1);
}

TEST_CASE("count_supertree6", "[supertree]") {
	constraints c = {{1, 0, 2}, {3, 4, 1}};
	CHECK(count_supertree(5, c) == 9);
}

TEST_CASE("count_supertree7", "[supertree]") {
	constraints c = {{0, 1, 3}, {3, 2, 0}, {4, 5, 6}, {6, 3, 4}, {2, 3, 6}, {2, 6, 7}};
	CHECK(count_supertree(8, c) == 173);
}

TEST_CASE("count_supertree_none", "[supertree]") {
	constraints c = {{0, 1, 2}, {2, 1, 0}};
	CHECK(count_supertree(3, c) == 0);
}

} // namespace tests
} // namespace terraces
