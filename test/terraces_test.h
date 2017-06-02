#include <limits.h>

#include "terraces.h"
#include "util.h"
#include "gtest/gtest.h"

// Test a simple tree file
TEST(Util, generate_induced_tree) {
    ntree_t *tree = get_newk_tree_from_string("((s1,s2),s3,(s4,s5));");

    const char *speciesNames[] = {"s1", "s2", "s3", "s4", "s5"};

    const unsigned char matrix1[] = {1, 0,
                                     1, 0,
                                     1, 1,
                                     0, 1,
                                     0, 1};

    //let's initialize some missing data data structures now
    missingData *example1 = initializeMissingData(5, 2, speciesNames);
    copyDataMatrix(matrix1, example1);

    std::string root_species_name;
    std::shared_ptr<Tree> r_tree = root_tree(tree, example1, root_species_name);
    std::map<std::string, unsigned char> species_map;
    for (unsigned char i = 0; i < example1->numberOfSpecies; i++) {
        species_map[std::string(example1->speciesNames[i])] = i;
    }

    auto part0 = generate_induced_tree(r_tree, example1, species_map, 0);
    ASSERT_TRUE(part0 != nullptr);
    ASSERT_EQ("s1", part0->right->left->label);
    ASSERT_EQ("s2", part0->right->right->label);
    ASSERT_EQ("s3", part0->left->label);

    auto part1 = generate_induced_tree(r_tree, example1, species_map, 1);
    ASSERT_TRUE(part1 != nullptr);
    ASSERT_EQ("s3", part1->left->label);
    ASSERT_EQ("s4", part1->right->left->label);
    ASSERT_EQ("s5", part1->right->right->label);

    ntree_destroy(tree);
    freeMissingData(example1);
}

// Test a simple tree file
TEST(ExtractConstraintsFromSuperTree, example_from_slides) {
    ntree_t *tree = get_newk_tree_from_string("((s1,s2),s3,(s4,s5));");

    const char *speciesNames[] = {"s1", "s2", "s3", "s4", "s5"};

    const unsigned char matrix1[] = {1, 0,
                                     1, 0,
                                     1, 1,
                                     0, 1,
                                     0, 1};

    //let's initialize some missing data now
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

TEST(FindAllUnrootedTrees, example_from_slides) {
    ntree_t *tree = get_newk_tree_from_string("((s1,s2),s3,(s4,s5));");

    const char *speciesNames[] = {"s1", "s2", "s3", "s4", "s5"};

    const unsigned char matrix1[] = {1, 0,
                                     1, 0,
                                     1, 1,
                                     0, 1,
                                     0, 1};

    //let's initialize some missing data data structures now
    missingData *example1 = initializeMissingData(5, 2, speciesNames);
    copyDataMatrix(matrix1, example1);

    std::string root_species_name;
    std::shared_ptr<Tree> r_tree = root_tree(tree, example1, root_species_name);
    auto leafs = extract_leaf_labels_from_supertree(r_tree);
    auto constraints = extract_constraints_from_supertree(r_tree, example1);

    auto result = find_all_unrooted_trees(leafs, constraints, root_species_name);
    ASSERT_EQ(result.size(), 15);

    ASSERT_EQ(result[0]->to_newick_string(), "(s3,((s5,s1),s2),s4);");
    ASSERT_EQ(result[1]->to_newick_string(), "(s3,(s5,(s2,s1)),s4);");
    ASSERT_EQ(result[2]->to_newick_string(), "(s3,((s5,s2),s1),s4);");
    ASSERT_EQ(result[3]->to_newick_string(), "(s3,(s5,s2),(s4,s1));");
    ASSERT_EQ(result[4]->to_newick_string(), "(s3,((s5,s2),s4),s1);");
    ASSERT_EQ(result[5]->to_newick_string(), "(s3,(s5,s1),(s4,s2));");
    ASSERT_EQ(result[6]->to_newick_string(), "(s3,s5,((s4,s1),s2));");
    ASSERT_EQ(result[7]->to_newick_string(), "(s3,s5,(s4,(s2,s1)));");
    ASSERT_EQ(result[8]->to_newick_string(), "(s3,s5,((s4,s2),s1));");
    ASSERT_EQ(result[9]->to_newick_string(), "(s3,(s5,(s4,s2)),s1);");
    ASSERT_EQ(result[10]->to_newick_string(), "(s3,((s5,s1),s4),s2);");
    ASSERT_EQ(result[11]->to_newick_string(), "(s3,(s5,(s4,s1)),s2);");
    ASSERT_EQ(result[12]->to_newick_string(), "(s3,((s5,s4),s1),s2);");
    ASSERT_EQ(result[13]->to_newick_string(), "(s3,(s5,s4),(s2,s1));");
    ASSERT_EQ(result[14]->to_newick_string(), "(s3,((s5,s4),s2),s1);");

    ntree_destroy(tree);
    freeMissingData(example1);
}

TEST(TerracesAnalysis, example1_from_old_main) {
    //open some output files for enumerating all trees on a terrace
    FILE *f0 = fopen("tree1", "w");
    FILE *f1 = fopen("tree2", "w");

    //Original: use raw tree as input
    //char *newickString0 = "((s1,s2),s3,(s4,s5))"; //tree T_0 from Fig 2 in the task specification
    //char *newickString1 = "((s1,s2),s4,(s3,s5))";  //tree T_1 from Fig 2 in the task sepcification

    const char *newickString0 = "((s1,s2),s3,(s4,s5));"; //tree T_0 from Fig 2 in the task specification
    const char *newickString1 = "((s1,s2),s4,(s3,s5));"; //tree T_1 from Fig 2 in the task sepcification

    //names of species in newick tree that corrspond to rows in the data matrix, i.e., position 0 in array speciesNames corresponds to
    //row 0 in matrix missingDataMatrix, etc.
    const char *speciesNames[] = {"s1", "s2", "s3", "s4", "s5"};

    //we are storing missing data matrices as one-dimensonial arrays:
    //rows represent the missing data for each species
    //columns the data available per partition
    const unsigned char matrix1[] = {1, 0, //1st row, species 0 has data for partition 0 only
                                     1, 0,
                                     1, 1,
                                     0, 1,
                                     0, 1}, //missing data matrix from example in the task specification

            matrix2[] = {1, 1, //1st row, species 0 has data for partitions 0 and 1
                         1, 1, 1, 1, 1, 1, 1, 1}; //no missing data, so there should not be any terraces

    //let's initialize some missing data data structures now
    missingData *example1 = initializeMissingData(5, 2, speciesNames);
    missingData *example2 = initializeMissingData(5, 2, speciesNames);

    //now copy the missing data matrices to the data structure
    copyDataMatrix(matrix1, example1);
    copyDataMatrix(matrix2, example2);

    //variables to store the terrace size
    mpz_t terraceSize0;
    mpz_t terraceSize1;

    mpz_init(terraceSize0);
    mpz_init(terraceSize1);

    mpz_set_ui(terraceSize0, 0);
    mpz_set_ui(terraceSize1, 0);

    //error code, return value of the function you shall implement
    int errorCode;

    //now let's calculate terraces for example 1
    errorCode = terraceAnalysis(example1, newickString0,
                                TA_COUNT + TA_ENUMERATE, f0, &terraceSize0);
    if (errorCode == TERRACE_SUCCESS) {
        d_printf("Test 1\n");
    } else {
        d_printf("Error %i\n", errorCode);
    }

    errorCode = terraceAnalysis(example1,
                                newickString1,
                                TA_COUNT + TA_ENUMERATE,
                                f1,
                                &terraceSize1);
    //now let's calculate terraces for example 2
    if (errorCode == TERRACE_SUCCESS) {
        d_printf("Test 2\n");
    } else {
        d_printf("Error %i\n", errorCode);
    }

    //the terraces for example input trees 0 and 1 must be of the same size and equal to 15
    ASSERT_TRUE(
            (mpz_cmp(terraceSize0, terraceSize1) == 0) && (mpz_cmp_ui(terraceSize0, 15) == 0));

    /*********************************************************************************************************************/

    // example calling a matrix with no missing data, hence there are no terraces, or the size of all terraces is 1, or
    // there are 15 unique trees.
    // call for example tree 0
    errorCode = terraceAnalysis(example2, newickString0,
                                TA_COUNT + TA_ENUMERATE, f0, &terraceSize0);
    if (errorCode == TERRACE_SUCCESS) {
        d_printf("Test 3\n");
    } else {
        d_printf("Error %i\n", errorCode);
    }

    // call for example tree 1
    errorCode = terraceAnalysis(example2, newickString1,
                                TA_COUNT + TA_ENUMERATE, f1, &terraceSize1);
    if (errorCode == TERRACE_SUCCESS) {
        d_printf("Test 4\n");
    } else {
        d_printf("Error %i\n", errorCode);
    }

    //terrace size for both trees must be 1
    ASSERT_TRUE(
            (mpz_cmp(terraceSize0, terraceSize1) == 0) && (mpz_cmp_ui(terraceSize0, 1) == 0));

    freeMissingData(example1);
    freeMissingData(example2);
    fclose(f0);
    fclose(f1);
}

TEST(TerracesAnalysis, example2_from_old_main) {
    //now let's define a weird missing data matrix, there are 6 partitions and 6 species, each species
    //has data in exactly one distinct partition, what should your function return?
    // what should your function return if we change the data matrix to:
    //
    // weirdDataMatrx[] = {1,0,0,0,0,0,
    //                     0,1,0,0,0,0,
    //                     0,0,1,0,0,0,
    //                     0,0,0,1,0,0,
    //                     0,0,0,0,1,0,
    //                     0,0,0,0,0,1};
    //
    // ?

    //open an output files for enumerating all trees on a terrace
    FILE *f0 = fopen("tree1", "w");

    const char *weirdSpeciesNames[] = {"s1", "s2", "s3", "s4", "s5", "s6"};

    const unsigned char weirdDataMatrix[] = {1, 1, 1, 1, 1, 1,
                                             0, 1, 0, 0, 0, 0,
                                             0, 0, 1, 0, 0, 0,
                                             0, 0, 0, 1, 0, 0,
                                             0, 0, 0, 0, 1, 0,
                                             0, 0, 0, 0, 0, 1};

    const char *weirdTree = "((s1,s2),(s3,s4),(s5,s6));";

    mpz_t weirdTerraceSize;

    mpz_init(weirdTerraceSize);
    mpz_set_ui(weirdTerraceSize, 0);

    //initialize missing data data structure
    missingData *weirdExample = initializeMissingData(6, 6, weirdSpeciesNames);

    copyDataMatrix(weirdDataMatrix, weirdExample);
    int errorCode = terraceAnalysis(weirdExample, weirdTree,
                                    TA_COUNT + TA_ENUMERATE, f0, &weirdTerraceSize);
    if (errorCode == TERRACE_SUCCESS) {
        d_printf("Weird Test\n");
    } else {
        d_printf("Error %i\n", errorCode);
    }

    char *weirdTerraceSizeString = nullptr;

    weirdTerraceSizeString = mpz_get_str(weirdTerraceSizeString, 10,
                                         weirdTerraceSize);

    d_printf("weird terrace size: %s\n\n", weirdTerraceSizeString);

    free(weirdTerraceSizeString);

    freeMissingData(weirdExample);
    fclose(f0);
}

