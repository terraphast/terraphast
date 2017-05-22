#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmp.h"
#include "gmock/gmock-matchers.h"

#include "terraces.h"

#include <limits.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wused-but-marked-unused"
#pragma clang diagnostic ignored "-Wcovered-switch-default"

TEST(ApplyConstraintsTest, example_from_slides) {

	std::set<leaf_number> leaves = { "1", "2", "3", "4", "5" };

	std::vector<constraint> constraints;

	constraint cons1 = { "1", "3", "2", "2" };
	constraint cons2 = { "4", "4", "5", "2" };

	constraints.push_back(cons1);
	constraints.push_back(cons2);

	std::vector<std::shared_ptr<std::set<leaf_number> > > result =
			apply_constraints(leaves, constraints);

	ASSERT_EQ(result.size(), 3);

	ASSERT_THAT(*result[0], testing::ElementsAre("1", "2"));
	ASSERT_THAT(*result[1], testing::ElementsAre("3"));
	ASSERT_THAT(*result[2], testing::ElementsAre("4", "5"));
}

TEST(ApplyConstraintsTest, merge_all_sets) {

	std::set<leaf_number> leaves = { "1", "2", "3", "4", "5" };

	std::vector<constraint> constraints;

	constraint cons1 = { "1", "3", "2", "2" };
	constraint cons2 = { "4", "4", "5", "2" };
	constraint cons3 = { "3", "3", "4", "2" };
	constraint cons4 = { "1", "1", "3", "5" };

	constraints.push_back(cons1);
	constraints.push_back(cons2);
	constraints.push_back(cons3);
	constraints.push_back(cons4);

	std::vector<std::shared_ptr<std::set<leaf_number> > > result =
			apply_constraints(leaves, constraints);

	ASSERT_EQ(result.size(), 1);

	ASSERT_THAT(*result[0], testing::ElementsAre("1", "2", "3", "4", "5"));

}

TEST(ApplyConstraintsTest, no_merges) {

	std::set<leaf_number> leaves = { "1", "2", "3", "4", "5" };

	std::vector<constraint> constraints;

	constraint cons1 = { "1", "3", "6", "6" };

	constraints.push_back(cons1);

	ASSERT_DEATH(apply_constraints(leaves, constraints),
			"Assertion (.)* failed.");
}

TEST(GetAllBinaryTrees, with_tree_leafs) {

	std::set<leaf_number> leafs = { "1", "2", "3" };

	auto result = get_all_binary_trees(leafs);

	ASSERT_EQ(result.size(), 3);

	ASSERT_EQ(result[0]->to_newick_string(), "((3,1),2);");
	ASSERT_EQ(result[1]->to_newick_string(), "(3,(2,1));");
	ASSERT_EQ(result[2]->to_newick_string(), "((3,2),1);");
}

TEST(GetAllBinaryTrees, with_four_leafs) {

	std::set<leaf_number> leafs = { "1", "2", "3", "4" };

	auto result = get_all_binary_trees(leafs);

	ASSERT_EQ(result.size(), 15);

	ASSERT_EQ(result[0]->to_newick_string(), "(((4,1),2),3);");
	ASSERT_EQ(result[1]->to_newick_string(), "((4,(2,1)),3);");
	ASSERT_EQ(result[2]->to_newick_string(), "(((4,2),1),3);");
	ASSERT_EQ(result[3]->to_newick_string(), "((4,2),(3,1));");
	ASSERT_EQ(result[4]->to_newick_string(), "(((4,2),3),1);");
	ASSERT_EQ(result[5]->to_newick_string(), "((4,1),(3,2));");
	ASSERT_EQ(result[6]->to_newick_string(), "(4,((3,1),2));");
	ASSERT_EQ(result[7]->to_newick_string(), "(4,(3,(2,1)));");
	ASSERT_EQ(result[8]->to_newick_string(), "(4,((3,2),1));");
	ASSERT_EQ(result[9]->to_newick_string(), "((4,(3,2)),1);");
	ASSERT_EQ(result[10]->to_newick_string(), "(((4,1),3),2);");
	ASSERT_EQ(result[11]->to_newick_string(), "((4,(3,1)),2);");
	ASSERT_EQ(result[12]->to_newick_string(), "(((4,3),1),2);");
	ASSERT_EQ(result[13]->to_newick_string(), "((4,3),(2,1));");
	ASSERT_EQ(result[14]->to_newick_string(), "(((4,3),2),1);");
}

TEST(PrintNewick, with_four_leafs) {

	auto l_1 = std::make_shared<Tree>("l1");
	auto l_2 = std::make_shared<Tree>("l2");
	auto l_3 = std::make_shared<Tree>("l3");
	auto l_4 = std::make_shared<Tree>("l4");
	auto c_1 = std::make_shared<Tree>(l_1, l_2, "c1");
	auto c_2 = std::make_shared<Tree>(l_3, l_4, "c2");
	auto r = std::make_shared<Tree>(c_1, c_2, "r");

	auto result = r->to_newick_string();

	ASSERT_EQ(result, "((l1,l2),(l3,l4));");
}

TEST(ApplyConstraintsTest, merges_to_two_parts) {

	std::set<leaf_number> leafs = { "1", "2", "3", "4" };

	std::vector<constraint> constraints;

	constraint cons1 = { "1", "1", "2", "3" };
	constraint cons2 = { "1", "1", "3", "4" };

	constraints.push_back(cons1);
	constraints.push_back(cons2);

	auto result = apply_constraints(leafs, constraints);

	ASSERT_EQ(result.size(), 2);
	ASSERT_THAT(*result[0], testing::ElementsAre("1", "2", "3"));
	ASSERT_THAT(*result[1], testing::ElementsAre("4"));
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

	//ASSERT_EQ(leafs.size(), 4);
//	ASSERT_EQ(*std::next(leafs.begin(), 0), 0);
//	ASSERT_EQ(*std::next(leafs.begin(), 1), 1);
//	ASSERT_EQ(*std::next(leafs.begin(), 2), 2);
//	ASSERT_EQ(*std::next(leafs.begin(), 3), 3);

	ASSERT_EQ(constraints.size(), 2);
	//lca(l1, l2) < lca(l1, l4)
	ASSERT_EQ(constraints[0].smaller_left, "l1");
	ASSERT_EQ(constraints[0].smaller_right, "l2");
	ASSERT_EQ(constraints[0].bigger_left, "l1");
	ASSERT_EQ(constraints[0].bigger_right, "l4");

	//lca(l3, l4) < lca(l1, l4)
	ASSERT_EQ(constraints[1].smaller_left, "l3");
	ASSERT_EQ(constraints[1].smaller_right, "l4");
	ASSERT_EQ(constraints[1].bigger_left, "l1");
	ASSERT_EQ(constraints[1].bigger_right, "l4");
}

TEST(GetNthPartitionTuple, example_with_two_parts) {

	std::set<leaf_number> part1 = { "1", "2" };
	std::set<leaf_number> part2 = { "3" };

	std::vector<std::shared_ptr<std::set<leaf_number> > > partitions;
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part1));
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part2));

	ASSERT_EQ(number_partition_tuples(partitions), 1);

	std::shared_ptr<std::set<leaf_number> > part_one;
	std::shared_ptr<std::set<leaf_number> > part_two;
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 1);
	ASSERT_THAT(*part_one, testing::ElementsAre("1", "2"));
	ASSERT_THAT(*part_two, testing::ElementsAre("3"));
}

TEST(GetNthPartitionTuple, example_from_slides) {

	std::set<leaf_number> part1 = { "1", "2" };
	std::set<leaf_number> part2 = { "3" };
	std::set<leaf_number> part3 = { "4", "5" };

	std::vector<std::shared_ptr<std::set<leaf_number> > > partitions;
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part1));
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part2));
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part3));

	ASSERT_EQ(number_partition_tuples(partitions), 3);

	std::shared_ptr<std::set<leaf_number> > part_one;
	std::shared_ptr<std::set<leaf_number> > part_two;
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 1);
	ASSERT_THAT(*part_one, testing::ElementsAre("1", "2"));
	ASSERT_THAT(*part_two, testing::ElementsAre("3", "4", "5"));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 2);
	ASSERT_THAT(*part_one, testing::ElementsAre("3"));
	ASSERT_THAT(*part_two, testing::ElementsAre("1", "2", "4", "5"));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 3);
	ASSERT_THAT(*part_one, testing::ElementsAre("1", "2", "3"));
	ASSERT_THAT(*part_two, testing::ElementsAre("4", "5"));
}

TEST(GetNthPartitionTuple, with_four_partitions) {

	std::set<leaf_number> part1 = { "1", "2" };
	std::set<leaf_number> part2 = { "3" };
	std::set<leaf_number> part3 = { "4", "5" };
	std::set<leaf_number> part4 = { "6", "7", "8" };

	std::vector<std::shared_ptr<std::set<leaf_number> > > partitions;
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part1));
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part2));
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part3));
	partitions.push_back(std::make_shared<std::set<leaf_number>>(part4));

	ASSERT_EQ(number_partition_tuples(partitions), 7);

	std::shared_ptr<std::set<leaf_number> > part_one;
	std::shared_ptr<std::set<leaf_number> > part_two;
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 1);
	ASSERT_THAT(*part_one, testing::ElementsAre("1", "2"));
	ASSERT_THAT(*part_two, testing::ElementsAre("3", "4", "5", "6", "7", "8"));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 2);
	ASSERT_THAT(*part_one, testing::ElementsAre("3"));
	ASSERT_THAT(*part_two,
			testing::ElementsAre("1", "2", "4", "5", "6", "7", "8"));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 3);
	ASSERT_THAT(*part_one, testing::ElementsAre("1", "2", "3"));
	ASSERT_THAT(*part_two, testing::ElementsAre("4", "5", "6", "7", "8"));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 4);
	ASSERT_THAT(*part_one, testing::ElementsAre("4", "5"));
	ASSERT_THAT(*part_two, testing::ElementsAre("1", "2", "3", "6", "7", "8"));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 5);
	ASSERT_THAT(*part_one, testing::ElementsAre("1", "2", "4", "5"));
	ASSERT_THAT(*part_two, testing::ElementsAre("3", "6", "7", "8"));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 6);
	ASSERT_THAT(*part_one, testing::ElementsAre("3", "4", "5"));
	ASSERT_THAT(*part_two, testing::ElementsAre("1", "2", "6", "7", "8"));
	std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 7);
	ASSERT_THAT(*part_one, testing::ElementsAre("1", "2", "3", "4", "5"));
	ASSERT_THAT(*part_two, testing::ElementsAre("6", "7", "8"));
}

TEST(ListTrees, example_from_slides) {

	std::set<leaf_number> leaves = { "1", "2", "3", "4", "5" };

	std::vector<constraint> constraints;

	constraint cons1 = { "1", "3", "2", "2" };
	constraint cons2 = { "4", "4", "5", "2" };

	constraints.push_back(cons1);
	constraints.push_back(cons2);

	auto n_trees = list_trees(constraints, leaves, nullptr);

	ASSERT_EQ(n_trees, 9);
}

TEST(ListTrees, example_from_slides_with_printing_stuff) {

	size_t buffer_size = 10000000;

	auto buffer = new char[buffer_size];
	std::string expected = "((2,1),((5,3),4));\n";
	expected += "((2,1),(5,(4,3)));\n";
	expected += "((2,1),((5,4),3));\n";
	expected += "(3,(1,(2,(5,4))));\n";
	expected += "(3,(2,((5,1),4)));\n";
	expected += "(3,(2,(5,(4,1))));\n";
	expected += "(3,(2,((5,4),1)));\n";
	expected += "(3,((2,1),(5,4)));\n";
	expected += "(((2,1),3),(5,4));\n";

	std::set<leaf_number> leaves = { "1", "2", "3", "4", "5" };

	std::vector<constraint> constraints;

	constraint cons1 = { "1", "3", "2", "2" };
	constraint cons2 = { "4", "4", "5", "2" };

	constraints.push_back(cons1);
	constraints.push_back(cons2);

	auto f = fmemopen(buffer, buffer_size, "w");
	ASSERT_TRUE(f != nullptr);
	list_trees(constraints, leaves, f);
	fclose(f);

	ASSERT_EQ(std::string(buffer), expected);

	delete[] buffer;
}

TEST(FindConstraintsTest, example_from_slides) {
	std::set<leaf_number> leaves = { "1", "2", "3" };

	std::vector<constraint> constraints;

	constraint cons1 = { "1", "3", "2", "2" };
	constraint cons2 = { "4", "4", "5", "2" };

	constraints.push_back(cons1);
	constraints.push_back(cons2);

	std::vector<constraint> result = find_constraints(leaves, constraints);

	ASSERT_EQ(result.size(), 1);
	ASSERT_EQ(result[0].smaller_left, "1");
	ASSERT_EQ(result[0].bigger_left, "3");
	ASSERT_EQ(result[0].smaller_right, "2");
	ASSERT_EQ(result[0].bigger_right, "2");
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
