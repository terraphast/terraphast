#include <catch.hpp>

#include <terraces/constraints.hpp>
#include <terraces/rooting.hpp>
#include <terraces/subtree_extraction.hpp>
#include <terraces/supertree.hpp>
#include <terraces/fast_set.hpp>

namespace terraces {

namespace tests {

TEST_CASE("full_run_disjoint", "[rerooting],[tree_extraction],[constraints],[supertree]") {
	tree t{{none, 1, 2},    {0, 3, 4},       {0, none, none}, {1, 5, 6},      {1, 7, 8},
	       {3, none, none}, {3, none, none}, {4, none, none}, {4, none, none}};
	bitmatrix b{9, 2};
	b.set(2, 0, true);
	b.set(2, 1, true);
	b.set(5, 1, true);
	b.set(6, 1, true);
	b.set(7, 0, true);
	b.set(8, 0, true);
	auto ts = subtrees(t, b);
	auto cs = compute_constraints(ts);
	fast_index_set leaves{9};
	leaves.insert(2);
	leaves.insert(5);
	leaves.insert(6);
	leaves.insert(7);
	leaves.insert(8);
	CHECK(leaves, cs) == 17);
}

} // namespace tests

} // namespace terraces
