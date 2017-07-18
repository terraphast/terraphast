#include <catch.hpp>

#include <terraces/constraints.hpp>
#include <terraces/subtree_extraction.hpp>

#include <algorithm>

namespace terraces {
namespace tests {

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

TEST_CASE("constraint deduplication", "[deduplication], [contraints]") {
	auto dup = constraints{{0, 1, 2}, {0, 1, 2}, {3, 4, 5}, {4, 3, 5}, {7, 6, 8}, {6, 7, 8}};
	auto num = deduplicate_constraints(dup);
	CHECK(num == 3);
	CHECK(dup == (constraints{{0, 1, 2}, {3, 4, 5}, {6, 7, 8}}));
}

} // namespace tests
} // namespace terraces
