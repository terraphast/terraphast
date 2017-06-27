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
    assert(tree != nullptr);
    assert(check_tree(tree));
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
    char label[2];
    label[0] = 'D';
    label[1] = 0;
    std::vector<std::string> id_to_label;

    ASSERT_TRUE(check_tree(tree));
    std::shared_ptr<Tree> root = root_at(get_leaf_by_name(tree, label), id_to_label);
    ASSERT_EQ("C", id_to_label[root->left->id]);
    ASSERT_EQ("A", id_to_label[root->right->left->id]);
    ASSERT_EQ("B", id_to_label[root->right->right->id]);

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

    std::string root_species_name_1;
    std::vector<std::string> id_to_label_1;
    std::string root_species_name_2;
    std::vector<std::string> id_to_label_2;
    std::shared_ptr<Tree> root1 = root_tree(tree, example1, root_species_name_1, id_to_label_1);
    std::shared_ptr<Tree> root2 = root_tree(tree, example2, root_species_name_2, id_to_label_2);

    ntree_destroy(tree);
    freeMissingData(example1);
    freeMissingData(example2);
}
