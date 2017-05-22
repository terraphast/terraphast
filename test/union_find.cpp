#include <catch.hpp>

#include <iostream>

#include <terraces/union_find.hpp>

namespace terraces {
namespace tests {

TEST_CASE("union_find1", "[union_find]") {
	union_find leaves = make_set(3);
	merge(leaves, 0, 1);
	CHECK(find(leaves, 0) == find(leaves, 1));
	CHECK(find(leaves, 2) == 2);
}

TEST_CASE("union_find2", "[union_find]") {
	union_find leaves = make_set(5);
	merge(leaves, 0, 1);
	merge(leaves, 1, 4);
	CHECK(find(leaves, 0) == find(leaves, 1));
	CHECK(find(leaves, 1) == find(leaves, 4));
	CHECK(find(leaves, 2) == 2);
	CHECK(find(leaves, 3) == 3);
}

TEST_CASE("to_set_of_sets1", "[union_find]") {
	union_find leaves = make_set(3);
	merge(leaves, 0, 1);
	std::vector<std::vector<index>> res = {{0, 1}, {2}};
	CHECK(to_set_of_sets(leaves) == res);
}

TEST_CASE("to_set_of_sets2", "[union_find]") {
	union_find leaves = make_set(4);
	merge(leaves, 0, 1);
	merge(leaves, 1, 3);
	std::vector<std::vector<index>> res = {{0, 1, 3}, {2}};
	CHECK(to_set_of_sets(leaves) == res);
}

} // namespace tests
} // namespace terraces
