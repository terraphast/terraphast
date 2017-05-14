
#include <catch.hpp>

#include <iostream>

#include <terraces/parser.hpp>

namespace terraces {
namespace tests {

TEST_CASE("parsing a tree with just a root-node", "[parser]") {
	const auto results = parse_nwk("foo");
	const auto& tree = results.tree;
	const auto& names = results.names;
	const auto& indeces = results.indeces;
	REQUIRE(tree.size() == 1);
	CHECK(tree[0].parent() == none);
	CHECK(tree[0].lchild() == none);
	CHECK(tree[0].rchild() == none);
	CHECK(names[0] == "foo");
	CHECK(indeces.at("foo") == 0);
}

TEST_CASE("parsing a tree with one real node", "[parser]") {
	const auto results = parse_nwk("(foo)");
	const auto& tree = results.tree;
	const auto& names = results.names;
	const auto& indeces = results.indeces;
	REQUIRE(tree.size() == 2);
	CHECK(tree[0].parent() == none);
	CHECK(tree[0].lchild() == 1);
	CHECK(tree[0].rchild() == none);
	CHECK(tree[1].parent() == 0);
	CHECK(tree[1].lchild() == none);
	CHECK(tree[1].rchild() == none);
	CHECK(names[1] == "foo");
	CHECK(indeces.at("foo") == 1);
}

TEST_CASE("parsing a tree with three leaves and two inner nodes", "[parser]") {
	const auto results = parse_nwk("((foo,bar), baz)");
	const auto& tree = results.tree;
	const auto& names = results.names;
	const auto& indeces = results.indeces;
	REQUIRE(tree.size() == 5);
	CHECK(tree[0].parent() == none);
	CHECK(tree[0].lchild() == 1);
	CHECK(tree[0].rchild() == 4);
	CHECK(tree[1].parent() == 0);
	CHECK(tree[1].lchild() == 2);
	CHECK(tree[1].rchild() == 3);
	CHECK(tree[2].parent() == 1);
	CHECK(tree[2].lchild() == none);
	CHECK(tree[2].rchild() == none);
	CHECK(tree[3].parent() == 1);
	CHECK(tree[3].lchild() == none);
	CHECK(tree[3].rchild() == none);
	CHECK(tree[4].parent() == 0);
	CHECK(tree[4].lchild() == none);
	CHECK(tree[4].rchild() == none);
	CHECK(names[2] == "foo");
	CHECK(names[3] == "bar");
	CHECK(names[4] == "baz");
	CHECK(indeces.at("foo") == 2);
	CHECK(indeces.at("bar") == 3);
	CHECK(indeces.at("baz") == 4);
}

} // namespace tests
} // namespace terraces
