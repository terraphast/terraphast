#include <catch.hpp>

#include <algorithm>

#include <terraces/trees.hpp>

namespace terraces {
namespace tests {

TEST_CASE("tree_cursor basic navigation", "[trees],[tree_cursor]") {
	auto tree =
	        terraces::tree{{none, 1, 2},    {0, 3, 4},       {0, 5, 6},      {1, none, none},
	                       {1, none, none}, {2, none, none}, {2, none, none}};

	auto cursor = terraces::tree_cursor{tree, 0};
	CHECK(not cursor.has_parent());
	CHECK(cursor.get_index() == 0);
	CHECK(cursor.get().parent() == none);
	CHECK(cursor.get().lchild() == 1);
	CHECK(cursor.get().rchild() == 2);
	CHECK(cursor.has_lchild());
	REQUIRE(cursor.has_rchild());

	cursor.go_rchild();

	CHECK(cursor.has_parent());
	CHECK(cursor.get_index() == 2);
	CHECK(cursor.get().parent() == 0);
	CHECK(cursor.get().lchild() == 5);
	CHECK(cursor.get().rchild() == 6);
	REQUIRE(cursor.has_lchild());
	CHECK(cursor.has_rchild());

	cursor.go_lchild();

	CHECK(cursor.get_index() == 5);
	CHECK(cursor.get().lchild() == none);
	CHECK(cursor.get().rchild() == none);
	CHECK(not cursor.has_lchild());
	CHECK(not cursor.has_rchild());
	REQUIRE(cursor.has_parent());
	CHECK(cursor.get().parent() == 2);

	cursor.go_parent();

	CHECK(cursor.get_index() == 2);
}

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

TEST_CASE("is_valid_tree(valid)", "[trees]") {
	tree t{
	        {9, none, none}, {10, 9, 6},      {10, 3, 4},      {2, 5, 8},
	        {2, none, none}, {3, none, none}, {1, none, none}, {9, none, none},
	        {3, none, none}, {1, 0, 7},       {none, 1, 2},
	};
	for (auto& n : t) {
		std::random_shuffle(n.data.begin(), n.data.end());
	}
	CHECK(is_valid_tree(t));
}

TEST_CASE("is_rooted_tree(valid)", "[trees]") {
	tree t{
	        {none, 1, 2},    {0, 9, 6},       {0, 3, 4},       {2, 5, 8},
	        {2, none, none}, {3, none, none}, {1, none, none}, {9, none, none},
	        {3, none, none}, {1, 10, 7},      {9, none, none},
	};
	CHECK(is_rooted_tree(t));
}

TEST_CASE("is_valid_tree(invalid_degree)", "[trees]") {
	tree t{{none, none, none}, {0, 1, none}};
	CHECK(!is_valid_tree(t));
}

TEST_CASE("is_valid_tree(two_roots)", "[trees]") {
	tree t{{0, 1, none}, {0, 1, none}};
	CHECK(!is_valid_tree(t));
}

TEST_CASE("is_valid_tree(cycle)", "[trees]") {
	tree t{{1, 2, none}, {0, 2, 3}, {0, 1, 3}, {1, 2, none}};
	CHECK(!is_valid_tree(t));
}

TEST_CASE("is_valid_tree(dag)", "[trees]") {
	tree t{{1, 2, none}, {0, none, 3}, {0, none, 3}, {1, 2, none}};
	CHECK(!is_valid_tree(t));
}

TEST_CASE("is_rooted_tree(invalid)", "[trees]") {
	tree t{{none, 2, 3}, {2, none, none}, {0, 1, none}, {1, none, none}};
	CHECK(!is_valid_tree(t));
}

TEST_CASE("foreach_postorder(example)", "[trees]") {
	tree t{
	        {none, 1, 2},    {0, 9, 6},       {0, 3, 4},       {2, 5, 8},
	        {2, none, none}, {3, none, none}, {1, none, none}, {9, none, none},
	        {3, none, none}, {1, 10, 7},      {9, none, none},
	};
	std::vector<index> expected{10, 7, 9, 6, 1, 5, 8, 3, 4, 2, 0};
	std::vector<index> result;
	foreach_postorder(t, [&](index i) { result.push_back(i); });
	CHECK(result == expected);
}

TEST_CASE("foreach_preorder(example)", "[trees]") {
	tree t{
	        {none, 1, 2},    {0, 9, 6},       {0, 3, 4},       {2, 5, 8},
	        {2, none, none}, {3, none, none}, {1, none, none}, {9, none, none},
	        {3, none, none}, {1, 10, 7},      {9, none, none},
	};
	std::vector<index> expected{0, 1, 9, 10, 7, 6, 2, 3, 5, 8, 4};
	std::vector<index> result;
	foreach_preorder(t, [&](index i) { result.push_back(i); });
	CHECK(result == expected);
}

TEST_CASE("foreach_postorder(trivial)", "[trees]") {
	tree t{{none, none, none}};
	std::vector<index> result;
	foreach_postorder(t, [&](index i) { result.push_back(i); });
	CHECK(result.size() == 1);
	CHECK(result[0] == 0);
}

TEST_CASE("foreach_preorder(trivial)", "[trees]") {
	tree t{{none, none, none}};
	std::vector<index> result;
	foreach_preorder(t, [&](index i) { result.push_back(i); });
	CHECK(result.size() == 1);
	CHECK(result[0] == 0);
}

TEST_CASE("tree_printing", "[trees][tree-printing]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4}, {2, none, none}, {2, none, none}};
	const auto names = name_map{"root", "foo", "", "bar", "baz"};
	std::ostringstream stream;
	stream << as_newick(t, names);
	CHECK(stream.str() == "(foo,(bar,baz));");
}

} // namespace tests
} // namespace terraces
