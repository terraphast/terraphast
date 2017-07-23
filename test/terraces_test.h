#include <limits.h>
#include <fstream>
#include <future>

#include "gtest/gtest.h"

#include "terraces.h"
#include "util.h"
#include "parameters.h"
#include "debug.h"

#define TIME_FOR_TESTS 1000*30

typedef TerracesAnalysisTestParameter<const char*> TACountParameter;

class TACountFixture : public ::testing::TestWithParam<TACountParameter> {
};

class TACheckIfTerraceFixture : public ::testing::TestWithParam<TACountParameter> {
};

class TAEnumerateFixture : public ::testing::TestWithParam<TACountParameter> {
};

class TAEnumerateCompressedFixture : public ::testing::TestWithParam<TACountParameter> {
};

static void test_terrace_analysis(const char *newick_file,
                                  const char *data_file,
                                  const int ta_outspec,
                                  const char *trees_on_terrace) {
    input_data *read_data = parse_input_data(data_file);
    assert(read_data != nullptr);
    char *read_tree = read_newk_tree(newick_file);
    assert(read_tree != nullptr);

    missingData *m = initializeMissingData(read_data->number_of_species,
                                           read_data->number_of_partitions,
                                           const_cast<const char **>(read_data->names));
    copyDataMatrix(read_data->matrix, m);

    mpz_t terraceSize;

    mpz_init(terraceSize);
    mpz_set_ui(terraceSize, 0);

    int errorCode = terraceAnalysis(m, read_tree, ta_outspec, nullptr, &terraceSize);

    ASSERT_EQ(errorCode, TERRACE_SUCCESS);

    char *terraceSizeString = nullptr;

    terraceSizeString = mpz_get_str(terraceSizeString, 10, terraceSize);

    ASSERT_STREQ(terraceSizeString, trees_on_terrace);

    free(terraceSizeString);

    freeMissingData(m);
}

static void test_terrace_analysis_with_file(const char *newick_file,
                                  const char *data_file,
                                  const int ta_outspec,
                                  const char *trees_on_terrace) {
    input_data *read_data = parse_input_data(data_file);
    assert(read_data != nullptr);
    char *read_tree = read_newk_tree(newick_file);
    assert(read_tree != nullptr);

    missingData *m = initializeMissingData(read_data->number_of_species,
                                           read_data->number_of_partitions,
                                           const_cast<const char **>(read_data->names));
    copyDataMatrix(read_data->matrix, m);

    mpz_t terraceSize;

    mpz_init(terraceSize);
    mpz_set_ui(terraceSize, 0);

    FILE *file = fopen("test_output", "w");
    assert(file != nullptr);

    int errorCode = terraceAnalysis(m, read_tree, ta_outspec, file, &terraceSize);

    ASSERT_EQ(errorCode, TERRACE_SUCCESS);

    char *terraceSizeString = nullptr;

    terraceSizeString = mpz_get_str(terraceSizeString, 10, terraceSize);

    ASSERT_STREQ(terraceSizeString, trees_on_terrace);

    free(terraceSizeString);

    freeMissingData(m);
    fclose(file);
}

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

    label_mapper id_to_label;
    Tree r_tree = root_tree(tree, example1, id_to_label);
    std::map<std::string, leaf_number> species_map;
    for (leaf_number i = 0; i < example1->numberOfSpecies; i++) {
        species_map[std::string(example1->speciesNames[i])] = i;
    }

    auto part0 = generate_induced_tree(r_tree, example1, species_map, id_to_label, 0);
    ASSERT_TRUE(part0 != nullptr);
    ASSERT_EQ(part0->to_newick_string(id_to_label), std::string("(s1,s2);"));

    auto part1 = generate_induced_tree(r_tree, example1, species_map, id_to_label, 1);
    ASSERT_TRUE(part1 != nullptr);
    ASSERT_EQ(part1->to_newick_string(id_to_label), std::string("(s4,s5);"));

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

    label_mapper id_to_label;
    Tree r_tree = root_tree(tree, example1, id_to_label);
    auto constraints = extract_constraints_from_supertree(r_tree, example1, id_to_label);

    ASSERT_EQ(constraints.size(), 0);

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

    label_mapper id_to_label;
    Tree r_tree = root_tree(tree, example1, id_to_label);
    auto leaves = LeafSet(id_to_label.size());
    auto constraints = extract_constraints_from_supertree(r_tree, example1, id_to_label);

    FindAllRootedTrees get_trees;
    ASSERT_EQ(CountAllRootedTrees().scan_terrace(leaves, constraints), 15);
    auto result = get_trees.scan_terrace(leaves, constraints, true);
    ASSERT_EQ(result.size(), 15);
    
    ASSERT_EQ(result[0]->to_newick_string(id_to_label), "(s3,((s1,s2),s4),s5);");
    ASSERT_EQ(result[1]->to_newick_string(id_to_label), "(s3,(s2,(s1,s4)),s5);");
    ASSERT_EQ(result[2]->to_newick_string(id_to_label), "(s3,((s2,s4),s1),s5);");
    ASSERT_EQ(result[3]->to_newick_string(id_to_label), "(s3,(s2,s4),(s1,s5));");
    ASSERT_EQ(result[4]->to_newick_string(id_to_label), "(s3,((s2,s4),s5),s1);");
    ASSERT_EQ(result[5]->to_newick_string(id_to_label), "(s3,(s1,s4),(s2,s5));");
    ASSERT_EQ(result[6]->to_newick_string(id_to_label), "(s3,s4,((s1,s2),s5));");
    ASSERT_EQ(result[7]->to_newick_string(id_to_label), "(s3,s4,(s2,(s1,s5)));");
    ASSERT_EQ(result[8]->to_newick_string(id_to_label), "(s3,s4,((s2,s5),s1));");
    ASSERT_EQ(result[9]->to_newick_string(id_to_label), "(s3,(s4,(s2,s5)),s1);");
    ASSERT_EQ(result[10]->to_newick_string(id_to_label), "(s3,((s1,s4),s5),s2);");
    ASSERT_EQ(result[11]->to_newick_string(id_to_label), "(s3,(s4,(s1,s5)),s2);");
    ASSERT_EQ(result[12]->to_newick_string(id_to_label), "(s3,((s4,s5),s1),s2);");
    ASSERT_EQ(result[13]->to_newick_string(id_to_label), "(s3,(s4,s5),(s1,s2));");
    ASSERT_EQ(result[14]->to_newick_string(id_to_label), "(s3,((s4,s5),s2),s1);");
    
    ntree_destroy(tree);
    freeMissingData(example1);
}

TEST(FindCompressedUnrootedTree, example_from_slides) {
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

    label_mapper id_to_label;
    Tree r_tree = root_tree(tree, example1, id_to_label);
    auto leaves = LeafSet(id_to_label.size());
    auto constraints = extract_constraints_from_supertree(r_tree, example1, id_to_label);

    FindCompressedTree get_trees;
    auto result = get_trees.scan_terrace(leaves, constraints, true);

    ASSERT_EQ(result->to_newick_string(id_to_label), "(s3,{s1,s2,s4,s5});");

    ntree_destroy(tree);
    freeMissingData(example1);
}

TEST(TerracesAnalysis, example1_from_old_main) {
    //open some output files for enumerating all trees on a terrace
    FILE *f0 = fopen("tree1", "w");
    FILE *f1 = fopen("tree2", "w");

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
                                     0, 1}; //missing data matrix from example in the task specification

    const unsigned char matrix2[] = {1, 1, //1st row, species 0 has data for partitions 0 and 1
                                     1, 1,
                                     1, 1,
                                     1, 1,
                                     1, 1}; //no missing data, so there should not be any terraces

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
                                TA_COUNT + TA_ENUMERATE,
                                f0,
                                &terraceSize0);
    ASSERT_EQ(errorCode, TERRACE_SUCCESS);

    //now let's calculate terraces for example 2
    errorCode = terraceAnalysis(example1,
                                newickString1,
                                TA_COUNT + TA_ENUMERATE,
                                f1,
                                &terraceSize1);
    ASSERT_EQ(errorCode, TERRACE_SUCCESS);

    //the terraces for example input trees 0 and 1 must be of the same size and equal to 15
    ASSERT_TRUE(
            (mpz_cmp(terraceSize0, terraceSize1) == 0) && (mpz_cmp_ui(terraceSize0, 15) == 0));

    /******************************************************************************************************************/

    // example calling a matrix with no missing data, hence there are no terraces, or the size of all terraces is 1, or
    // there are 15 unique trees.
    // call for example tree 0
    errorCode = terraceAnalysis(example2, newickString0,
                                TA_COUNT + TA_ENUMERATE, f0, &terraceSize0);
    ASSERT_EQ(errorCode, TERRACE_SUCCESS);

    // call for example tree 1
    errorCode = terraceAnalysis(example2, newickString1,
                                TA_COUNT + TA_ENUMERATE, f1, &terraceSize1);
    ASSERT_EQ(errorCode, TERRACE_SUCCESS);

    //terrace size for both trees must be 1
    ASSERT_TRUE(
            (mpz_cmp(terraceSize0, terraceSize1) == 0) && (mpz_cmp_ui(terraceSize0, 1) == 0));

    freeMissingData(example1);
    freeMissingData(example2);
    fclose(f0);
    fclose(f1);
}

//now let's define a weird missing data matrix, there are 6 partitions and 6 species, each species
//has data in exactly one distinct partition, what should your function return?
// what should your function return if we change the data matrix to:
TEST(TerracesAnalysis, examle_with_no_root_species) {
    //open an output files for enumerating all trees on a terrace
    FILE *f0 = fopen("tree1", "w");

    const char *weirdSpeciesNames[] = {"s1", "s2", "s3", "s4", "s5", "s6"};

    const unsigned char weirdDataMatrix[] = {1, 0, 0, 0, 0, 0,
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
    //TODO: Should not be an assert! return a corresponding error code instead
    ASSERT_EQ(terraceAnalysis(weirdExample, weirdTree, TA_COUNT + TA_ENUMERATE, f0, &weirdTerraceSize),
                 TERRACE_NO_ROOT_SPECIES_ERROR);

    freeMissingData(weirdExample);
    fclose(f0);
}

TEST(TerracesAnalysis, example2_from_old_main) {

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
    ASSERT_EQ(errorCode, TERRACE_SUCCESS);

    char *weirdTerraceSizeString = nullptr;

    weirdTerraceSizeString = mpz_get_str(weirdTerraceSizeString, 10,
                                         weirdTerraceSize);

    //input data of the 6 taxa yields to no constraint
    // -> 105 possibilities exist to build a 6 taxa
    ASSERT_STREQ(weirdTerraceSizeString, "105");

    free(weirdTerraceSizeString);

    freeMissingData(weirdExample);
    fclose(f0);
}

TEST_P(TACountFixture, ExamplesFromModifiedInput) {
    auto param = GetParam();

    std::promise<bool> promisedFinished;
    auto futureResult = promisedFinished.get_future();
    std::thread([param](std::promise<bool> &finished) {
        test_terrace_analysis(param.newick_file, param.data_file, TA_COUNT, param.expected_value);
        finished.set_value(true);
    }, std::ref(promisedFinished)).detach();
    EXPECT_TRUE(futureResult.wait_for(std::chrono::milliseconds(TIME_FOR_TESTS))
                != std::future_status::timeout);
}

TEST_P(TACheckIfTerraceFixture, ExamplesFromModifiedInput) {
    auto param = GetParam();

    std::promise<bool> promisedFinished;
    auto futureResult = promisedFinished.get_future();
    std::thread([param](std::promise<bool> &finished) {
        test_terrace_analysis(param.newick_file, param.data_file, TA_DETECT, param.expected_value);
        finished.set_value(true);
    }, std::ref(promisedFinished)).detach();
    EXPECT_TRUE(futureResult.wait_for(std::chrono::milliseconds(TIME_FOR_TESTS))
                != std::future_status::timeout);
}

TEST_P(TAEnumerateFixture, ExamplesFromModifiedInput) {
    auto param = GetParam();

    std::promise<bool> promisedFinished;
    auto futureResult = promisedFinished.get_future();
    std::thread([param](std::promise<bool> &finished) {
        test_terrace_analysis_with_file(param.newick_file, param.data_file, TA_ENUMERATE, param.expected_value);
        finished.set_value(true);
    }, std::ref(promisedFinished)).detach();
    EXPECT_TRUE(futureResult.wait_for(std::chrono::milliseconds(TIME_FOR_TESTS))
                != std::future_status::timeout);
}

TEST_P(TAEnumerateCompressedFixture, ExamplesFromModifiedInput) {
    auto param = GetParam();

    std::promise<bool> promisedFinished;
    auto futureResult = promisedFinished.get_future();
    std::thread([param](std::promise<bool> &finished) {
        test_terrace_analysis_with_file(param.newick_file, param.data_file, TA_ENUMERATE_COMPRESS, param.expected_value);
        finished.set_value(true);
    }, std::ref(promisedFinished)).detach();
    EXPECT_TRUE(futureResult.wait_for(std::chrono::milliseconds(TIME_FOR_TESTS))
                != std::future_status::timeout);
}

INSTANTIATE_TEST_CASE_P(ModifiedDataInstance, TACountFixture, ::testing::Values(
    TACountParameter("../input/modified/Allium.nwk",
                     "../input/modified/Allium.data",
                     "8038035"),
    TACountParameter("../input/modified/Allium_Reduced.nwk",
                     "../input/modified/Allium_Reduced.data",
                     "730680125"),
    TACountParameter("../input/modified/Allium_Tiny.nwk",
                     "../input/modified/Allium_Tiny.data",
                     "35"),
    TACountParameter("../input/modified/Asplenium.nwk.1",
                     "../input/modified/Asplenium.data.1",
                     "1"),
    TACountParameter("../input/modified/Asplenium.nwk.2",
                     "../input/modified/Asplenium.data.2",
                     "95"),
    TACountParameter("../input/modified/Bouchenak.nwk",
                     "../input/modified/Bouchenak.data",
                     "61261515"),
    TACountParameter("../input/modified/Caryophyllaceae.nwk",
                     "../input/modified/Caryophyllaceae.data",
                     "718346120625"),
    TACountParameter("../input/modified/Eucalyptus.nwk.1",
                     "../input/modified/Eucalyptus.data.1",
                     "229"),
    TACountParameter("../input/modified/Eucalyptus.nwk.1",
                     "../input/modified/Eucalyptus.data.2",
                     "267"),
    TACountParameter("../input/modified/Eucalyptus.nwk.3",
                     "../input/modified/Eucalyptus.data.3",
                     "9"),
    TACountParameter("../input/modified/Euphorbia.nwk.1",
                     "../input/modified/Euphorbia.data.1",
                     "759"),
    TACountParameter("../input/modified/Euphorbia.nwk.2",
                     "../input/modified/Euphorbia.data.2",
                     "759"),
    TACountParameter("../input/modified/Ficus.nwk.1",
                     "../input/modified/Ficus.data.1",
                     "283815"),
    TACountParameter("../input/modified/Ficus.nwk.2",
                     "../input/modified/Ficus.data.2",
                     "851445"),
    TACountParameter("../input/modified/Ficus.nwk.3",
                     "../input/modified/Ficus.data.3",
                     "851445"),
    TACountParameter("../input/modified/Iris.nwk",
                     "../input/modified/Iris.data",
                     "1"),
    TACountParameter("../input/modified/Meusemann.nwk",
                     "../input/modified/Meusemann.data",
                     "1"),
    TACountParameter("../input/modified/Pyron.nwk",
                     "../input/modified/Pyron.data",
                     "2205")
));

INSTANTIATE_TEST_CASE_P(ModifiedDataInstance, TAEnumerateFixture, ::testing::Values(
    TACountParameter("../input/modified/Allium_Tiny.nwk",
                     "../input/modified/Allium_Tiny.data",
                     "35"),
    TACountParameter("../input/modified/Asplenium.nwk.1",
                     "../input/modified/Asplenium.data.1",
                     "1"),
    TACountParameter("../input/modified/Asplenium.nwk.2",
                     "../input/modified/Asplenium.data.2",
                     "95"),
    TACountParameter("../input/modified/Eucalyptus.nwk.1",
                     "../input/modified/Eucalyptus.data.1",
                     "229"),
    TACountParameter("../input/modified/Eucalyptus.nwk.1",
                     "../input/modified/Eucalyptus.data.2",
                     "267"),
    TACountParameter("../input/modified/Eucalyptus.nwk.3",
                     "../input/modified/Eucalyptus.data.3",
                     "9"),
    TACountParameter("../input/modified/Euphorbia.nwk.1",
                     "../input/modified/Euphorbia.data.1",
                     "759"),
    TACountParameter("../input/modified/Euphorbia.nwk.2",
                     "../input/modified/Euphorbia.data.2",
                     "759"),
    TACountParameter("../input/modified/Iris.nwk",
                     "../input/modified/Iris.data",
                     "1"),
    TACountParameter("../input/modified/Meusemann.nwk",
                     "../input/modified/Meusemann.data",
                     "1"),
    TACountParameter("../input/modified/Pyron.nwk",
                     "../input/modified/Pyron.data",
                     "2205")
));

INSTANTIATE_TEST_CASE_P(ModifiedDataInstance, TAEnumerateCompressedFixture, ::testing::Values(
    TACountParameter("../input/modified/Allium_Tiny.nwk",
                     "../input/modified/Allium_Tiny.data",
                     "35"),
    TACountParameter("../input/modified/Asplenium.nwk.1",
                     "../input/modified/Asplenium.data.1",
                     "1"),
    TACountParameter("../input/modified/Asplenium.nwk.2",
                     "../input/modified/Asplenium.data.2",
                     "95"),
    TACountParameter("../input/modified/Eucalyptus.nwk.1",
                     "../input/modified/Eucalyptus.data.1",
                     "229"),
    TACountParameter("../input/modified/Eucalyptus.nwk.1",
                     "../input/modified/Eucalyptus.data.2",
                     "267"),
    TACountParameter("../input/modified/Eucalyptus.nwk.3",
                     "../input/modified/Eucalyptus.data.3",
                     "9"),
    TACountParameter("../input/modified/Euphorbia.nwk.1",
                     "../input/modified/Euphorbia.data.1",
                     "759"),
    TACountParameter("../input/modified/Euphorbia.nwk.2",
                     "../input/modified/Euphorbia.data.2",
                     "759"),
    TACountParameter("../input/modified/Iris.nwk",
                     "../input/modified/Iris.data",
                     "1"),
    TACountParameter("../input/modified/Meusemann.nwk",
                     "../input/modified/Meusemann.data",
                     "1"),
    TACountParameter("../input/modified/Pyron.nwk",
                     "../input/modified/Pyron.data",
                     "2205")
));

INSTANTIATE_TEST_CASE_P(ModifiedDataInstance, TACheckIfTerraceFixture, ::testing::Values(
    TACountParameter("../input/modified/Allium.nwk",
                     "../input/modified/Allium.data",
                     "2"),
    TACountParameter("../input/modified/Allium_Reduced.nwk",
                     "../input/modified/Allium_Reduced.data",
                     "2"),
    TACountParameter("../input/modified/Allium_Tiny.nwk",
                     "../input/modified/Allium_Tiny.data",
                     "2"),
    TACountParameter("../input/modified/Asplenium.nwk.1",
                     "../input/modified/Asplenium.data.1",
                     "0"),
    TACountParameter("../input/modified/Asplenium.nwk.2",
                     "../input/modified/Asplenium.data.2",
                     "2"),
    TACountParameter("../input/modified/Bouchenak.nwk",
                     "../input/modified/Bouchenak.data",
                     "2"),
    TACountParameter("../input/modified/Caryophyllaceae.nwk",
                     "../input/modified/Caryophyllaceae.data",
                     "2"),
    TACountParameter("../input/modified/Eucalyptus.nwk.1",
                     "../input/modified/Eucalyptus.data.1",
                     "2"),
    TACountParameter("../input/modified/Eucalyptus.nwk.1",
                     "../input/modified/Eucalyptus.data.2",
                     "2"),
    TACountParameter("../input/modified/Eucalyptus.nwk.3",
                     "../input/modified/Eucalyptus.data.3",
                     "2"),
    TACountParameter("../input/modified/Euphorbia.nwk.1",
                     "../input/modified/Euphorbia.data.1",
                     "2"),
    TACountParameter("../input/modified/Euphorbia.nwk.2",
                     "../input/modified/Euphorbia.data.2",
                     "2"),
    TACountParameter("../input/modified/Ficus.nwk.1",
                     "../input/modified/Ficus.data.1",
                     "2"),
    TACountParameter("../input/modified/Ficus.nwk.2",
                     "../input/modified/Ficus.data.2",
                     "2"),
    TACountParameter("../input/modified/Ficus.nwk.3",
                     "../input/modified/Ficus.data.3",
                     "2"),
    TACountParameter("../input/modified/Iris.nwk",
                     "../input/modified/Iris.data",
                     "0"),
    TACountParameter("../input/modified/Meusemann.nwk",
                     "../input/modified/Meusemann.data",
                     "0"),
    TACountParameter("../input/modified/Pyron.nwk",
                     "../input/modified/Pyron.data",
                     "2")
));

TEST(CheckNewickError, examle_with_corrupt_newick_string_1) {
    //open an output files for enumerating all trees on a terrace
    FILE *f0 = fopen("tree1", "w");

    const char *speciesNames[] = {"s1", "s2", "s3", "s4", "s5", "s6"};

    const unsigned char dataMatrix[] = {1, 1, 1, 1, 1, 1,
                                        0, 1, 0, 0, 0, 0,
                                        0, 0, 1, 0, 0, 0,
                                        0, 0, 0, 1, 0, 0,
                                        0, 0, 0, 0, 1, 0,
                                        0, 0, 0, 0, 0, 1};

    const char *corruptTree = "((s1,s2),(s3,s4),(s5,s6);";

    mpz_t terraceSize;

    mpz_init(terraceSize);
    mpz_set_ui(terraceSize, 0);

    //initialize missing data data structure
    missingData *example = initializeMissingData(6, 6, speciesNames);

    copyDataMatrix(dataMatrix,example);

    int errorCode;
    errorCode = terraceAnalysis(example, corruptTree, TA_COUNT + TA_ENUMERATE, f0, &terraceSize);
    ASSERT_EQ(errorCode, TERRACE_NEWICK_ERROR);

    freeMissingData(example);
    fclose(f0);
}

TEST(CheckNewickError, examle_with_corrupt_newick_string_2) {
    //open an output files for enumerating all trees on a terrace
    FILE *f0 = fopen("tree1", "w");

    const char *speciesNames[] = {"s1", "s2", "s3", "s4", "s5", "s6"};

    const unsigned char dataMatrix[] = {1, 1, 1, 1, 1, 1,
                                        0, 1, 0, 0, 0, 0,
                                        0, 0, 1, 0, 0, 0,
                                        0, 0, 0, 1, 0, 0,
                                        0, 0, 0, 0, 1, 0,
                                        0, 0, 0, 0, 0, 1};

    const char *corruptTree = "((s1,s2)(s3,s4),(s5,s6));";

    mpz_t terraceSize;

    mpz_init(terraceSize);
    mpz_set_ui(terraceSize, 0);

    //initialize missing data data structure
    missingData *example = initializeMissingData(6, 6, speciesNames);

    copyDataMatrix(dataMatrix,example);

    int errorCode;
    errorCode = terraceAnalysis(example, corruptTree, TA_COUNT + TA_ENUMERATE, f0, &terraceSize);
    ASSERT_EQ(errorCode, TERRACE_NEWICK_ERROR);

    freeMissingData(example);
    fclose(f0);
}

TEST(CheckNewickError, examle_with_corrupt_newick_string_3) {
    //open an output files for enumerating all trees on a terrace
    FILE *f0 = fopen("tree1", "w");

    const char *speciesNames[] = {"s1", "s2", "s3", "s4", "s5", "s6"};

    const unsigned char dataMatrix[] = {1, 1, 1, 1, 1, 1,
                                        0, 1, 0, 0, 0, 0,
                                        0, 0, 1, 0, 0, 0,
                                        0, 0, 0, 1, 0, 0,
                                        0, 0, 0, 0, 1, 0,
                                        0, 0, 0, 0, 0, 1};

    const char *corruptTree = "((s1,s2),(s3,s4),(s5,s6))";

    mpz_t terraceSize;

    mpz_init(terraceSize);
    mpz_set_ui(terraceSize, 0);

    //initialize missing data data structure
    missingData *example = initializeMissingData(6, 6, speciesNames);

    copyDataMatrix(dataMatrix,example);

    int errorCode;
    errorCode = terraceAnalysis(example, corruptTree, TA_COUNT + TA_ENUMERATE, f0, &terraceSize);
    ASSERT_EQ(errorCode, TERRACE_NEWICK_ERROR);

    freeMissingData(example);
    fclose(f0);
}

TEST(CheckNewickError, examle_with_corrupt_newick_string_4) {
    //open an output files for enumerating all trees on a terrace
    FILE *f0 = fopen("tree1", "w");

    const char *speciesNames[] = {"s1", "s2", "s3", "s4", "s5", "s6", "s55"};

    const unsigned char dataMatrix[] = {1, 1, 1, 1, 1, 1, 1,
                                        0, 1, 0, 0, 0, 0, 1,
                                        0, 0, 1, 0, 0, 0, 1,
                                        0, 0, 0, 1, 0, 0, 1,
                                        0, 0, 0, 0, 1, 0, 1,
                                        0, 0, 0, 0, 0, 1, 1,
                                        0, 0, 0, 1, 0, 0, 1};

    const char *corruptTree = "((s1,s2,s55),(s3,s4),(s5,s6))";

    mpz_t terraceSize;

    mpz_init(terraceSize);
    mpz_set_ui(terraceSize, 0);

    //initialize missing data data structure
    missingData *example = initializeMissingData(7, 7, speciesNames);

    copyDataMatrix(dataMatrix,example);

    int errorCode;
    errorCode = terraceAnalysis(example, corruptTree, TA_COUNT + TA_ENUMERATE, f0, &terraceSize);
    ASSERT_EQ(errorCode, TERRACE_NEWICK_ERROR);

    freeMissingData(example);
    fclose(f0);
}

TEST(CheckNewickError, examle_with_corrupt_newick_string_5) {
    //open an output files for enumerating all trees on a terrace
    FILE *f0 = fopen("tree1", "w");

    const char *speciesNames[] = {"s1", "s2", "s3", "s4", "s5", "s6"};

    const unsigned char dataMatrix[] = {1, 1, 1, 1, 1, 1,
                                        0, 1, 0, 0, 0, 0,
                                        0, 0, 1, 0, 0, 0,
                                        0, 0, 0, 1, 0, 0,
                                        0, 0, 0, 0, 1, 0,
                                        0, 0, 0, 0, 0, 1};

    const char *corruptTree = "((s1,s2),(,s4),(s5,s6));";

    mpz_t terraceSize;

    mpz_init(terraceSize);
    mpz_set_ui(terraceSize, 0);

    //initialize missing data data structure
    missingData *example = initializeMissingData(6, 6, speciesNames);

    copyDataMatrix(dataMatrix,example);

    int errorCode;
    errorCode = terraceAnalysis(example, corruptTree, TA_COUNT + TA_ENUMERATE, f0, &terraceSize);
    ASSERT_EQ(errorCode, TERRACE_NEWICK_ERROR);

    freeMissingData(example);
    fclose(f0);
}

TEST(CheckTreeNotBinaryError, examle_with_not_binary_newick_string_1) {
    //open an output files for enumerating all trees on a terrace
    FILE *f0 = fopen("tree1", "w");

    const char *speciesNames[] = {"s1", "s2", "s3", "s4"};

    const unsigned char dataMatrix[] = {1, 1, 1, 1,
                                        0, 1, 0, 1,
                                        0, 0, 1, 1,
                                        0, 1, 0, 0};

    const char *corruptTree = "(s1, s2, s3, s4);";

    mpz_t terraceSize;

    mpz_init(terraceSize);
    mpz_set_ui(terraceSize, 0);

    //initialize missing data data structure
    missingData *example = initializeMissingData(4, 4, speciesNames);

    copyDataMatrix(dataMatrix,example);

    int errorCode;
    errorCode = terraceAnalysis(example, corruptTree, TA_COUNT + TA_ENUMERATE, f0, &terraceSize);
    ASSERT_EQ(errorCode, TERRACE_TREE_NOT_BINARY_ERROR);

    freeMissingData(example);
    fclose(f0);
}

TEST(CheckTreeNotBinaryError, examle_with_not_binary_newick_string_2) {
  //open an output files for enumerating all trees on a terrace
  FILE *f0 = fopen("tree1", "w");

  const char *speciesNames[] = {"s1", "s2", "s3", "s4", "s5", "s6"};

  const unsigned char dataMatrix[] = {1, 1, 1, 1, 1, 1,
                                      0, 1, 0, 0, 0, 0,
                                      0, 0, 1, 0, 0, 0,
                                      0, 0, 0, 1, 0, 0,
                                      0, 0, 0, 0, 1, 0,
                                      0, 0, 0, 0, 0, 1};

  const char *corruptTree = "((s1,s2,s3),s4,(s5,s6));";

  mpz_t terraceSize;

  mpz_init(terraceSize);
  mpz_set_ui(terraceSize, 0);

  //initialize missing data data structure
  missingData *example = initializeMissingData(6, 6, speciesNames);

  copyDataMatrix(dataMatrix,example);

  int errorCode;
  errorCode = terraceAnalysis(example, corruptTree, TA_COUNT + TA_ENUMERATE, f0, &terraceSize);
  ASSERT_EQ(errorCode, TERRACE_TREE_NOT_BINARY_ERROR);

  freeMissingData(example);
  fclose(f0);
}
