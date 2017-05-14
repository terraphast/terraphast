
#include <catch.hpp>

#include <iostream>

#include <terraces/parser.hpp>

namespace terraces {
namespace tests {

TEST_CASE("parsing a tree with just a root-node", "[parser]") {
	const auto results = parse_nwk("foo");
	const auto& tree = results.first;
	const auto& names = results.second;
	REQUIRE(tree.size() == 1);
	CHECK(tree[0].data[0] == none);
	CHECK(tree[0].data[1] == none);
	CHECK(tree[0].data[2] == none);
	CHECK(names[0] == "foo");
}


TEST_CASE("parsing a tree with one real node", "[parser]") {
	const auto results = parse_nwk("(foo)");
	const auto& tree = results.first;
	const auto& names = results.second;
	REQUIRE(tree.size() == 2);
	CHECK(tree[0].data[0] == none);
	CHECK(tree[0].data[1] == 1);
	CHECK(tree[0].data[2] == none);
	CHECK(tree[1].data[0] == 0);
	CHECK(tree[1].data[1] == none);
	CHECK(tree[1].data[2] == none);
	CHECK(names[1] == "foo");
}


TEST_CASE("parsing a tree with three leaves and two inner nodes", "[parser]") {
	const auto results = parse_nwk("((foo,bar), baz)");
	const auto& tree = results.first;
	const auto& names = results.second;
	REQUIRE(tree.size() == 5);
	CHECK(tree[0].data[0] == none);
	CHECK(tree[0].data[1] == 1);
	CHECK(tree[0].data[2] == 4);
	CHECK(tree[1].data[0] == 0);
	CHECK(tree[1].data[1] == 2);
	CHECK(tree[1].data[2] == 3);
	CHECK(tree[2].data[0] == 1);
	CHECK(tree[2].data[1] == none);
	CHECK(tree[2].data[2] == none);
	CHECK(tree[3].data[0] == 1);
	CHECK(tree[3].data[1] == none);
	CHECK(tree[3].data[2] == none);
	CHECK(tree[4].data[0] == 0);
	CHECK(tree[4].data[1] == none);
	CHECK(tree[4].data[2] == none);
	CHECK(names[2] == "foo");
	CHECK(names[3] == "bar");
	CHECK(names[4] == "baz");
}

} // namespace tests
} // namespace terraces


