#include "input_parser.h"
#include "util.h"

#include <limits.h>
#include <iostream>
#include <memory>
#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"
#include "gmock/gmock.h"

// Test a simple tree file
TEST(GetNewickTreeFromStringTest, simple_tree) {
    ntree_t *tree = get_newk_tree_from_string("(A,B,(C,D));");
    ASSERT_TRUE(tree != nullptr);
    ASSERT_TRUE(check_tree(tree));
    ASSERT_EQ(tree->parent, nullptr);
    ASSERT_STREQ("A", tree->children[0]->label);
    ASSERT_STREQ("B", tree->children[1]->label);
    ASSERT_STREQ("C", tree->children[2]->children[0]->label);
    ASSERT_STREQ("D", tree->children[2]->children[1]->label);
    ASSERT_STREQ("A",
            tree->children[2]->children[1]->parent->parent->children[0]->label);

    ntree_destroy(tree);
}

// Test a simple tree file
TEST(GetNewickTreeTest, simple_tree) {
    ntree_t *tree = get_newk_tree("../test/input/simple_tree.nwk");
    ASSERT_TRUE(tree != nullptr); // make sure file was read correctly
    ASSERT_TRUE(check_tree(tree));
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
    ntree_t *tree = get_newk_tree_from_string("(A,B,(C,D));");
    assert(check_tree(tree));
    ASSERT_EQ(tree->children[0], get_leaf_by_name(tree, "A"));
    ASSERT_EQ(tree->children[1], get_leaf_by_name(tree, "B"));
    ASSERT_EQ(tree->children[2]->children[0], get_leaf_by_name(tree, "C"));
    ASSERT_EQ(tree->children[2]->children[1], get_leaf_by_name(tree, "D"));

    ntree_destroy(tree);
}

TEST(Root_at_Test, simple_tree) {
    ntree_t *tree = get_newk_tree_from_string("(A,B,(C,D));");
    char label[] = {'D',0};
    label_mapper id_to_label;
    ASSERT_TRUE(check_tree(tree));
    Tree node = root_at(get_leaf_by_name(tree, label), id_to_label);
    ASSERT_FALSE(node->is_leaf());
    auto root = std::static_pointer_cast<InnerNode>(node);
    ASSERT_FALSE(root->left->is_leaf());
    auto left = std::static_pointer_cast<InnerNode>(root->left);
    ASSERT_TRUE(left->left->is_leaf());
    ASSERT_EQ("A", id_to_label[left->left->get_leaf()]);
    ASSERT_TRUE(left->right->is_leaf());
    ASSERT_EQ("B", id_to_label[left->right->get_leaf()]);
    ASSERT_TRUE(root->right->is_leaf());
    ASSERT_EQ("C", id_to_label[root->right->get_leaf()]);
    

    ntree_destroy(tree);
}

TEST(Tree_root_Test, simple_tree) {
    ntree_t *tree = get_newk_tree_from_string("(A,B,(C,D));");

    const char *speciesNames[] = { "A", "B", "C", "D" };

    const unsigned char matrix1[] = { 1, 0, //1st row, species 0 has data for partition 0 only
                                      1, 0,
                                      1, 1,
                                      0, 1 }; //missing data matrix from example in the task specification

    const unsigned char matrix2[] = { 1, 1, //1st row, species 0 has data for partitions 0 and 1
                                      1, 1,
                                      1, 1,
                                      1, 1 }; //no missing data, so there should not be any terraces

    //let's initialize some missing data data structures now
    missingData *example1 = initializeMissingData(5, 2, speciesNames);
    missingData *example2 = initializeMissingData(5, 2, speciesNames);

    //now copy the missing data matrices to the data structure
    copyDataMatrix(matrix1, example1);
    copyDataMatrix(matrix2, example2);

    label_mapper id_to_label_1;
    label_mapper id_to_label_2;
    Tree root1 = root_tree(tree, example1, id_to_label_1);
    Tree root2 = root_tree(tree, example2, id_to_label_2);

    //TODO asserts

    ntree_destroy(tree);
    freeMissingData(example1);
    freeMissingData(example2);
}

TEST(CheckCorruptDataFileError, number_of_species_to_low) {
  input_data *read_data = parse_input_data("../test/input/Allium_wrong_numbers1.data");
  ASSERT_TRUE(read_data == NULL);
}

TEST(CheckCorruptDataFileError, number_of_species_to_high) {
  input_data *read_data = parse_input_data("../test/input/Allium_wrong_numbers2.data");
  ASSERT_TRUE(read_data == NULL);
}
