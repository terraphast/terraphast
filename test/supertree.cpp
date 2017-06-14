#include <catch.hpp>

#include <iostream>

#include <terraces/supertree.hpp>

namespace terraces {
namespace tests {

TEST_CASE("count_supertree1", "[supertree]") {
	constraints c = {};
	terraces::tree_master tm;
	CHECK(tm.count_supertree(2, c).count == 1);
}

TEST_CASE("count_supertree2", "[supertree]") {
	constraints c = {};
	terraces::tree_master tm;
	CHECK(tm.count_supertree(3, c).count == 3);
}

TEST_CASE("count_supertree3", "[supertree]") {
	constraints c = {};
	terraces::tree_master tm;
	CHECK(tm.count_supertree(7, c).count == 10395);
}

TEST_CASE("count_supertree4", "[supertree]") {
	constraints c = {{0, 1, 2}};
	terraces::tree_master tm;
	CHECK(tm.count_supertree(3, c).count == 1);
}

TEST_CASE("count_supertree5", "[supertree]") {
	constraints c = {{0, 1, 2}, {2, 3, 4}};
	terraces::tree_master tm;
	CHECK(tm.count_supertree(5, c).count == 9);
}

TEST_CASE("count_supertree6", "[supertree]") {
	constraints c = {{1, 0, 2}, {3, 4, 1}};
	terraces::tree_master tm;
	CHECK(tm.count_supertree(5, c).count == 9);
}

TEST_CASE("count_supertree7", "[supertree]") {
	constraints c = {{0, 1, 3}, {3, 2, 0}, {4, 5, 6}, {6, 3, 4}, {2, 3, 6}, {2, 6, 7}};
	terraces::tree_master tm;
	CHECK(tm.count_supertree(8, c).count == 173);
}

TEST_CASE("count_supertree_none", "[supertree]") {
	constraints c = {{0, 1, 2}, {2, 1, 0}};
	terraces::tree_master tm;
	CHECK(tm.count_supertree(3, c).count == 0);
}

} // namespace tests
} // namespace terraces
