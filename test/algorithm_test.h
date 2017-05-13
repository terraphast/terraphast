#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"

#include "terraces.h"

#include <limits.h>
#include <iostream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"

TEST(ApplyConstraintsTest, example_from_slides) {

	std::set<leaf_number> leaves = { 1, 2, 3, 4, 5 };

	std::vector<constraint> constraints;

	constraint cons1 = { 1, 3, 2, 2 };
	constraint cons2 = { 4, 4, 5, 2 };

	constraints.push_back(cons1);
	constraints.push_back(cons2);

	std::vector<std::shared_ptr<std::set<leaf_number> > > result =
			apply_constraints(leaves, constraints);

	ASSERT_EQ(result.size(), 3);

	size_t sum = 0;
	for (size_t i = 0; i < result.size(); i++) {
		for (leaf_number n : *result[i]) {
			sum += n;
		}
	}
	// all elements still contained in the sets?
	ASSERT_EQ(sum, 15);
}

TEST(ExtractConstraintsFromSupertree, example_from_slides) {
	binary_tree l_1 = { .is_leaf = true, .label = "l1", .left_subtree = nullptr,
			.right_subtree = nullptr };
	binary_tree l_2 = { .is_leaf = true, .label = "l2", .left_subtree = nullptr,
			.right_subtree = nullptr };
	binary_tree l_3 = { .is_leaf = true, .label = "l3", .left_subtree = nullptr,
			.right_subtree = nullptr };
	binary_tree l_4 = { .is_leaf = true, .label = "l4", .left_subtree = nullptr,
			.right_subtree = nullptr };
	binary_tree c_1 = { .is_leaf = false, .label = "c1", .left_subtree = &l_1,
			.right_subtree = &l_2 };
	binary_tree c_2 = { .is_leaf = false, .label = "c2", .left_subtree = &l_3,
			.right_subtree = &l_4 };
	binary_tree r = { .is_leaf = false, .label = "r", .left_subtree = &c_1,
			.right_subtree = &c_2 };

	std::vector<constraint> constraints;
	std::set<leaf_number> leafs;
	std::tie(leafs, constraints) = extract_constraints_from_supertree(&r);

	ASSERT_EQ(leafs.size(), 4);
	ASSERT_EQ(*std::next(leafs.begin(), 0), 0);
	ASSERT_EQ(*std::next(leafs.begin(), 1), 1);
	ASSERT_EQ(*std::next(leafs.begin(), 2), 2);
	ASSERT_EQ(*std::next(leafs.begin(), 3), 3);

	ASSERT_EQ(constraints.size(), 2);
	//lca(0, 1) < lca(0, 3)
	ASSERT_EQ(constraints[0].smaller_left, 0);
	ASSERT_EQ(constraints[0].smaller_right, 1);
	ASSERT_EQ(constraints[0].bigger_left, 0);
	ASSERT_EQ(constraints[0].bigger_right, 3);

	//lca(2, 3) < lca(0, 3)
	ASSERT_EQ(constraints[1].smaller_left, 2);
	ASSERT_EQ(constraints[1].smaller_right, 3);
	ASSERT_EQ(constraints[1].bigger_left, 0);
	ASSERT_EQ(constraints[1].bigger_right, 3);
}

//TODO not implemented yet
TEST(CombineSets, example1_from_slides) {
	ntree_t *tree = get_newk_tree("test/dummy_tree1_rooted.nwk");
	rtree_t *r_tree = ntree_to_rtree(tree);

	const char *speciesNames[] = { "s1", "s2", "s3", "s4", "s5" };

	const unsigned char matrix1[] = { 1, 0, 1, 0, 1, 1, 0, 1, 0, 1 };

	//let's initialize some missing data data structures now
	missingData *example1 = initializeMissingData(5, 2, speciesNames);
	copyDataMatrix(matrix1, example1);
	mpz_t terraceSize0;
	mpz_init(terraceSize0);
	mpz_set_ui(terraceSize0, 0);

	std::map<const char*, unsigned char, cmp_str> species_map;
	for (unsigned char i = 0; i < example1->numberOfSpecies; i++) {
		species_map[example1->speciesNames[i]] = i;
	}

//	d_print_tree(r_tree);
//	std::vector<binary_tree> all_trees = combine_sets(
//				apply_constraints(leaves, constraints), constraints);
}

TEST(FindConstraintsTest, example_from_slides) {
	std::set<leaf_number> leaves = { 1, 2, 3 };

	std::vector<constraint> constraints;

	constraint cons1 = { 1, 3, 2, 2 };
	constraint cons2 = { 4, 4, 5, 2 };

	constraints.push_back(cons1);
	constraints.push_back(cons2);

	std::vector<constraint> result = find_constraints(leaves, constraints);

	ASSERT_EQ(result.size(), 1);
	ASSERT_EQ(result[0].smaller_left, 1);
	ASSERT_EQ(result[0].bigger_left, 3);
	ASSERT_EQ(result[0].smaller_right, 2);
	ASSERT_EQ(result[0].bigger_right, 2);
}

TEST(MergeSubtreesTest, simple_tree) {
	binary_tree leaf_1 = { .is_leaf = true, .label = "leaf_1", .left_subtree =
			nullptr, .right_subtree = nullptr };
	binary_tree leaf_2 = { .is_leaf = true, .label = "leaf_2", .left_subtree =
			nullptr, .right_subtree = nullptr };

	std::vector<binary_tree> left, right;

	left.push_back(leaf_1);
	right.push_back(leaf_2);

	std::vector<binary_tree> result = merge_subtrees(left, right);

	ASSERT_EQ(result.size(), 1);
	ASSERT_EQ(result[0].is_leaf, false);
	ASSERT_STREQ(result[0].left_subtree->label, "leaf_1");
	ASSERT_STREQ(result[0].right_subtree->label, "leaf_2");
}

#pragma clang diagnostic pop
