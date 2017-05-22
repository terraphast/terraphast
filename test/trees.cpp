#include <catch.hpp>

#include <iostream>

#include <algorithm>
#include <terraces/trees.hpp>

namespace terraces {
namespace tests {

TEST_CASE("is_root(root)", "[trees]") {
	auto root_node = terraces::node{none, 1, 2};
	CHECK(terraces::is_root(root_node));
}

TEST_CASE("is_root(non_root)", "[trees]") {
	auto non_root_node = terraces::node{1, 2, 3};
	CHECK(!terraces::is_root(non_root_node));
}

TEST_CASE("is_leaf(leaf)", "[trees]") {
	auto leaf_node = terraces::node{0, none, none};
	CHECK(terraces::is_leaf(leaf_node));
}

TEST_CASE("is_leaf(non_leaf)", "[trees]") {
	auto non_leaf_node = terraces::node{0, 1, 2};
	CHECK(!terraces::is_leaf(non_leaf_node));
}

TEST_CASE("foreach_postorder(example)", "[trees]") {
	tree t{
	        {9, none, none}, {10, 9, 6},      {10, 3, 4},      {2, 5, 8},
	        {2, none, none}, {3, none, none}, {1, none, none}, {9, none, none},
	        {3, none, none}, {1, 0, 7},       {none, 1, 2},
	};
	std::vector<index> expected{0, 7, 9, 6, 1, 5, 8, 3, 4, 2, 10};
	std::vector<index> result;
	foreach_postorder(t, 10, [&](index i) { result.push_back(i); });
	CHECK(std::equal(expected.begin(), expected.end(), result.begin(), result.end()));
}

TEST_CASE("foreach_postorder(trivial)", "[trees]") {
	tree t{{none, none, none}};
	std::vector<index> result;
	foreach_postorder(t, 0, [&](index i) { result.push_back(i); });
	CHECK(result.size() == 1);
	CHECK(result[0] == 0);
}

} // namespace tests
} // namespace terraces
