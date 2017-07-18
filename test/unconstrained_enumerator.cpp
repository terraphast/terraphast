
#include <catch.hpp>

#include "../lib/unconstrained_enumerator.hpp"
#include "../lib/validation.hpp"

namespace terraces {
namespace tests {

TEST_CASE("small_bipartition", "[unconstrained]") {
	small_bipartition bip{0b00101001011100};
	auto step = [&](index i) {
		CHECK(bip.is_valid());
		CHECK(bip.left_mask() == i);
		bip.next();
	};

	CHECK(bip.is_valid());
	step(0b00000000000100);
	step(0b00000000001000);
	step(0b00000000001100);
	step(0b00000000010000);
	step(0b00000000010100);
	step(0b00000000011000);
	step(0b00000000011100);
	step(0b00000001000000);
	step(0b00000001000100);
	step(0b00000001001000);
	step(0b00000001001100);
	step(0b00000001010000);
	step(0b00000001010100);
	step(0b00000001011000);
	step(0b00000001011100);
	step(0b00001000000000);
	step(0b00001000000100);
	step(0b00001000001000);
	step(0b00001000001100);
	step(0b00001000010000);
	step(0b00001000010100);
	step(0b00001000011000);
	step(0b00001000011100);
	step(0b00001001000000);
	step(0b00001001000100);
	step(0b00001001001000);
	step(0b00001001001100);
	step(0b00001001010000);
	step(0b00001001010100);
	step(0b00001001011000);
	step(0b00001001011100);
	CHECK(!bip.is_valid());
}

TEST_CASE("unconstrained_tree_iterator small", "[unconstrained]") {
	tree t(5, {none, none, none});
	permutation leaf_perm(t.size(), 0);
	t[0] = {none, none, none};
	multitree_nodes::unconstrained leaves;
	index leaves_data[] = {0, 1, 2};
	leaves.begin = leaves_data;
	leaves.end = leaves.begin + sizeof(leaves_data) / sizeof(index);
	unconstrained_tree_iterator it{leaves, t, leaf_perm, 0};
	std::vector<std::vector<bitvector>> leaf_bipartitions{};
	index i = 1;
	do {
		it.next();
		++i;
	} while (it.has_next());
	std::sort(leaf_bipartitions.begin(), leaf_bipartitions.end());
	CHECK(i == 3);
	CHECK(std::adjacent_find(leaf_bipartitions.begin(), leaf_bipartitions.end()) ==
	      leaf_bipartitions.end());
}

TEST_CASE("unconstrained_tree_iterator", "[unconstrained]") {
	tree t(13, {none, none, none});
	permutation leaf_perm(t.size(), 0);
	t[0] = {none, 1, 2};
	t[1] = {0, none, none};
	t[2] = {0, none, none};
	leaf_perm[0] = none;
	leaf_perm[1] = 0;
	multitree_nodes::unconstrained leaves;
	index leaves_data[] = {1, 2, 3, 4, 5, 6};
	leaves.begin = leaves_data;
	leaves.end = leaves.begin + sizeof(leaves_data) / sizeof(index);
	unconstrained_tree_iterator it{leaves, t, leaf_perm, 2};
	utils::free_list fl;
	utils::stack_allocator<index> alloc{fl, bitvector::alloc_size(t.size())};
	std::vector<std::vector<bitvector>> leaf_bipartitions{};
	bool first = true;
	index i = 0;
	for (int k = 0; i < 699; ++i) {
		it.next();
	}
	do {
		if (!first) {
			++i;
			it.next();
		}
		first = false;
		leaf_bipartitions.emplace_back(tree_bipartitions(t, leaf_perm, alloc));
		// std::cout << as_newick(t, make_names(leaf_perm)) << "\n" << std::flush;
	} while (it.has_next());
	std::sort(leaf_bipartitions.begin(), leaf_bipartitions.end());
	CHECK(std::adjacent_find(leaf_bipartitions.begin(), leaf_bipartitions.end()) ==
	      leaf_bipartitions.end());
	// CHECK(leaf_bipartitions.size() == 945);
}

} // namespace tests
} // namespace terraces
