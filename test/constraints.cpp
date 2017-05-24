#include <catch.hpp>

#include <terraces/constraints.hpp>
#include <terraces/subtree_extraction.hpp>

#include <algorithm>

namespace terraces {
namespace tests {

TEST_CASE("constraints1", "[constraints]") {
	std::vector<index> leaves = {0, 1, 2};
	constraints c = {constraint{7, 8, 9}};
	std::vector<std::vector<index>> res = {{0}, {1}, {2}};
	CHECK(apply_constraints(leaves.size(), filter_constraints(leaves, c)) == res);
}

TEST_CASE("constraints2", "[constraints]") {
	std::vector<index> leaves = {0, 1, 2};
	constraints c = {constraint{7, 8, 9}, constraint{0, 1, 2}, constraint{1, 3, 2}};
	std::vector<std::vector<index>> res = {{0, 1}, {2}};
	CHECK(apply_constraints(leaves.size(), filter_constraints(leaves, c)) == res);
}

TEST_CASE("constraints3", "[constraints]") {
	std::vector<index> leaves = {0, 1, 2, 3};
	constraints c = {constraint{7, 8, 9}, constraint{0, 1, 2}, constraint{1, 3, 2}};
	std::vector<std::vector<index>> res = {{0, 1, 3}, {2}};
	CHECK(apply_constraints(leaves.size(), filter_constraints(leaves, c)) == res);
}

TEST_CASE("constraints4", "[constraints]") {
	std::vector<index> leaves = {0, 1, 2, 3};
	constraints c = {constraint{0, 1, 2}, constraint{2, 3, 0}};
	std::vector<std::vector<index>> res = {{0, 1}, {2, 3}};
	CHECK(apply_constraints(leaves.size(), filter_constraints(leaves, c)) == res);
}

TEST_CASE("constraint extraction: full data", "[subtree_extraction],[constraints]") {
	tree t{{none, 4, 5}, {2, none, none}, {4, 6, 1},      {4, none, none},
	       {0, 2, 3},    {0, none, none}, {2, none, none}};

	bitmatrix bm{t.size(), 1};
	for (index row = 0; row < t.size(); ++row) {
		if (is_leaf(t[row])) {
			bm.set(row, 0, true);
		}
	}

	auto ts = subtrees(t, bm);
	auto result = compute_constraints(ts);
	auto required = constraints{{3, 6, 5}, {1, 6, 3}};
	// auto required = constraints{{6, 3, 5}, {6, 1, 3}}; TODO These should work equivalently -
	// find out the details!
	CHECK(result == required);
}

TEST_CASE("constraint extraction: example", "[subtree_extraction],[constraints]") {
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

	auto result = compute_constraints(trees);
	auto required = constraints{{3, 1, 5}, {1, 6, 5}};
	CHECK(result == required);
}

} // namespace tests
} // namespace terraces
