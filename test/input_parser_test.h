
#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"
#include <limits.h>
#include "../input_parser.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"

// Test a simple tree file
//TODO figure out, why i get a compiler error
TEST(GetNewickTreeTest, simple_tree) {
  /*ntree_t *tree = get_newk_tree("simple_tree.nwk");

  EXPECT_EQ(tree->children[0]->label, "A");
  EXPECT_EQ(tree->children[1]->label, "B");
  EXPECT_EQ(tree->children[2]->children[0]->label, "C");
  EXPECT_EQ(tree->children[2]->children[0]->label, "D");
  EXPECT_EQ(tree->children[2]->children[1]->parent->parent->children[0]->label, "A");*/
}

#pragma clang diagnostic pop
