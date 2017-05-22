#include <catch.hpp>

#include <iostream>

#include <terraces/supertree.hpp>

namespace terraces {
namespace tests {

// static bool operator==(const constraint& c1, const constraint& c2) {
//        return c1.shared == c2.shared && c1.left == c2.left && c1.right == c2.right;
//}
//
// static bool operator==(const constraints& c1, const constraints& c2) {
//        bool res = true;
//        for (size_t i = 0; i < c1.size(); i++) {
//                res &= (c1.at(i) == c2.at(i));
//        }
//        return res;
//}
//
// TEST_CASE("mapping", "[mapping]") {
//        std::vector<index> leaves = { 2, 8, 5 };
//        constraints c = { {0, 2, 3}, {6, 7, 8}, {2, 5, 8} };
//        constraints t = { {0, 0, 3}, {6, 7, 2}, {0, 1, 2} };
//        CHECK(map_constraints(leaves, c) == t);
//}

TEST_CASE("supertree", "[supertree]") {
	std::vector<struct supertree_node*> s;
	std::vector<index> leaves;
	constraints c;

	leaves = {0, 1, 2, 3, 4, 5, 6};
	c = {};
	s = construct_supertree(leaves, c);
	CHECK(s.size() == 10395);

	leaves = {0, 1, 2};
	c = {{0, 1, 2}};
	s = construct_supertree(leaves, c);
	CHECK(s.size() == 1);

	leaves = {0, 1, 2, 3};
	c = {{0, 1, 2}, {2, 3, 4}};
	s = construct_supertree(leaves, c);
	CHECK(s.size() == 1);

	leaves = {0, 1, 2, 3, 4};
	c = {{1, 0, 2}, {3, 4, 1}};
	s = construct_supertree(leaves, c);
	CHECK(s.size() == 9);

	std::cout << "number of trees: " << s.size() << "\n";
	for (size_t i = 0; i < s.size(); i++) {
		std::cout << pretty_preorder(s.at(i)) << "\n";
	}
}

} // namespace tests
} // namespace terraces
