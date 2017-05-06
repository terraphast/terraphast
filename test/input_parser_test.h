
#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"

#include "../input_parser.h"

#include <limits.h>
#include <iostream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"

// Test a simple tree file
//TODO figure out, why i get a compiler error
TEST(GetNewickTreeTest, simple_tree) {
  ntree_t *tree = get_newk_tree("test/simple_tree.nwk");

  std::cout << tree->children[0]->label << std::endl;

  ASSERT_STREQ(tree->children[0]->label, "A");
  ASSERT_STREQ(tree->children[1]->label, "B");
  ASSERT_STREQ(tree->children[2]->children[0]->label, "C");
  ASSERT_STREQ(tree->children[2]->children[1]->label, "D");
  ASSERT_STREQ(tree->children[2]->children[1]->parent->parent->children[0]->label, "A");
}

#pragma clang diagnostic pop
