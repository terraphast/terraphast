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

TEST_CASE("check_supertree1", "[supertree]") {
	constraints c = {};
	CHECK(check_supertree(2, c) == false);
}

TEST_CASE("check_supertree2", "[supertree]") {
	constraints c = {};
	CHECK(check_supertree(3, c) == true);
}

TEST_CASE("check_supertree3", "[supertree]") {
	constraints c = {};
	CHECK(check_supertree(7, c) == true);
}

TEST_CASE("check_supertree4", "[supertree]") {
	constraints c = {{0, 1, 2}};
	CHECK(check_supertree(3, c) == false);
}

TEST_CASE("check_supertree5", "[supertree]") {
	constraints c = {{0, 1, 2}, {2, 3, 4}};
	CHECK(check_supertree(4, c) == false);
}

TEST_CASE("check_supertree6", "[supertree]") {
	constraints c = {{1, 0, 2}, {3, 4, 1}};
	CHECK(check_supertree(5, c) == true);
}

TEST_CASE("count_supertree1", "[supertree]") {
	constraints c = {};
	CHECK(count_supertree(2, c) == 1);
}

TEST_CASE("count_supertree2", "[supertree]") {
	constraints c = {};
	CHECK(count_supertree(3, c) == 3);
}

TEST_CASE("count_supertree3", "[supertree]") {
	constraints c = {};
	CHECK(count_supertree(7, c) == 10395);
}

TEST_CASE("count_supertree4", "[supertree]") {
	constraints c = {{0, 1, 2}};
	CHECK(count_supertree(3, c) == 1);
}

TEST_CASE("count_supertree5", "[supertree]") {
	constraints c = {{0, 1, 2}, {2, 3, 4}};
	CHECK(count_supertree(4, c) == 1);
}

TEST_CASE("count_supertree6", "[supertree]") {
	constraints c = {{1, 0, 2}, {3, 4, 1}};
	CHECK(count_supertree(5, c) == 9);
}

TEST_CASE("enum_supertree1", "[supertree]") {
	constraints c = {};
	std::vector<struct supertree_node*> s = construct_supertree(7, c);
	CHECK(s.size() == 10395);
}

TEST_CASE("enum_supertree2", "[supertree]") {
	constraints c = {{0, 1, 2}};
	std::vector<struct supertree_node*> s = construct_supertree(3, c);
	CHECK(s.size() == 1);
}

TEST_CASE("enum_supertree3", "[supertree]") {
	constraints c = {{0, 1, 2}, {2, 3, 4}};
	std::vector<struct supertree_node*> s = construct_supertree(4, c);
	CHECK(s.size() == 1);
}

TEST_CASE("enum_supertree4", "[supertree]") {
	constraints c = {{1, 0, 2}, {3, 4, 1}};
	std::vector<struct supertree_node*> s = construct_supertree(5, c);
	CHECK(s.size() == 9);
}

} // namespace tests
} // namespace terraces
