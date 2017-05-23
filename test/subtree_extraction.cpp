#include <catch.hpp>

#include <terraces/subtree_extraction.hpp>

#include <algorithm>

#include <iostream>

namespace terraces {
namespace tests {

TEST_CASE("subtree extraction: full data", "[subtree_extraction]") {
	tree t{{none, 4, 5}, {2, none, none}, {4, 6, 1},      {4, none, none},
	       {0, 2, 3},    {0, none, none}, {2, none, none}};

	bitmatrix bm{t.size(), 1};
	for (index row = 0; row < t.size(); ++row) {
		if (is_leaf(t[row])) {
			bm.set(row, 0, true);
		}
	}

	auto t2 = subtrees(t, bm)[0];
	CHECK(is_rooted_tree(t));
	CHECK(is_rooted_tree(t2));
	CHECK(std::equal(t.begin(), t.end(), t2.begin(), t2.end()));
}

TEST_CASE("subtree extraction: example", "[subtree_extraction]") {
	tree t{{none, 4, 5}, {2, none, none}, {4, 6, 1},      {4, none, none},
	       {0, 2, 3},    {0, none, none}, {2, none, none}};

	bitmatrix bm{t.size(), 1};
	bm.set(0, 1, true);
	bm.set(1, 0, true);
	bm.set(1, 1, true);
	bm.set(3, 0, true);
	bm.set(5, 0, true);
	bm.set(5, 1, true);

	/*auto trees = subtrees(t, bm);
	    auto t1 = trees[0];
	    auto t2 = trees[1];

	    for (auto n : t1) {
	            std::cout << n << ",";
	    }
	    std::cout << "\n";

	    for (auto n : t2) {
	            std::cout << n << ",";
	    }
	CHECK(false);*/
}

} // namespace tests
} // namespace terraces
