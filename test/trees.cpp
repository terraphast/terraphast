#include <catch.hpp>

#include <iostream>

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

} // namespace tests
} // namespace terraces
