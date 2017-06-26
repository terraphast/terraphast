#include "gmp.h"
#include <limits.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <future>

#include "terraces.h"
#include "parameters.h"
#include "debug.h"

typedef TerracesAnalysisTestParameter<const long> RootedTreesCountParameter;

class RootedTreesAnalysis : public ::testing::TestWithParam<RootedTreesCountParameter> {
};

static void test_rooted_trees(const char* newick_file, const char* data_file, long trees_on_terrace) {
    
    input_data *read_data = parse_input_data(data_file);
    assert(read_data != nullptr);
    ntree_t *tree = get_newk_tree(newick_file);
    assert(tree != nullptr);

    missingData *m = initializeMissingData(read_data->number_of_species,
                                           read_data->number_of_partitions,
                                           const_cast<const char **>(read_data->names));
    copyDataMatrix(read_data->matrix, m);

    std::string root_species_name;
    std::shared_ptr<Tree> rtree = root_tree(tree, m, root_species_name);

    std::set<leaf_label> leafs;
    for (size_t k = 0; k < m->numberOfSpecies; k++) {
        for (size_t j = 0; j < m->numberOfPartitions; j++) {
            if (getDataMatrix(m, k, j) == static_cast<unsigned char>(1)) {
                leafs.insert(leaf_label(m->speciesNames[k]));
                break;
            }
        }
    }

    auto result = count_all_rooted_trees(leafs, extract_constraints_from_supertree(rtree, m));

    ASSERT_EQ(result, trees_on_terrace);

    ntree_destroy(tree);
    freeMissingData(m);
    free_input_data(read_data);
}

TEST(ApplyConstraintsTest, example_from_slides) {

    std::set<leaf_label> leaves = {"1", "2", "3", "4", "5"};

    std::vector<constraint> constraints;

    constraint cons1 = {"1", "3", "2", "2"};
    constraint cons2 = {"4", "4", "5", "2"};

    constraints.push_back(cons1);
    constraints.push_back(cons2);

    std::vector<std::shared_ptr<std::set<leaf_label> > > result =
            apply_constraints(leaves, constraints);

    ASSERT_EQ(result.size(), 3);

    ASSERT_THAT(*result[0], testing::ElementsAre("1", "2"));
    ASSERT_THAT(*result[1], testing::ElementsAre("3"));
    ASSERT_THAT(*result[2], testing::ElementsAre("4", "5"));
}

TEST(ApplyConstraintsTest, merge_all_sets) {

    std::set<leaf_label> leaves = {"1", "2", "3", "4", "5"};

    std::vector<constraint> constraints;

    constraint cons1 = {"1", "3", "2", "2"};
    constraint cons2 = {"4", "4", "5", "2"};
    constraint cons3 = {"3", "3", "4", "2"};
    constraint cons4 = {"1", "1", "3", "5"};

    constraints.push_back(cons1);
    constraints.push_back(cons2);
    constraints.push_back(cons3);
    constraints.push_back(cons4);

    std::vector<std::shared_ptr<std::set<leaf_label> > > result =
            apply_constraints(leaves, constraints);

    ASSERT_EQ(result.size(), 1);

    ASSERT_THAT(*result[0], testing::ElementsAre("1", "2", "3", "4", "5"));

}

TEST(ApplyConstraintsTest, no_merges) {

    std::set<leaf_label> leaves = {"1", "2", "3", "4", "5"};

    std::vector<constraint> constraints;

    constraint cons1 = {"1", "3", "6", "6"};

    constraints.push_back(cons1);

    ASSERT_DEATH(apply_constraints(leaves, constraints),
                 "Assertion (.)* failed.");
}

TEST(GetAllBinaryTrees, with_tree_leafs) {

    std::set<leaf_label> leafs = {"1", "2", "3"};

    auto result = get_all_binary_trees(leafs);

    ASSERT_EQ(result.size(), 3);

    ASSERT_EQ(result[0]->to_newick_string(), "((3,1),2);");
    ASSERT_EQ(result[1]->to_newick_string(), "(3,(2,1));");
    ASSERT_EQ(result[2]->to_newick_string(), "((3,2),1);");
}

TEST(GetAllBinaryTrees, with_four_leafs) {

    std::set<leaf_label> leafs = {"1", "2", "3", "4"};

    auto result = get_all_binary_trees(leafs);

    ASSERT_EQ(result.size(), 15);

    ASSERT_EQ(result[0]->to_newick_string(), "(((4,1),2),3);");
    ASSERT_EQ(result[1]->to_newick_string(), "((4,(2,1)),3);");
    ASSERT_EQ(result[2]->to_newick_string(), "(((4,2),1),3);");
    ASSERT_EQ(result[3]->to_newick_string(), "((4,2),(3,1));");
    ASSERT_EQ(result[4]->to_newick_string(), "(((4,2),3),1);");
    ASSERT_EQ(result[5]->to_newick_string(), "((4,1),(3,2));");
    ASSERT_EQ(result[6]->to_newick_string(), "(4,((3,1),2));");
    ASSERT_EQ(result[7]->to_newick_string(), "(4,(3,(2,1)));");
    ASSERT_EQ(result[8]->to_newick_string(), "(4,((3,2),1));");
    ASSERT_EQ(result[9]->to_newick_string(), "((4,(3,2)),1);");
    ASSERT_EQ(result[10]->to_newick_string(), "(((4,1),3),2);");
    ASSERT_EQ(result[11]->to_newick_string(), "((4,(3,1)),2);");
    ASSERT_EQ(result[12]->to_newick_string(), "(((4,3),1),2);");
    ASSERT_EQ(result[13]->to_newick_string(), "((4,3),(2,1));");
    ASSERT_EQ(result[14]->to_newick_string(), "(((4,3),2),1);");
}

TEST(PrintNewick, with_four_leafs) {

    auto l_1 = std::make_shared<Tree>("l1");
    auto l_2 = std::make_shared<Tree>("l2");
    auto l_3 = std::make_shared<Tree>("l3");
    auto l_4 = std::make_shared<Tree>("l4");
    auto c_1 = std::make_shared<Tree>(l_1, l_2, "c1");
    auto c_2 = std::make_shared<Tree>(l_3, l_4, "c2");
    auto r = std::make_shared<Tree>(c_1, c_2, "r");

    auto result = r->to_newick_string();

    ASSERT_EQ(result, "((l1,l2),(l3,l4));");
}

TEST(ApplyConstraintsTest, merges_to_two_parts) {

    std::set<leaf_label> leafs = {"1", "2", "3", "4"};

    std::vector<constraint> constraints;

    constraint cons1 = {"1", "1", "2", "3"};
    constraint cons2 = {"1", "1", "3", "4"};

    constraints.push_back(cons1);
    constraints.push_back(cons2);

    auto result = apply_constraints(leafs, constraints);

    ASSERT_EQ(result.size(), 2);
    ASSERT_THAT(*result[0], testing::ElementsAre("1", "2", "3"));
    ASSERT_THAT(*result[1], testing::ElementsAre("4"));
}

TEST(ExtractLeafLabelsFromSupertree, example_from_slides) {
    auto l_1 = std::make_shared<Tree>("l1");
    auto l_2 = std::make_shared<Tree>("l2");
    auto l_3 = std::make_shared<Tree>("l3");
    auto l_4 = std::make_shared<Tree>("l4");
    auto c_1 = std::make_shared<Tree>(l_1, l_2, "c1");
    auto c_2 = std::make_shared<Tree>(l_3, l_4, "c2");
    auto r = std::make_shared<Tree>(c_1, c_2, "r");

    auto leafs = extract_leaf_labels_from_supertree(r);

    ASSERT_THAT(leafs, testing::ElementsAre("l1", "l2", "l3", "l4"));
}

TEST(ExtractConstraintsFromTree, example_from_slides) {
    auto l_1 = std::make_shared<Tree>("l1");
    auto l_2 = std::make_shared<Tree>("l2");
    auto l_3 = std::make_shared<Tree>("l3");
    auto l_4 = std::make_shared<Tree>("l4");
    auto c_1 = std::make_shared<Tree>(l_1, l_2, "c1");
    auto c_2 = std::make_shared<Tree>(l_3, l_4, "c2");
    auto r = std::make_shared<Tree>(c_1, c_2, "r");

    auto constraints = extract_constraints_from_tree(r);

    ASSERT_EQ(constraints.size(), 2);
    //lca(l1, l2) < lca(l1, l4)
    ASSERT_EQ(constraints[0].smaller_left, "l1");
    ASSERT_EQ(constraints[0].smaller_right, "l2");
    ASSERT_EQ(constraints[0].bigger_left, "l1");
    ASSERT_EQ(constraints[0].bigger_right, "l4");

    //lca(l3, l4) < lca(l1, l4)
    ASSERT_EQ(constraints[1].smaller_left, "l3");
    ASSERT_EQ(constraints[1].smaller_right, "l4");
    ASSERT_EQ(constraints[1].bigger_left, "l1");
    ASSERT_EQ(constraints[1].bigger_right, "l4");
}

TEST(GetNthPartitionTuple, example_with_two_parts) {

    std::set<leaf_label> part1 = {"1", "2"};
    std::set<leaf_label> part2 = {"3"};

    std::vector<std::shared_ptr<std::set<leaf_label> > > partitions;
    partitions.push_back(std::make_shared<std::set<leaf_label>>(part1));
    partitions.push_back(std::make_shared<std::set<leaf_label>>(part2));

    ASSERT_EQ(number_partition_tuples(partitions), 1);

    std::shared_ptr<std::set<leaf_label> > part_one;
    std::shared_ptr<std::set<leaf_label> > part_two;
    std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 1);
    ASSERT_THAT(*part_one, testing::ElementsAre("1", "2"));
    ASSERT_THAT(*part_two, testing::ElementsAre("3"));
}

TEST(GetNthPartitionTuple, example_from_slides) {

    std::set<leaf_label> part1 = {"1", "2"};
    std::set<leaf_label> part2 = {"3"};
    std::set<leaf_label> part3 = {"4", "5"};

    std::vector<std::shared_ptr<std::set<leaf_label> > > partitions;
    partitions.push_back(std::make_shared<std::set<leaf_label>>(part1));
    partitions.push_back(std::make_shared<std::set<leaf_label>>(part2));
    partitions.push_back(std::make_shared<std::set<leaf_label>>(part3));

    ASSERT_EQ(number_partition_tuples(partitions), 3);

    std::shared_ptr<std::set<leaf_label> > part_one;
    std::shared_ptr<std::set<leaf_label> > part_two;
    std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 1);
    ASSERT_THAT(*part_one, testing::ElementsAre("1", "2"));
    ASSERT_THAT(*part_two, testing::ElementsAre("3", "4", "5"));
    std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 2);
    ASSERT_THAT(*part_one, testing::ElementsAre("3"));
    ASSERT_THAT(*part_two, testing::ElementsAre("1", "2", "4", "5"));
    std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 3);
    ASSERT_THAT(*part_one, testing::ElementsAre("1", "2", "3"));
    ASSERT_THAT(*part_two, testing::ElementsAre("4", "5"));
}

TEST(GetNthPartitionTuple, with_four_partitions) {

    std::set<leaf_label> part1 = {"1", "2"};
    std::set<leaf_label> part2 = {"3"};
    std::set<leaf_label> part3 = {"4", "5"};
    std::set<leaf_label> part4 = {"6", "7", "8"};

    std::vector<std::shared_ptr<std::set<leaf_label> > > partitions;
    partitions.push_back(std::make_shared<std::set<leaf_label>>(part1));
    partitions.push_back(std::make_shared<std::set<leaf_label>>(part2));
    partitions.push_back(std::make_shared<std::set<leaf_label>>(part3));
    partitions.push_back(std::make_shared<std::set<leaf_label>>(part4));

    ASSERT_EQ(number_partition_tuples(partitions), 7);

    std::shared_ptr<std::set<leaf_label> > part_one;
    std::shared_ptr<std::set<leaf_label> > part_two;
    std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 1);
    ASSERT_THAT(*part_one, testing::ElementsAre("1", "2"));
    ASSERT_THAT(*part_two, testing::ElementsAre("3", "4", "5", "6", "7", "8"));
    std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 2);
    ASSERT_THAT(*part_one, testing::ElementsAre("3"));
    ASSERT_THAT(*part_two,
                testing::ElementsAre("1", "2", "4", "5", "6", "7", "8"));
    std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 3);
    ASSERT_THAT(*part_one, testing::ElementsAre("1", "2", "3"));
    ASSERT_THAT(*part_two, testing::ElementsAre("4", "5", "6", "7", "8"));
    std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 4);
    ASSERT_THAT(*part_one, testing::ElementsAre("4", "5"));
    ASSERT_THAT(*part_two, testing::ElementsAre("1", "2", "3", "6", "7", "8"));
    std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 5);
    ASSERT_THAT(*part_one, testing::ElementsAre("1", "2", "4", "5"));
    ASSERT_THAT(*part_two, testing::ElementsAre("3", "6", "7", "8"));
    std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 6);
    ASSERT_THAT(*part_one, testing::ElementsAre("3", "4", "5"));
    ASSERT_THAT(*part_two, testing::ElementsAre("1", "2", "6", "7", "8"));
    std::tie(part_one, part_two) = get_nth_partition_tuple(partitions, 7);
    ASSERT_THAT(*part_one, testing::ElementsAre("1", "2", "3", "4", "5"));
    ASSERT_THAT(*part_two, testing::ElementsAre("6", "7", "8"));
}

TEST(FindAllRootedTrees, example_from_slides) {

    std::set<leaf_label> leafs = {"1", "2", "3", "4", "5"};

    std::vector<constraint> constraints;

    constraint cons1 = {"1", "3", "2", "2"};
    constraint cons2 = {"4", "4", "5", "2"};

    constraints.push_back(cons1);
    constraints.push_back(cons2);

    auto result = find_all_rooted_trees(leafs, constraints);

    ASSERT_EQ(result.size(), 9);
    ASSERT_EQ(result[0]->to_newick_string(), "((2,1),((5,3),4));");
    ASSERT_EQ(result[1]->to_newick_string(), "((2,1),(5,(4,3)));");
    ASSERT_EQ(result[2]->to_newick_string(), "((2,1),((5,4),3));");
    ASSERT_EQ(result[3]->to_newick_string(), "(3,(1,(2,(5,4))));");
    ASSERT_EQ(result[4]->to_newick_string(), "(3,(2,((5,1),4)));");
    ASSERT_EQ(result[5]->to_newick_string(), "(3,(2,(5,(4,1))));");
    ASSERT_EQ(result[6]->to_newick_string(), "(3,(2,((5,4),1)));");
    ASSERT_EQ(result[7]->to_newick_string(), "(3,((2,1),(5,4)));");
    ASSERT_EQ(result[8]->to_newick_string(), "(((2,1),3),(5,4));");
}


//TODO: Test takes to long, disabled for now
TEST(ListTrees, DISABLED_with_Pyron_data) {

    input_data *read_data = parse_input_data("../input/Pyron.data");
    assert(read_data != nullptr);
    ntree_t *tree = get_newk_tree("../input/Pyron.nwk");
    assert(tree != nullptr);

    missingData *m = initializeMissingData(read_data->number_of_species,
                                           read_data->number_of_partitions,
                                           const_cast<const char **>(read_data->names));
    copyDataMatrix(read_data->matrix, m);

    std::string root_species_name;
    std::shared_ptr<Tree> rtree = root_tree(tree, m, root_species_name);

    std::set<leaf_label> leafs;
    for (size_t k = 0; k < m->numberOfSpecies; k++) {
        leafs.insert(leaf_label(m->speciesNames[k]));
    }

    //TODO: hangs here
    for(auto &c : extract_constraints_from_supertree(rtree, m)) {
        fprintf(stderr,"(%s, %s) < (%s, %s);",c.smaller_left.c_str(),
        c.smaller_right.c_str(),
        c.bigger_left.c_str(),
        c.bigger_right.c_str());
    }

    auto result = find_all_rooted_trees(leafs, extract_constraints_from_supertree(rtree, m));

    dout("Tree count: " << result.size());

    ntree_destroy(tree);
    freeMissingData(m);
    free_input_data(read_data);
}

TEST(FindConstraintsTest, example_from_slides) {
    std::set<leaf_label> leaves = {"1", "2", "3"};

    std::vector<constraint> constraints;

    constraint cons1 = {"1", "3", "2", "2"};
    constraint cons2 = {"4", "4", "5", "2"};

    constraints.push_back(cons1);
    constraints.push_back(cons2);

    std::vector<constraint> result = find_constraints(leaves, constraints);

    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].smaller_left, "1");
    ASSERT_EQ(result[0].bigger_left, "3");
    ASSERT_EQ(result[0].smaller_right, "2");
    ASSERT_EQ(result[0].bigger_right, "2");
}

TEST(MergeSubtreesTest, simple_tree) {
    auto leaf_1 = std::make_shared<Tree>("leaf_1");
    auto leaf_2 = std::make_shared<Tree>("leaf_2");

    std::vector<std::shared_ptr<Tree> > left, right;

    left.push_back(leaf_1);
    right.push_back(leaf_2);

    std::vector<std::shared_ptr<Tree> > result = merge_subtrees(left, right);

    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0]->is_leaf(), 0);
    ASSERT_EQ(result[0]->left->label, std::string("leaf_1"));
    ASSERT_EQ(result[0]->right->label, std::string("leaf_2"));
}

TEST_P(RootedTreesAnalysis, ExamplesFromModifiedInput) {
    size_t max_time = 1000 * 30;

    auto param = GetParam();

    std::promise<bool> promisedFinished;
    auto futureResult = promisedFinished.get_future();
    std::thread([param](std::promise<bool> &finished) {
        test_rooted_trees(param.newick_file, param.data_file, param.expected_value);
        finished.set_value(true);
    }, std::ref(promisedFinished)).detach();
    EXPECT_TRUE(futureResult.wait_for(std::chrono::milliseconds(max_time))
                != std::future_status::timeout);
}

INSTANTIATE_TEST_CASE_P(ModifiedDataInstance, RootedTreesAnalysis, ::testing::Values(
    RootedTreesCountParameter("../input/modified/Ficus.nwk", "../input/modified/Ficus.data.1", 283815),
    RootedTreesCountParameter("../input/modified/Ficus.nwk", "../input/modified/Ficus.data.2", 851445),
    RootedTreesCountParameter("../input/modified/Ficus.nwk", "../input/modified/Ficus.data.3", 851445),
    RootedTreesCountParameter("../input/Caryophyllaceae.nwk", "../input/Caryophyllaceae.data", 718346120625)
));
