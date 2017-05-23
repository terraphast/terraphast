
#include <catch.hpp>
#include <iostream>

#include <terraces/rooting.hpp>
#include <terraces/trees.hpp>

namespace terraces {
namespace tests {

TEST_CASE("rerooting basic", "[rerooting]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4}, {2, none, none}, {2, none, none}};
	auto t_2 = tree(t);
	auto t_3 = tree(t);

	const auto root_leaf = 3;
	reroot_inplace(t, root_leaf);
	CHECK(((t[0].lchild() == root_leaf) or (t[0].rchild() == root_leaf)));

	const auto root_leaf_2 = 4;
	reroot_inplace(t_2, root_leaf_2);
	CHECK(((t_2[0].lchild() == root_leaf_2) or (t_2[0].rchild() == root_leaf_2)));

	const auto root_leaf_3 = 1;
	reroot_inplace(t_3, root_leaf_3);
	CHECK(((t_3[0].lchild() == root_leaf_3) or (t_3[0].rchild() == root_leaf_3)));
}

TEST_CASE("rerooting advanced at 1", "[rerooting]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4},
	              {2, 5, 6},    {2, none, none}, {3, none, none},
	              {3, 7, 8},    {6, none, none}, {6, none, none}};

	const auto root_leaf = 1;
	reroot_inplace(t, root_leaf);
	CHECK(((t[0].lchild() == root_leaf) or (t[0].rchild() == root_leaf)));
}

TEST_CASE("rerooting advanced at 2 (should fail)", "[rerooting]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4},
	              {2, 5, 6},    {2, none, none}, {3, none, none},
	              {3, 7, 8},    {6, none, none}, {6, none, none}};

	const auto root_leaf = 2;
	CHECK_THROWS_AS( reroot_inplace(t, root_leaf), std::invalid_argument );
}

TEST_CASE("rerooting advanced at 3 (should fail)", "[rerooting]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4},
	              {2, 5, 6},    {2, none, none}, {3, none, none},
	              {3, 7, 8},    {6, none, none}, {6, none, none}};

	const auto root_leaf = 3;
	CHECK_THROWS_AS( reroot_inplace(t, root_leaf), std::invalid_argument );
}

TEST_CASE("rerooting advanced at 4", "[rerooting]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4},
	              {2, 5, 6},    {2, none, none}, {3, none, none},
	              {3, 7, 8},    {6, none, none}, {6, none, none}};

	const auto root_leaf = 4;
	reroot_inplace(t, root_leaf);
	CHECK(((t[0].lchild() == root_leaf) or (t[0].rchild() == root_leaf)));
}

TEST_CASE("rerooting advanced at 5", "[rerooting]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4},
	              {2, 5, 6},    {2, none, none}, {3, none, none},
	              {3, 7, 8},    {6, none, none}, {6, none, none}};

	const auto root_leaf = 5;
	reroot_inplace(t, root_leaf);
	CHECK(((t[0].lchild() == root_leaf) or (t[0].rchild() == root_leaf)));
}

TEST_CASE("rerooting advanced at 6 (should fail)", "[rerooting]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4},
	              {2, 5, 6},    {2, none, none}, {3, none, none},
	              {3, 7, 8},    {6, none, none}, {6, none, none}};

	const auto root_leaf = 6;
	CHECK_THROWS_AS( reroot_inplace(t, root_leaf), std::invalid_argument );
}

TEST_CASE("rerooting advanced at 7", "[rerooting]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4},
	              {2, 5, 6},    {2, none, none}, {3, none, none},
	              {3, 7, 8},    {6, none, none}, {6, none, none}};

	const auto root_leaf = 7;
	reroot_inplace(t, root_leaf);
	CHECK(((t[0].lchild() == root_leaf) or (t[0].rchild() == root_leaf)));
}

TEST_CASE("rerooting advanced at 8", "[rerooting]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4},
	              {2, 5, 6},    {2, none, none}, {3, none, none},
	              {3, 7, 8},    {6, none, none}, {6, none, none}};

	const auto root_leaf = 8;
	reroot_inplace(t, root_leaf);
	CHECK(((t[0].lchild() == root_leaf) or (t[0].rchild() == root_leaf)));
}
} // namespace tests
} // namespace terraces
