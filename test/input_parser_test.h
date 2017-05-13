#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"

#include "../input_parser.h"
#include "../util.h"

#include <limits.h>
#include <iostream>
#include <memory>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"

// Test a simple tree file
TEST(GetNewickTreeTest, simple_tree) {
	ntree_t *tree = get_newk_tree("test/simple_tree.nwk");

	ASSERT_STREQ("A", tree->children[0]->label);
	ASSERT_STREQ("B", tree->children[1]->label);
	ASSERT_STREQ("C", tree->children[2]->children[0]->label);
    ASSERT_STREQ("D", tree->children[2]->children[1]->label);
	ASSERT_STREQ("A",
			tree->children[2]->children[1]->parent->parent->children[0]->label);
}

TEST(Root_at_Test, simple_tree) {
    ntree_t *tree = get_newk_tree("test/simple_tree.nwk");
    //char label[2];
    //label[0] = 'D';
    //label[1] = 0;
    //std::shared_ptr<Rtree> root = root_at(get_leaf_by_name(tree, label));
    //ASSERT_STREQ("D", root->left->label.c_str());
}

#pragma clang diagnostic pop
