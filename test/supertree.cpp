#include <catch.hpp>

#include <iostream>

#include <terraces/supertree.hpp>

namespace terraces {
namespace tests {

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

	//	leaves = {0, 1, 2, 3, 4};
	//	c = {{1, 0, 2}, {3, 4, 1}};
	//	s = construct_supertree(leaves, c);
	//	CHECK(s.size() == 9);

	std::cout << "number of trees: " << s.size() << "\n";
	for (size_t i = 0; i < s.size(); i++) {
		std::cout << pretty_preorder(s.at(i)) << "\n";
	}
}

} // namespace tests
} // namespace terraces
