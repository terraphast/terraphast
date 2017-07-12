
#include <catch.hpp>

#include <iostream>
#include <sstream>

#include <terraces/parser.hpp>

namespace terraces {
namespace tests {

TEST_CASE("parsing a tree with just a root-node", "[parser]") {
	const auto results = parse_nwk("foo");
	const auto& tree = results.tree;
	const auto& names = results.names;
	const auto& indices = results.indices;
	REQUIRE(tree.size() == 1);
	CHECK(tree[0].parent() == none);
	CHECK(tree[0].lchild() == none);
	CHECK(tree[0].rchild() == none);
	CHECK(names[0] == "foo");
	CHECK(indices.at("foo") == 0);
}

TEST_CASE("parsing a tree with one real node", "[parser]") {
	const auto results = parse_nwk("(foo)bar");
	const auto& tree = results.tree;
	const auto& names = results.names;
	const auto& indices = results.indices;
	REQUIRE(tree.size() == 2);
	CHECK(tree[0].parent() == none);
	CHECK(tree[0].lchild() == 1);
	CHECK(tree[0].rchild() == none);
	CHECK(tree[1].parent() == 0);
	CHECK(tree[1].lchild() == none);
	CHECK(tree[1].rchild() == none);
	CHECK(names[0] == "");
	CHECK(names[1] == "foo");
	CHECK(indices.at("foo") == 1);
	CHECK(indices.size() == 1);
}

TEST_CASE("parsing a tree with three leaves and two inner nodes", "[parser]") {
	const auto results = parse_nwk("((foo,bar)inner, baz)outer");
	const auto& tree = results.tree;
	const auto& names = results.names;
	const auto& indices = results.indices;
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
	CHECK(names[0] == "");
	CHECK(names[1] == "");
	CHECK(names[2] == "foo");
	CHECK(names[3] == "bar");
	CHECK(names[4] == "baz");
	CHECK(indices.at("foo") == 2);
	CHECK(indices.at("bar") == 3);
	CHECK(indices.at("baz") == 4);
	CHECK(indices.size() == 3);
}

TEST_CASE("parsing trees with mismatching parentheses", "[parser]") {
	// too many closing parentheses
	CHECK_THROWS_AS(parse_nwk("((,),))"), bad_input_error);
	// too many opening parentheses
	CHECK_THROWS_AS(parse_nwk("((,)"), bad_input_error);
	// too many opening parentheses
	CHECK_THROWS_AS(parse_nwk("((,),"), bad_input_error);
	// ternary nodes (simple)
	CHECK_THROWS_AS(parse_nwk("((,,),"), bad_input_error);
	// ternary nodes (complex)
	CHECK_THROWS_AS(parse_nwk("((,),((,),),)"), bad_input_error);
}

TEST_CASE("parsing trees invalid format", "[parser]") {
	// inner node names must come after their children.
	CHECK_THROWS_AS(parse_nwk("a(,)"), bad_input_error);
}

TEST_CASE("parsing a datafile with three species and two cols", "[parser],[data-parser]") {
	auto stream = std::istringstream{"3 2\n0 1 foo\n1 1 bar\n1 1 baz\n"};
	const auto map = terraces::index_map{{"foo", 2}, {"bar", 1}, {"baz", 3}};
	const auto res = terraces::parse_bitmatrix(stream, map, 5);
	const auto& mat = res.first;
	const auto& root_species = res.second;

	REQUIRE(mat.cols() == 2);
	REQUIRE(mat.rows() == 5);

	CHECK(!mat.get(0, 0));
	CHECK(!mat.get(0, 1));
	CHECK(mat.get(1, 0));
	CHECK(mat.get(1, 1));
	CHECK(!mat.get(2, 0));
	CHECK(mat.get(2, 1));
	CHECK(mat.get(3, 0));
	CHECK(mat.get(3, 1));

	CHECK(root_species == 1);
}

TEST_CASE("parsing a datafile with unknown species", "[parser],[data-parser]") {
	auto stream = std::istringstream{"3 2\n0 1 f00\n1 1 bar\n1 1 baz\n"};
	const auto map = terraces::index_map{{"foo", 2}, {"bar", 1}, {"baz", 3}};
	CHECK_THROWS_AS(terraces::parse_bitmatrix(stream, map, 5), bad_input_error);
}

TEST_CASE("parsing a datafile with missing species", "[parser],[data-parser]") {
	auto stream = std::istringstream{"3 2\n1 1 bar\n1 1 baz\n"};
	const auto map = terraces::index_map{{"foo", 2}, {"bar", 1}, {"baz", 3}};
	CHECK_THROWS_AS(terraces::parse_bitmatrix(stream, map, 5), bad_input_error);
}

TEST_CASE("parsing a complex datafile", "[parser],[data-parser]") {
	auto stream = std::istringstream{"5 3\n"
	                                 "0 1 0 foo\n"
	                                 "1 1 1 bar\n"
	                                 "0 1 0 bla\n"
	                                 "1 0 1 blub\n"
	                                 "1 1 0 gähn\n"};
	const auto map =
	        terraces::index_map{{"foo", 1}, {"bar", 2}, {"bla", 3}, {"blub", 4}, {"gähn", 5}};
	const auto res = terraces::parse_bitmatrix(stream, map, 9);
	const auto& mat = res.first;
	const auto& root_species = res.second;

	REQUIRE(mat.cols() == 3);
	REQUIRE(mat.rows() == 9);

	CHECK(!mat.get(0, 0));
	CHECK(!mat.get(0, 1));
	CHECK(!mat.get(0, 2));
	CHECK(!mat.get(1, 0));
	CHECK(mat.get(1, 1));
	CHECK(!mat.get(1, 2));
	CHECK(mat.get(2, 0));
	CHECK(mat.get(2, 1));
	CHECK(mat.get(2, 2));

	CHECK(root_species == 2);
}

} // namespace tests
} // namespace terraces
