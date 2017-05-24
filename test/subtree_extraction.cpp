#include <catch.hpp>

#include <terraces/subtree_extraction.hpp>

#include <algorithm>

namespace terraces {
namespace tests {

using std::vector;

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
	vector<index> exp_pre{0, 4, 2, 6, 1, 3, 5};
	vector<index> exp_post{6, 1, 2, 3, 4, 5, 0};
	auto res_pre = preorder(t2);
	auto res_post = postorder(t2);
	CHECK(exp_pre == res_pre);
	CHECK(exp_post == res_post);
}

TEST_CASE("subtree extraction: example", "[subtree_extraction]") {
	tree t{{none, 4, 5}, {2, none, none}, {4, 6, 1},      {4, none, none},
	       {0, 2, 3},    {0, none, none}, {2, none, none}};

	bitmatrix bm{t.size(), 2};
	bm.set(1, 0, true);
	bm.set(1, 1, true);
	bm.set(3, 0, true);
	bm.set(5, 0, true);
	bm.set(5, 1, true);
	bm.set(6, 1, true);

	auto trees = subtrees(t, bm);
	auto t1 = trees[0];
	auto t2 = trees[1];

	vector<index> exp_pre1{0, 4, 1, 3, 5};
	vector<index> exp_pre2{0, 2, 6, 1, 5};
	vector<index> exp_post1{1, 3, 4, 5, 0};
	vector<index> exp_post2{6, 1, 2, 5, 0};
	CHECK(exp_pre1 == preorder(trees[0]));
	CHECK(exp_pre2 == preorder(trees[1]));
	CHECK(exp_post1 == postorder(trees[0]));
	CHECK(exp_post2 == postorder(trees[1]));
}

} // namespace tests
} // namespace terraces
