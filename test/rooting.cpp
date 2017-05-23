
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
	std::cout << "==========root_leaf==" << root_leaf << "\n";
	reroot_inplace(t, root_leaf);
	CHECK(((t[0].lchild() == root_leaf) or (t[0].rchild() == root_leaf)));
	std::cout << t;

	const auto root_leaf_2 = 4;
	std::cout << "==========root_leaf==" << root_leaf_2 << "\n";
	reroot_inplace(t_2, root_leaf_2);
	CHECK(((t_2[0].lchild() == root_leaf_2) or (t_2[0].rchild() == root_leaf_2)));
	std::cout << t_2;

	const auto root_leaf_3 = 1;
	std::cout << "==========root_leaf==" << root_leaf_3 << "\n";
	reroot_inplace(t_3, root_leaf_3);
	CHECK(((t_3[0].lchild() == root_leaf_3) or (t_3[0].rchild() == root_leaf_3)));
	std::cout << t_3;
}

TEST_CASE("rerooting advanced at 1", "[rerooting]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4},
	              {2, 5, 6},    {2, none, none}, {3, none, none},
	              {3, 7, 8},    {6, none, none}, {6, none, none}};

	const auto root_leaf = 1;
	std::cout << "==========root_leaf==" << root_leaf << "\n";
	reroot_inplace(t, root_leaf);
	CHECK(((t[0].lchild() == root_leaf) or (t[0].rchild() == root_leaf)));
	std::cout << t;
}

TEST_CASE("rerooting advanced at 4", "[rerooting]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4},
	              {2, 5, 6},    {2, none, none}, {3, none, none},
	              {3, 7, 8},    {6, none, none}, {6, none, none}};

	const auto root_leaf = 4;
	std::cout << "==========root_leaf==" << root_leaf << "\n";
	reroot_inplace(t, root_leaf);
	CHECK(((t[0].lchild() == root_leaf) or (t[0].rchild() == root_leaf)));
	std::cout << t;
}

TEST_CASE("rerooting advanced at 5", "[rerooting]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4},
	              {2, 5, 6},    {2, none, none}, {3, none, none},
	              {3, 7, 8},    {6, none, none}, {6, none, none}};

	const auto root_leaf = 5;
	std::cout << "==========root_leaf==" << root_leaf << "\n";
	reroot_inplace(t, root_leaf);
	CHECK(((t[0].lchild() == root_leaf) or (t[0].rchild() == root_leaf)));
	std::cout << t;
}

TEST_CASE("rerooting advanced at 7", "[rerooting]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4},
	              {2, 5, 6},    {2, none, none}, {3, none, none},
	              {3, 7, 8},    {6, none, none}, {6, none, none}};

	const auto root_leaf = 7;
	std::cout << "==========root_leaf==" << root_leaf << "\n";
	reroot_inplace(t, root_leaf);
	CHECK(((t[0].lchild() == root_leaf) or (t[0].rchild() == root_leaf)));
	std::cout << t;
}

TEST_CASE("rerooting advanced at 8", "[rerooting]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4},
	              {2, 5, 6},    {2, none, none}, {3, none, none},
	              {3, 7, 8},    {6, none, none}, {6, none, none}};

	const auto root_leaf = 8;
	std::cout << "==========root_leaf==" << root_leaf << "\n";
	reroot_inplace(t, root_leaf);
	CHECK(((t[0].lchild() == root_leaf) or (t[0].rchild() == root_leaf)));
	std::cout << t;
}
} // namespace tests
} // namespace terraces
