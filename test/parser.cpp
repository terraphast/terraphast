
#include <catch.hpp>

#include <iostream>

#include <terraces/parser.hpp>

namespace terraces {
namespace tests {

TEST_CASE("parsing a tree with just a root-node", "[parser]") {
	auto result = parse_nwk("foo");
	REQUIRE(result.size() == 1);
	CHECK(result[0].data[0] == none);
	CHECK(result[0].data[1] == none);
	CHECK(result[0].data[2] == none);
}


TEST_CASE("parsing a tree with one real node", "[parser]") {
	auto result = parse_nwk("(foo)");
	REQUIRE(result.size() == 2);
	CHECK(result[0].data[0] == none);
	CHECK(result[0].data[1] == 1);
	CHECK(result[0].data[2] == none);
	CHECK(result[1].data[0] == 0);
	CHECK(result[1].data[1] == none);
	CHECK(result[1].data[2] == none);
}


TEST_CASE("parsing a tree with three leaves and two inner nodes", "[parser]") {
	auto result = parse_nwk("((foo,bar), baz)");
	REQUIRE(result.size() == 5);
	CHECK(result[0].data[0] == none);
	CHECK(result[0].data[1] == 1);
	CHECK(result[0].data[2] == 4);
	CHECK(result[1].data[0] == 0);
	CHECK(result[1].data[1] == 2);
	CHECK(result[1].data[2] == 3);
	CHECK(result[2].data[0] == 1);
	CHECK(result[2].data[1] == none);
	CHECK(result[2].data[2] == none);
	CHECK(result[3].data[0] == 1);
	CHECK(result[3].data[1] == none);
	CHECK(result[3].data[2] == none);
	CHECK(result[4].data[0] == 0);
	CHECK(result[4].data[1] == none);
	CHECK(result[4].data[2] == none);
}

} // namespace tests
} // namespace terraces


