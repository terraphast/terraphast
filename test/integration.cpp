#include <catch.hpp>

#include <terraces/advanced.hpp>
#include <terraces/constraints.hpp>
#include <terraces/parser.hpp>
#include <terraces/rooting.hpp>
#include <terraces/subtree_extraction.hpp>
#include <terraces/supertree.hpp>

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
	auto names = name_map(t.size());
	index root = 2;
	auto num_leaves = remap_to_leaves(t, cs, names, root);
	CHECK(count_terrace({cs, num_leaves, root}) == 15);
}

} // namespace tests

} // namespace terraces
