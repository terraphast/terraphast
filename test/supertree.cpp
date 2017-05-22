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

TEST_CASE("supertree1", "[supertree]") {
	std::vector<index> leaves = {0, 1, 2, 3, 4, 5, 6};
	constraints c = {};
	std::vector<struct supertree_node*> s = construct_supertree(leaves, c);
	CHECK(s.size() == 10395);
}

TEST_CASE("supertree2", "[supertree]") {
	std::vector<index> leaves = {0, 1, 2};
	constraints c = {{0, 1, 2}};
	std::vector<struct supertree_node*> s = construct_supertree(leaves, c);
	CHECK(s.size() == 1);
}

TEST_CASE("supertree3", "[supertree]") {
	std::vector<index> leaves = {0, 1, 2, 3};
	constraints c = {{0, 1, 2}, {2, 3, 4}};
	std::vector<struct supertree_node*> s = construct_supertree(leaves, c);
	CHECK(s.size() == 1);
}

TEST_CASE("supertree4", "[supertree]") {
	std::vector<index> leaves = {0, 1, 2, 3, 4};
	constraints c = {{1, 0, 2}, {3, 4, 1}};
	std::vector<struct supertree_node*> s = construct_supertree(leaves, c);
	CHECK(s.size() == 9);
}

} // namespace tests
} // namespace terraces
