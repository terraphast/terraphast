#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"

#include "terraces.h"

#include <limits.h>
#include <iostream>
#include <string>

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

TEST(ApplyConstraintsTest, merge_all_sets) {

	std::set<leaf_number> leaves = { 1, 2, 3, 4, 5 };

	std::vector<constraint> constraints;

	constraint cons1 = { 1, 3, 2, 2 };
	constraint cons2 = { 4, 4, 5, 2 };
	constraint cons3 = { 3, 3, 4, 2 };
	constraint cons4 = { 1, 1, 3, 5 };

	constraints.push_back(cons1);
	constraints.push_back(cons2);
	constraints.push_back(cons3);
	constraints.push_back(cons4);

	std::vector<std::shared_ptr<std::set<leaf_number> > > result =
			apply_constraints(leaves, constraints);

	ASSERT_EQ(result.size(), 1);

	size_t sum = 0;
	for (size_t i = 0; i < result.size(); i++) {
		for (leaf_number n : *result[i]) {
			sum += n;
		}
	}
	// all elements still contained in the sets?
	ASSERT_EQ(sum, 15);
}

TEST(ApplyConstraintsTest, no_merges) {

	std::set<leaf_number> leaves = { 1, 2, 3, 4, 5 };

	std::vector<constraint> constraints;

	constraint cons1 = { 1, 3, 6, 6 };

	constraints.push_back(cons1);

	ASSERT_DEATH(apply_constraints(leaves, constraints),
			"Assertion (.)* failed.");
}

TEST(ExtractConstraintsFromSupertree, example_from_slides) {
	auto l_1 = std::make_shared<Tree>("l1");
	auto l_2 = std::make_shared<Tree>("l2");
	auto l_3 = std::make_shared<Tree>("l3");
	auto l_4 = std::make_shared<Tree>("l4");
	auto c_1 = std::make_shared<Tree>(l_1, l_2, "c1");
	auto c_2 = std::make_shared<Tree>(l_3, l_4, "c2");
	auto r = std::make_shared<Tree>(c_1, c_2, "r");

	std::vector<constraint> constraints;
	std::set<leaf_number> leafs;
	std::tie(leafs, constraints) = extract_constraints_from_supertree(r);

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

	const char *speciesNames[] = { "s1", "s2", "s3", "s4", "s5" };

	const unsigned char matrix1[] = { 1, 0, 1, 0, 1, 1, 0, 1, 0, 1 };

	//let's initialize some missing data data structures now
	missingData *example1 = initializeMissingData(5, 2, speciesNames);
	copyDataMatrix(matrix1, example1);
	mpz_t terraceSize0;
	mpz_init(terraceSize0);
	mpz_set_ui(terraceSize0, 0);

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
	auto leaf_1 = std::make_shared<Tree>("leaf_1");
	auto leaf_2 = std::make_shared<Tree>("leaf_2");

	std::vector<std::shared_ptr<Tree> > left, right;

	left.push_back(leaf_1);
	right.push_back(leaf_2);

	std::vector<std::shared_ptr<Tree> > result = merge_subtrees(left, right);

	ASSERT_EQ(result.size(), 1);
	ASSERT_EQ(result[0]->is_leaf(), false);
	ASSERT_EQ(result[0]->left->label, std::string("leaf_1"));
	ASSERT_EQ(result[0]->right->label, std::string("leaf_2"));
}

#pragma clang diagnostic pop
