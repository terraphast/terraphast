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

static void test_rooted_trees(const char *newick_file, const char *data_file, long trees_on_terrace) {

    input_data *read_data = parse_input_data(data_file);
    assert(read_data != nullptr);
    ntree_t *tree = get_newk_tree(newick_file);
    assert(tree != nullptr);

    missingData *m = initializeMissingData(read_data->number_of_species,
                                           read_data->number_of_partitions,
                                           const_cast<const char **>(read_data->names));
    copyDataMatrix(read_data->matrix, m);

    for(auto &root_species_name : get_root_species(m)) {
        label_mapper id_to_label;

        ntree_t *future_root = get_leaf_by_name(tree,
                                                root_species_name.c_str());
        assert (future_root != nullptr);

        Tree rtree = root_at(future_root, id_to_label);

        std::set<std::string> m_labels;
        for (size_t k = 0; k < m->numberOfSpecies; k++) {
            bool noData = true;
            for (size_t j = 0; j < m->numberOfPartitions; j++) {
                if (getDataMatrix(m, k, j) == static_cast<unsigned char>(1)) {
                    noData = false;
                    break;
                }
            }
            if (noData) {
                dout("Species with no data in any partition: " << m->speciesNames[k] << "\n");
                assert(0);
            }
            m_labels.insert(std::string(m->speciesNames[k]));
        }

        for (size_t i = 0; i < id_to_label.size(); i++) {
            if (m_labels.count(std::string(id_to_label[i])) == 0) {
                dout("Species appears in newick file, but not in missing data file:" << id_to_label[i] << "\n");
                assert(0);
            }
        }

        auto leaves = LeafSet(id_to_label.size());

        CountAllRootedTrees algo;
        auto result = algo.scan_terrace(leaves, extract_constraints_from_supertree(rtree, m, id_to_label));

        ASSERT_EQ(result, trees_on_terrace);
    }

    ntree_destroy(tree);
    freeMissingData(m);
    free_input_data(read_data);
}

TEST(ApplyConstraintsTest, example_from_slides) {
    LeafSet leaves = {0, 1, 2, 3, 4, 5};

    std::vector<constraint> constraints;

    constraint cons1 = {1, 2, 3};
    constraint cons2 = {4, 5, 2};

    constraints.push_back(cons1);
    constraints.push_back(cons2);

    leaves.apply_constraints(constraints);
    auto result = leaves.get_list_of_partitions();

    ASSERT_EQ(result.size(), 4);

    ASSERT_THAT(result[0]->to_set(), testing::ElementsAre(0));
    ASSERT_THAT(result[1]->to_set(), testing::ElementsAre(1, 2));
    ASSERT_THAT(result[2]->to_set(), testing::ElementsAre(3));
    ASSERT_THAT(result[3]->to_set(), testing::ElementsAre(4, 5));
}

TEST(ApplyConstraintsTest, merge_all_sets) {

    LeafSet leaves = {0, 1, 2, 3, 4, 5};

    std::vector<constraint> constraints;

    constraint cons1 = {1, 2, 3};
    constraint cons2 = {4, 5, 2};
    constraint cons3 = {3, 4, 2};
    constraint cons4 = {1, 3, 5};

    constraints.push_back(cons1);
    constraints.push_back(cons2);
    constraints.push_back(cons3);
    constraints.push_back(cons4);

    leaves.apply_constraints(constraints);
    auto result = leaves.get_list_of_partitions();

    ASSERT_EQ(result.size(), 2);

    ASSERT_THAT(result[1]->to_set(), testing::ElementsAre(1, 2, 3, 4, 5));

}

TEST(ApplyConstraintsTest, no_merges) {

    LeafSet leaves = {0, 1, 2, 3, 4, 5};

    std::vector<constraint> constraints;

    constraint cons1 = {1, 6, 3};

    constraints.push_back(cons1);

    ASSERT_DEATH(leaves.apply_constraints(constraints),
                 "Assertion (.)* failed.");
}

TEST(GetAllBinaryTrees, with_tree_leafs) {

    LeafSet leaves = {0, 1, 2};
    label_mapper id_to_label({"1", "2", "3"});

    auto result = FindAllRootedTrees::get_all_binary_trees(leaves);
    
    ASSERT_EQ(result.size(), 3);

    std::vector<std::string> result_strings;

    for (auto temp : result) {
        result_strings.push_back(temp->to_newick_string(id_to_label));
    }

    ASSERT_TRUE(std::find(result_strings.begin(), result_strings.end(), "((1,2),3);") != result_strings.end());
    ASSERT_TRUE(std::find(result_strings.begin(), result_strings.end(), "(2,(1,3));") != result_strings.end());
    ASSERT_TRUE(std::find(result_strings.begin(), result_strings.end(), "((2,3),1);") != result_strings.end());
}

TEST(GetAllBinaryTrees, with_four_leaves) {

    LeafSet leaves = {0, 1, 2, 3};
    label_mapper id_to_label;
    id_to_label.labels = { "1", "2", "3", "4" };

    auto result = FindAllRootedTrees::get_all_binary_trees(leaves);

    ASSERT_EQ(result.size(), 15);

    ASSERT_EQ(result[0]->to_newick_string(id_to_label), "(((1,2),3),4);");
    ASSERT_EQ(result[1]->to_newick_string(id_to_label), "((2,(1,3)),4);");
    ASSERT_EQ(result[2]->to_newick_string(id_to_label), "(((2,3),1),4);");
    ASSERT_EQ(result[3]->to_newick_string(id_to_label), "((2,3),(1,4));");
    ASSERT_EQ(result[4]->to_newick_string(id_to_label), "(((2,3),4),1);");
    ASSERT_EQ(result[5]->to_newick_string(id_to_label), "((1,3),(2,4));");
    ASSERT_EQ(result[6]->to_newick_string(id_to_label), "(3,((1,2),4));");
    ASSERT_EQ(result[7]->to_newick_string(id_to_label), "(3,(2,(1,4)));");
    ASSERT_EQ(result[8]->to_newick_string(id_to_label), "(3,((2,4),1));");
    ASSERT_EQ(result[9]->to_newick_string(id_to_label), "((3,(2,4)),1);");
    ASSERT_EQ(result[10]->to_newick_string(id_to_label), "(((1,3),4),2);");
    ASSERT_EQ(result[11]->to_newick_string(id_to_label), "((3,(1,4)),2);");
    ASSERT_EQ(result[12]->to_newick_string(id_to_label), "(((3,4),1),2);");
    ASSERT_EQ(result[13]->to_newick_string(id_to_label), "((3,4),(1,2));");
    ASSERT_EQ(result[14]->to_newick_string(id_to_label), "(((3,4),2),1);");
}

TEST(ApplyConstraintsTest, merges_to_two_parts) {

    LeafSet leaves = {0, 1, 2, 3, 4};

    std::vector<constraint> constraints;

    constraint cons1 = {1, 2, 3};
    constraint cons2 = {1, 3, 4};

    constraints.push_back(cons1);
    constraints.push_back(cons2);
    leaves.apply_constraints(constraints);
    auto result = leaves.get_list_of_partitions();

    ASSERT_EQ(result.size(), 3);
    
    std::vector<size_t> elements = {0,1,2,3,4};
    std::set<size_t> set_0 (elements.begin(),     elements.begin() + 1);
    std::set<size_t> set_1 (elements.begin() + 1, elements.begin() + 4);
    std::set<size_t> set_2 (elements.begin() + 4, elements.end());
    
    std::vector<std::set<size_t>> sets = {set_0, set_1, set_2};
    std::set<std::set<size_t>> sets_set(sets.begin(), sets.end());
    
    ASSERT_EQ(result.size(), sets_set.size());
    
    for(auto r : result) {
        ASSERT_TRUE(sets_set.find(r->to_set()) != sets_set.end());
    }
}

TEST(ExtractConstraintsFromTree, example_from_slides) {
    auto l_1 = std::make_shared<Leaf>(1);
    auto l_2 = std::make_shared<Leaf>(2);
    auto l_3 = std::make_shared<Leaf>(3);
    auto l_4 = std::make_shared<Leaf>(4);
    auto c_1 = std::make_shared<InnerNode>(l_1, l_2);
    auto c_2 = std::make_shared<InnerNode>(l_3, l_4);
    auto r = std::make_shared<InnerNode>(c_1, c_2);

    auto constraints = r->extract_constraints();

    ASSERT_EQ(constraints.size(), 2);
    //lca(l1, l2) < lca(l1, l4)
    ASSERT_EQ(constraints[0].smaller_left, 1);
    ASSERT_EQ(constraints[0].smaller_right, 2);
    ASSERT_EQ(constraints[0].bigger, 4);

    //lca(l3, l4) < lca(l1, l4)
    ASSERT_EQ(constraints[1].smaller_left, 3);
    ASSERT_EQ(constraints[1].smaller_right, 4);
    ASSERT_EQ(constraints[1].bigger, 1);
}

TEST(GetNthPartitionTuple, example_with_two_parts) {

    LeafSet leaves = {0, 1, 2};
    std::vector<constraint> constraints;
    constraint cons1 = {0, 1, 2};
    constraints.push_back(cons1);

    leaves.apply_constraints(constraints);
    auto result = leaves.get_list_of_partitions();

    ASSERT_EQ(leaves.number_partition_tuples(), 1);

    std::shared_ptr<LeafSet> part_one;
    std::shared_ptr<LeafSet> part_two;
    std::tie(part_one, part_two) = leaves.get_nth_partition_tuple(1);
    ASSERT_THAT(part_one->to_set(), testing::ElementsAre(0, 1));
    ASSERT_THAT(part_two->to_set(), testing::ElementsAre(2));
}

TEST(GetNthPartitionTuple, example_from_slides) {

    LeafSet leaves = {0, 1, 2, 3, 4};
    std::vector<constraint> constraints;
    constraints.push_back({0, 1, 2});
    constraints.push_back({3, 4, 2}); //TODO

    leaves.apply_constraints(constraints);
    auto result = leaves.get_list_of_partitions();

    ASSERT_EQ(leaves.number_partition_tuples(), 3);

    std::shared_ptr<LeafSet> part_one;
    std::shared_ptr<LeafSet> part_two;

    std::tie(part_one, part_two) = leaves.get_nth_partition_tuple(1);
    ASSERT_THAT(part_one->to_set(), testing::ElementsAre(0, 1));
    ASSERT_THAT(part_two->to_set(), testing::ElementsAre(2, 3, 4));
    std::tie(part_one, part_two) = leaves.get_nth_partition_tuple(2);
    ASSERT_THAT(part_one->to_set(), testing::ElementsAre(2));
    ASSERT_THAT(part_two->to_set(), testing::ElementsAre(0, 1, 3, 4));
    std::tie(part_one, part_two) = leaves.get_nth_partition_tuple(3);
    ASSERT_THAT(part_one->to_set(), testing::ElementsAre(0, 1, 2));
    ASSERT_THAT(part_two->to_set(), testing::ElementsAre(3, 4));
}

TEST(GetNthPartitionTuple, with_four_partitions) {

    LeafSet leaves = {0, 1, 2, 3, 4, 5, 6, 7};
    std::vector<constraint> constraints;
    constraints.push_back({0, 1, 2});
    constraints.push_back({3, 4, 2}); //TODO
    constraints.push_back({5, 6, 2}); //TODO
    constraints.push_back({6, 7, 2}); //TODO

    leaves.apply_constraints(constraints);
    auto result = leaves.get_list_of_partitions();

    ASSERT_EQ(leaves.number_partition_tuples(), 7);

    std::shared_ptr<LeafSet> part_one;
    std::shared_ptr<LeafSet> part_two;
    std::tie(part_one, part_two) = leaves.get_nth_partition_tuple(1);
    ASSERT_THAT(part_one->to_set(), testing::ElementsAre(0, 1));
    ASSERT_THAT(part_two->to_set(), testing::ElementsAre(2, 3, 4, 5, 6, 7));
    std::tie(part_one, part_two) = leaves.get_nth_partition_tuple(2);
    ASSERT_THAT(part_one->to_set(), testing::ElementsAre(2));
    ASSERT_THAT(part_two->to_set(),
                testing::ElementsAre(0, 1, 3, 4, 5, 6, 7));
    std::tie(part_one, part_two) = leaves.get_nth_partition_tuple(3);
    ASSERT_THAT(part_one->to_set(), testing::ElementsAre(0, 1, 2));
    ASSERT_THAT(part_two->to_set(), testing::ElementsAre(3, 4, 5, 6, 7));
    std::tie(part_one, part_two) = leaves.get_nth_partition_tuple(4);
    ASSERT_THAT(part_one->to_set(), testing::ElementsAre(3, 4));
    ASSERT_THAT(part_two->to_set(), testing::ElementsAre(0, 1, 2, 5, 6, 7));
    std::tie(part_one, part_two) = leaves.get_nth_partition_tuple(5);
    ASSERT_THAT(part_one->to_set(), testing::ElementsAre(0, 1, 3, 4));
    ASSERT_THAT(part_two->to_set(), testing::ElementsAre(2, 5, 6, 7));
    std::tie(part_one, part_two) = leaves.get_nth_partition_tuple(6);
    ASSERT_THAT(part_one->to_set(), testing::ElementsAre(2, 3, 4));
    ASSERT_THAT(part_two->to_set(), testing::ElementsAre(0, 1, 5, 6, 7));
    std::tie(part_one, part_two) = leaves.get_nth_partition_tuple(7);
    ASSERT_THAT(part_one->to_set(), testing::ElementsAre(0, 1, 2, 3, 4));
    ASSERT_THAT(part_two->to_set(), testing::ElementsAre(5, 6, 7));
}

TEST(FindAllRootedTrees, example_from_slides) {

    LeafSet leaves = {0, 1, 2, 3, 4};
    label_mapper id_to_label;
    id_to_label.labels = {"1", "2", "3", "4", "5"};

    std::vector<constraint> constraints;

    constraint cons1 = {0, 1, 2};
    constraint cons2 = {3, 4, 1};

    constraints.push_back(cons1);
    constraints.push_back(cons2);

    FindAllRootedTrees algo;
    auto result = algo.scan_terrace(leaves, constraints);
    ASSERT_EQ(result.size(), 9);
    ASSERT_EQ(result[0]->to_newick_string(id_to_label), "((1,2),((3,4),5));");
    ASSERT_EQ(result[1]->to_newick_string(id_to_label), "((1,2),(4,(3,5)));");
    ASSERT_EQ(result[2]->to_newick_string(id_to_label), "((1,2),((4,5),3));");
    ASSERT_EQ(result[3]->to_newick_string(id_to_label), "(3,(1,(2,(4,5))));");
    ASSERT_EQ(result[4]->to_newick_string(id_to_label), "(3,(2,((1,4),5)));");
    ASSERT_EQ(result[5]->to_newick_string(id_to_label), "(3,(2,(4,(1,5))));");
    ASSERT_EQ(result[6]->to_newick_string(id_to_label), "(3,(2,((4,5),1)));");
    ASSERT_EQ(result[7]->to_newick_string(id_to_label), "(3,((1,2),(4,5)));");
    ASSERT_EQ(result[8]->to_newick_string(id_to_label), "(((1,2),3),(4,5));");
}

TEST(FindCompressedTree, example_from_slides) {

    LeafSet leaves = {0, 1, 2, 3, 4};
    label_mapper id_to_label;
    id_to_label.labels = { "1", "2", "3", "4", "5" };

    std::vector<constraint> constraints;

    constraint cons1 = {0, 1, 2};
    constraint cons2 = {3, 4, 1};

    constraints.push_back(cons1);
    constraints.push_back(cons2);

    FindCompressedTree algo;
    auto result = algo.scan_terrace(leaves, constraints);

    ASSERT_EQ(result->to_newick_string(id_to_label),
              "((1,2),{3,4,5})|(3,(1,(2,(4,5)))|(2,{1,4,5})|((1,2),(4,5)))|(((1,2),3),(4,5));");
}

TEST(FindConstraintsTest, example_from_slides) {
    LeafSet leaves = {0, 1, 2};
    label_mapper id_to_label;
    id_to_label.labels = { "1", "2", "3" };
    std::vector<constraint> constraints;

    constraint cons1 = {0, 1, 2};
    constraint cons2 = {3, 4, 1};
    constraints.push_back(cons1);
    constraints.push_back(cons2);
    std::vector<constraint> result = find_constraints(leaves, constraints);
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0].smaller_left, 0);
    ASSERT_EQ(result[0].smaller_right, 1);
    ASSERT_EQ(result[0].bigger, 2);
}


TEST(MergeSubtreesTest, simple_tree) {
    auto leaf_1 = std::make_shared<Leaf>(1);
    auto leaf_2 = std::make_shared<Leaf>(2);

    std::vector<Tree> left = { leaf_1 }, right = { leaf_2 };

    std::vector<Tree> result = FindAllRootedTrees::merge_subtrees(left, right);

    ASSERT_EQ(result.size(), 1);
    auto inner = std::static_pointer_cast<InnerNode>(result[0]);
    ASSERT_FALSE(inner->is_leaf());
    ASSERT_TRUE(inner->left->is_leaf());
    ASSERT_TRUE(inner->right->is_leaf());
    ASSERT_EQ(inner->left->get_leaf(), 1);
    ASSERT_EQ(inner->right->get_leaf(), 2);
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
        RootedTreesCountParameter("../input/modified/Allium.nwk",
                                  "../input/modified/Allium.data",
                                  8038035),
        RootedTreesCountParameter("../input/modified/Allium_Reduced.nwk",
                                  "../input/modified/Allium_Reduced.data",
                                  730680125),
        RootedTreesCountParameter("../input/modified/Allium_Tiny.nwk",
                                  "../input/modified/Allium_Tiny.data",
                                  35),
        RootedTreesCountParameter("../input/modified/Asplenium.nwk.1",
                                  "../input/modified/Asplenium.data.1",
                                  1),
        RootedTreesCountParameter("../input/modified/Asplenium.nwk.2",
                                  "../input/modified/Asplenium.data.2",
                                  95),
        RootedTreesCountParameter("../input/modified/Bouchenak.nwk",
                                  "../input/modified/Bouchenak.data",
                                  61261515),
        RootedTreesCountParameter("../input/modified/Caryophyllaceae.nwk",
                                  "../input/modified/Caryophyllaceae.data",
                                  718346120625),
        RootedTreesCountParameter("../input/modified/Eucalyptus.nwk.1",
                                  "../input/modified/Eucalyptus.data.1",
                                  229),
        RootedTreesCountParameter("../input/modified/Eucalyptus.nwk.1",
                                  "../input/modified/Eucalyptus.data.2",
                                  267),
        RootedTreesCountParameter("../input/modified/Eucalyptus.nwk.3",
                                  "../input/modified/Eucalyptus.data.3",
                                  9),
        RootedTreesCountParameter("../input/modified/Euphorbia.nwk.1",
                                  "../input/modified/Euphorbia.data.1",
                                  759),
        RootedTreesCountParameter("../input/modified/Euphorbia.nwk.2",
                                  "../input/modified/Euphorbia.data.2",
                                  759),
        RootedTreesCountParameter("../input/modified/Ficus.nwk.1",
                                  "../input/modified/Ficus.data.1",
                                  283815),
        RootedTreesCountParameter("../input/modified/Ficus.nwk.2",
                                  "../input/modified/Ficus.data.2",
                                  851445),
        RootedTreesCountParameter("../input/modified/Ficus.nwk.3",
                                  "../input/modified/Ficus.data.3",
                                  851445),
        RootedTreesCountParameter("../input/modified/Iris.nwk",
                                  "../input/modified/Iris.data",
                                  1),
        RootedTreesCountParameter("../input/modified/Meusemann.nwk",
                                  "../input/modified/Meusemann.data",
                                  1),
        RootedTreesCountParameter("../input/modified/Pyron.nwk",
                                  "../input/modified/Pyron.data",
                                  2205)
));
