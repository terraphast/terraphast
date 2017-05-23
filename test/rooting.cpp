
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
	std::cout << t[0] << "\n" << t[1] << "\n" << t[2] << "\n" << t[3] << "\n" << t[4] << "\n";

	const auto root_leaf_2 = 4;
	std::cout << "==========root_leaf==" << root_leaf_2 << "\n";
	reroot_inplace(t_2, root_leaf_2);
	CHECK(((t_2[0].lchild() == root_leaf_2) or (t_2[0].rchild() == root_leaf_2)));
	std::cout << t_2[0] << "\n" << t_2[1] << "\n" << t_2[2] << "\n" << t_2[3] << "\n" << t_2[4] << "\n";

	const auto root_leaf_3 = 1;
	std::cout << "==========root_leaf==" << root_leaf_3 << "\n";
	reroot_inplace(t_3, root_leaf_3);
	CHECK(((t_3[0].lchild() == root_leaf_3) or (t_3[0].rchild() == root_leaf_3)));
	std::cout << t_3[0] << "\n" << t_3[1] << "\n" << t_3[2] << "\n" << t_3[3] << "\n" << t_3[4] << "\n";

}



} // namespace tests
} // namespace terraces
