
#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"

#include "../ifugao.cpp"

#include <limits.h>
#include <iostream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"

TEST(ApplyConstraintsTest, example_from_slides) {

  std::set<leaf_number> leaves = {1, 2, 3, 4, 5};

  std::vector<constraint> constraints;

  constraint cons1 = {1, 3, 2, 2};
  constraint cons2 = {4, 4, 5, 2};

  constraints.push_back(cons1);
  constraints.push_back(cons2);

  std::vector<std::shared_ptr<std::set<leaf_number> > > result = apply_constraints(leaves, constraints);
 
  ASSERT_EQ(result.size(), 3);

  int sum = 0;
  for(int i = 0; i < result.size(); i++){
	for(leaf_number n: *result[i]) {
	   sum += n;
	}
  }
  // all elements still contained in the sets?
  ASSERT_EQ(sum, 15);
}


TEST(FindConstraintsTest, example_from_slides) {
  std::set<leaf_number> leaves = {1, 2, 3};

  std::vector<constraint> constraints;

  constraint cons1 = {1, 3, 2, 2};
  constraint cons2 = {4, 4, 5, 2};

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
  binary_tree leaf_1 = {.is_leaf = true, .label = "leaf_1", .left_subtree = NULL, .right_subtree = NULL};
  binary_tree leaf_2 = {.is_leaf = true, .label = "leaf_2", .left_subtree = NULL, .right_subtree = NULL};

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
