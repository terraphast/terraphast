
#include <catch.hpp>
#include <iostream>

#include <terraces/rooting.hpp>
#include <terraces/trees.hpp>

namespace terraces {
namespace tests {

TEST_CASE("rerooting basic", "[rerooting]") {
	auto t = tree{{none, 1, 2}, {0, none, none}, {0, 3, 4}, {2, none, none}, {2, none, none}};
	const auto root_leaf = 3;
	reroot_inplace(t, root_leaf);

	// TODO: implement the algorithm, make this work and add further tests:
	CHECK(((t[0].lchild() == root_leaf) or (t[0].rchild() == root_leaf)));
	std::cout << t[0] << "\n" << t[1] << "\n" << t[2] << "\n" << t[3] << "\n" << t[4];
	std::cout << t[0].parent() + 1;
}

} // namespace tests
} // namespace terraces
