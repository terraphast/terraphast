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
    assert(tree != nullptr);
    assert(check_tree(tree));
    ntree_t *ch0 = tree->children[0];
    ntree_t *ch1 = tree->children[1];
    ntree_t *ch2 = tree->children[2];
    ntree_t *ch2ch0 = tree->children[2]->children[0];
    ntree_t *ch2ch1 = tree->children[2]->children[1];
    ASSERT_EQ(tree->parent, nullptr);
	ASSERT_STREQ("A", tree->children[0]->label);
	ASSERT_STREQ("B", tree->children[1]->label);
	ASSERT_STREQ("C", tree->children[2]->children[0]->label);
    ASSERT_STREQ("D", tree->children[2]->children[1]->label);
	ASSERT_STREQ("A",
			tree->children[2]->children[1]->parent->parent->children[0]->label);

    ntree_destroy(tree);
}

TEST(get_leaf_by_name_Test, simiple_test) {
    ntree_t *tree = get_newk_tree("test/simple_tree.nwk");
    assert(check_tree(tree));
    ASSERT_EQ(tree->children[0], get_leaf_by_name(tree, "A"));
    ASSERT_EQ(tree->children[1], get_leaf_by_name(tree, "B"));
    ASSERT_EQ(tree->children[2]->children[0], get_leaf_by_name(tree, "C"));
    ASSERT_EQ(tree->children[2]->children[1], get_leaf_by_name(tree, "D"));

    ntree_destroy(tree);
}

TEST(Root_at_Test, simple_tree) {
    ntree_t *tree = get_newk_tree("test/simple_tree.nwk");
    assert(check_tree(tree));
    char label[2];
    label[0] = 'D';
    label[1] = 0;
    std::shared_ptr<Tree> root = root_at(get_leaf_by_name(tree, label));
    ASSERT_STREQ("D", root->left->label.c_str());
    ASSERT_STREQ("C", root->right->left->label.c_str());
    ASSERT_STREQ("B", root->right->right->right->label.c_str());
    ASSERT_STREQ("A", root->right->right->left->label.c_str());

    ntree_destroy(tree);
}

TEST(Tree_root_Test, simple_tree) {
    ntree_t *tree = get_newk_tree("test/simple_tree.nwk");

    const char *speciesNames[] = { "A", "B", "C", "D" };

    const unsigned char matrix1[] = { 1, 0, //1st row, species 0 has data for partition 0 only
            1, 0, 1, 1, 0, 1 }, //missing data matrix from example in the task specification

            matrix2[] = { 1, 1, //1st row, species 0 has data for partitions 0 and 1
                    1, 1, 1, 1, 1, 1}; //no missing data, so there should not be any terraces

    //let's initialize some missing data data structures now
    missingData *example1 = initializeMissingData(5, 2, speciesNames);
    missingData *example2 = initializeMissingData(5, 2, speciesNames);

    //now copy the missing data matrices to the data structure
    copyDataMatrix(matrix1, example1);
    copyDataMatrix(matrix2, example2);

    std::shared_ptr<Tree> root1 = root_tree(tree, example1);
    std::shared_ptr<Tree> root2 = root_tree(tree, example2);

    ntree_destroy(tree);
    freeMissingData(example1);
    freeMissingData(example2);
}

#pragma clang diagnostic pop
