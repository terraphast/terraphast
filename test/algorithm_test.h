#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmp.h"
#include "gmock/gmock-matchers.h"

#include "terraces.h"

#include <limits.h>
#include <iostream>
#include <string>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma clang diagnostic ignored "-Wcovered-switch-default"

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

	leaf_number sum = 0;
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

	leaf_number sum = 0;
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

TEST(ApplyConstraintsTest, merges_to_two_parts) {

	std::set<leaf_number> leaves = { 1, 2, 3, 4 };

	std::vector<constraint> constraints;

	constraint cons1 = { 1, 1, 2, 3 };
	constraint cons2 = { 1, 1, 3, 4 };

	constraints.push_back(cons1);
	constraints.push_back(cons2);

	auto result = apply_constraints(leaves, constraints);

	ASSERT_EQ(result.size(), 2);
	ASSERT_EQ(result[0]->size(), 3);
	ASSERT_TRUE(result[0]->count(1) == 1);
	ASSERT_TRUE(result[0]->count(2) == 1);
	ASSERT_TRUE(result[0]->count(3) == 1);
	ASSERT_EQ(result[1]->size(), 1);
	ASSERT_TRUE(result[1]->count(4) == 1);
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

TEST(GetNthPartitionTuple, example_with_two_parts) {

	std::set<leaf_number> part1 = { 1, 2 };
	std::set<leaf_number> part2 = { 3 };

	std::vector<std::shared_ptr<std::set<leaf_number> > > partitions;
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part1));
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part2));

	ASSERT_EQ(number_partition_tuples(partitions), 1);

	std::shared_ptr<std::set<leaf_number> > part_one;
	std::shared_ptr<std::set<leaf_number> > part_two;
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 1);
	ASSERT_THAT(*part_one, testing::ElementsAre(1, 2));
	ASSERT_THAT(*part_two, testing::ElementsAre(3));
}

TEST(GetNthPartitionTuple, example_from_slides) {

	std::set<leaf_number> part1 = { 1, 2 };
	std::set<leaf_number> part2 = { 3 };
	std::set<leaf_number> part3 = { 4, 5 };

	std::vector<std::shared_ptr<std::set<leaf_number> > > partitions;
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part1));
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part2));
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part3));

	ASSERT_EQ(number_partition_tuples(partitions), 3);

	std::shared_ptr<std::set<leaf_number> > part_one;
	std::shared_ptr<std::set<leaf_number> > part_two;
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 1);
	ASSERT_THAT(*part_one, testing::ElementsAre(1, 2));
	ASSERT_THAT(*part_two, testing::ElementsAre(3, 4, 5));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 2);
	ASSERT_THAT(*part_one, testing::ElementsAre(3));
	ASSERT_THAT(*part_two, testing::ElementsAre(1, 2, 4, 5));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 3);
	ASSERT_THAT(*part_one, testing::ElementsAre(1, 2, 3));
	ASSERT_THAT(*part_two, testing::ElementsAre(4, 5));
}

TEST(GetNthPartitionTuple, with_four_partitions) {

	std::set<leaf_number> part1 = { 1, 2 };
	std::set<leaf_number> part2 = { 3 };
	std::set<leaf_number> part3 = { 4, 5 };
	std::set<leaf_number> part4 = { 6, 7, 8 };

	std::vector<std::shared_ptr<std::set<leaf_number> > > partitions;
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part1));
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part2));
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part3));
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part4));

	ASSERT_EQ(number_partition_tuples(partitions), 7);

	std::shared_ptr<std::set<leaf_number> > part_one;
	std::shared_ptr<std::set<leaf_number> > part_two;
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 1);
	ASSERT_THAT(*part_one, testing::ElementsAre(1, 2));
	ASSERT_THAT(*part_two, testing::ElementsAre(3, 4, 5, 6, 7, 8));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 2);
	ASSERT_THAT(*part_one, testing::ElementsAre(3));
	ASSERT_THAT(*part_two, testing::ElementsAre(1, 2, 4, 5, 6, 7, 8));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 3);
	ASSERT_THAT(*part_one, testing::ElementsAre(1, 2, 3));
	ASSERT_THAT(*part_two, testing::ElementsAre(4, 5, 6, 7, 8));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 4);
	ASSERT_THAT(*part_one, testing::ElementsAre(4, 5));
	ASSERT_THAT(*part_two, testing::ElementsAre(1, 2, 3, 6, 7, 8));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 5);
	ASSERT_THAT(*part_one, testing::ElementsAre(1, 2, 4, 5));
	ASSERT_THAT(*part_two, testing::ElementsAre(3, 6, 7, 8));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 6);
	ASSERT_THAT(*part_one, testing::ElementsAre(3, 4, 5));
	ASSERT_THAT(*part_two, testing::ElementsAre(1, 2, 6, 7, 8));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 7);
	ASSERT_THAT(*part_one, testing::ElementsAre(1, 2, 3, 4, 5));
	ASSERT_THAT(*part_two, testing::ElementsAre(6, 7, 8));
}

//TODO not implemented yet
TEST(CombineSets, example1_from_slides) {

	std::set<leaf_number> leaves = { 1, 2, 3, 4, 5 };

	std::vector<constraint> constraints;

	constraint cons1 = { 1, 3, 2, 2 };
	constraint cons2 = { 4, 4, 5, 2 };

	constraints.push_back(cons1);
	constraints.push_back(cons2);

	auto result = apply_constraints(leaves, constraints);

	for (auto &one_set : result) {
		fprintf(stderr, "{");
		for (auto &elem : *one_set) {
			fprintf(stderr, "%d ", elem);
		}
		fprintf(stderr, "}");
	}
	fprintf(stderr, "\n");
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
