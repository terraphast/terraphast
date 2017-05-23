#include <limits.h>

#include "input_parser_test.h"
#include "algorithm_test.h"
#include "terraces.h"
#include "util.h"
#include "gtest/gtest.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"

// Test a simple tree file
TEST(Util, generate_induced_tree) {
	ntree_t *tree = get_newk_tree("test/dummy_tree1.nwk");

	const char *speciesNames[] = { "s1", "s2", "s3", "s4", "s5" };

	const unsigned char matrix1[] = { 1, 0, 1, 0, 1, 1, 0, 1, 0, 1 };

	//let's initialize some missing data data structures now
	missingData *example1 = initializeMissingData(5, 2, speciesNames);
	copyDataMatrix(matrix1, example1);

    std::string root_species_name;
    std::shared_ptr<Tree> r_tree = root_tree(tree, example1, root_species_name);
	std::map<std::string, unsigned char> species_map;
	for (unsigned char i = 0; i < example1->numberOfSpecies; i++) {
		species_map[std::string(example1->speciesNames[i])] = i;
	}

	// d_print_tree(r_tree);
	auto part0 = generate_induced_tree(r_tree, example1, species_map, 0);
	// d_print_tree(part0);
	ASSERT_TRUE(part0 != nullptr);
	ASSERT_EQ("s1", part0->right->left->label);
	ASSERT_EQ("s2", part0->right->right->label);
	ASSERT_EQ("s3", part0->left->label);

	auto part1 = generate_induced_tree(r_tree, example1, species_map, 1);
	// d_print_tree(part1);
	ASSERT_TRUE(part1 != nullptr);
	ASSERT_EQ("s3", part1->left->label);
	ASSERT_EQ("s4", part1->right->left->label);
	ASSERT_EQ("s5", part1->right->right->label);

	ntree_destroy(tree);
	freeMissingData(example1);
}


// Test a simple tree file
TEST(ExtractConstraintsFromSuperTree, example_from_slides) {
	ntree_t *tree = get_newk_tree("test/dummy_tree1.nwk");

	const char *speciesNames[] = { "s1", "s2", "s3", "s4", "s5" };

	const unsigned char matrix1[] = { 1, 0, 1, 0, 1, 1, 0, 1, 0, 1 };

	//let's initialize some missing data data structures now
	missingData *example1 = initializeMissingData(5, 2, speciesNames);
	copyDataMatrix(matrix1, example1);

    std::string root_species_name;
    std::shared_ptr<Tree> r_tree = root_tree(tree, example1, root_species_name);
	auto constraints = extract_constraints_from_supertree(r_tree, example1);

	ASSERT_EQ(constraints.size(), 2);

	ASSERT_EQ(constraints[0].smaller_left, "s1");
	ASSERT_EQ(constraints[0].smaller_right, "s2");
	ASSERT_EQ(constraints[0].bigger_left, "s3");
	ASSERT_EQ(constraints[0].bigger_right, "s2");

	ASSERT_EQ(constraints[1].smaller_left, "s4");
	ASSERT_EQ(constraints[1].smaller_right, "s5");
	ASSERT_EQ(constraints[1].bigger_left, "s3");
	ASSERT_EQ(constraints[1].bigger_right, "s5");

	ntree_destroy(tree);
	freeMissingData(example1);
}

TEST(TerraceAnalysis, generate_induced_tree) {
	ntree_t *tree = get_newk_tree("test/dummy_tree2.nwk");

	const char *speciesNames[] = { "s1", "s2", "s3", "s4", "s5" };

	const unsigned char matrix1[] = { 1, 0, 1, 0, 1, 1, 0, 1, 0, 1 };

	//let's initialize some missing data data structures now
	missingData *example1 = initializeMissingData(5, 2, speciesNames);
	copyDataMatrix(matrix1, example1);

    std::string root_species_name;
    std::shared_ptr<Tree> r_tree = root_tree(tree, example1, root_species_name);
//	d_print_tree(r_tree);
	auto leafs = extract_leaf_labels_from_supertree(r_tree);
	auto constraints = extract_constraints_from_supertree(r_tree, example1);
	for (auto &c : constraints) {
		d_printf("lca(%s, %s) < lca(%s, %s)\n", c.smaller_left.c_str(),
				c.smaller_right.c_str(), c.bigger_left.c_str(),
				c.bigger_right.c_str());
	}

    auto result = combine_sets(leafs, constraints, root_species_name);
	d_printf("%s\n", r_tree->to_newick_string().c_str());
	d_printf("%lu\n", result.size());
	for (auto &r : result) {
		d_printf("%s\n", r->to_newick_string().c_str());
//		d_print_tree(r);
	}

	ntree_destroy(tree);
	freeMissingData(example1);
}

#pragma clang diagnostic pop

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
