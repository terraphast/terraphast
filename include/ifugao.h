#ifndef IFUGAO_H
#define IFUGAO_H

#include "tree.h"
#include "constraints.h"

#include <memory>
#include <vector>
#include <set>
#include <assert.h>
#include <iostream>
#include <iterator>

/**
 * Method calling overview:
 * 
 *  list_trees
 *    combine_sets
 *      [recursion stop]
 *        get_all_binary_trees
 *      apply_constraints
 *     
 *   d_print_tree
 * 
 */

/**
 * Applies the given constraints on a set of given leaves, by merging them if
 * they two of them are on the left side of a constraint.
 *
 * @param leaves Leaves to apply the constraints on.
 * @param constraints Constraints to apply.
 * @return Sets merged from given sets according to the given contraints.
 */
std::vector<std::shared_ptr<std::set<leaf_number>> > apply_constraints(
        const std::set<leaf_number> &leaves, const std::vector<constraint> &constraints);

//TODO: only temporarly
size_t count_all_rooted_trees(const std::set<leaf_number> &leaves,
                              const std::vector<constraint> &constraints);

/**
 * Returns a vector containing all constraints infered from the given supertree.
 *
 * @param supertree All supertree from which the constraints will be extracted
 * @return All constraints of the given super tree.
 */
std::vector<constraint> extract_constraints_from_tree(
        const std::shared_ptr<Tree> supertree);

std::vector<std::shared_ptr<Tree> > get_all_binary_trees(const std::set<leaf_number> &leafs);

/**
 * Returns a vector containing all constraints that still are valid for the given set of leaves.
 *
 * @param leaves Leaves to search valid constraints for.
 * @param constraints All constraints that could still be valid.
 * @return All constraints that are still valid.
 */
std::vector<constraint> find_constraints(const std::set<leaf_number> &leaves,
                                         const std::vector<constraint> &constraints);

/** merges two sub-trees */
std::vector<std::shared_ptr<Tree> > merge_subtrees(
        const std::vector<std::shared_ptr<Tree> > &left,
        const std::vector<std::shared_ptr<Tree> > &right);

/**
 * Checks whenever the n-th bit is set in the given number
 * @param num the number to check
 * @param n the n-th bit to check whenever it is set or not
 * @return true of the n-th bit is set
 */
inline bool is_bit_set(size_t num, size_t n) {
    return 1 == ((num >> n) & 1);
}

/**
 * Returns the number of partition tuples that can be formed by combining the
 * given list of partitions. The formular is 2^(n-1) - 1, where n is the size of
 * the list.
 * @param partitions the list of partitions
 * @return the number of partition tuples that can be formed from the given list
 */
inline size_t number_partition_tuples(
        std::vector<std::shared_ptr<std::set<leaf_number>> > &partitions) {
    assert(partitions.size() > 1);

    return (1 << (partitions.size() - 1)) - 1;
}

//TODO Doc
std::set<leaf_number> extract_leaf_labels_from_supertree(
        std::shared_ptr<Tree> tree);

/**
 * Returns the n-th partition tuple of the given partition list
 * @param partitions the list of partitions
 * @param n the target n-th partition tuple to select, ranging from [1,2^(p-1)-1] where p is the number of partitions
 * @return the n-th partition tuple formed from the given partition list
 */
std::tuple<std::shared_ptr<std::set<leaf_number>>, std::shared_ptr<std::set<leaf_number>> > get_nth_partition_tuple(
        std::vector<std::shared_ptr<std::set<leaf_number>> > &partitions, size_t n);

template <typename T>
class TerraceAlgorithm {
public:
    virtual ~TerraceAlgorithm() = default;

    T scan_terrace(const std::set<leaf_number> &leafs, const std::vector<constraint> &constraints) {

        if (constraints.empty()) {
            auto result = scan_unconstraint_leaves(leafs);
            return result;
        }

        auto partitions = apply_constraints(leafs, constraints);
        T result;

        for (size_t i = 1; i <= number_partition_tuples(partitions); i++) {
            std::shared_ptr<std::set<leaf_number> > part_left;
            std::shared_ptr<std::set<leaf_number> > part_right;
            std::tie(part_left, part_right) = get_nth_partition_tuple(partitions, i);

            auto constraints_left = find_constraints(*part_left, constraints);
            auto constraints_right = find_constraints(*part_right, constraints);

            auto subtrees_left = scan_terrace(*part_left, constraints_left);
            auto subtrees_right = scan_terrace(*part_right, constraints_right);
            auto trees = combine_part_results(subtrees_left, subtrees_right);

            combine_bipartition_results(result, trees);
        }

        return result;
    }
protected:
    inline
    virtual T scan_unconstraint_leaves(const std::set<leaf_number> &leafs) = 0;

    inline
    virtual T combine_part_results(const T &left_part,
                                   const T &right_part) = 0;

    inline
    virtual void combine_bipartition_results(T &aggregation,
                                             const T &new_results) = 0;
};

typedef std::vector<std::shared_ptr<Tree> >  tree_result_list_t;

class FindAllRootedTrees : public TerraceAlgorithm<tree_result_list_t> {
protected:
    inline
    tree_result_list_t scan_unconstraint_leaves(const std::set<leaf_number> &leafs) {
        return get_all_binary_trees(leafs);
    }

    inline
    tree_result_list_t combine_part_results(const tree_result_list_t &left_part,
                                            const tree_result_list_t &right_part) {
        return merge_subtrees(left_part, right_part);
    }

    inline
    void combine_bipartition_results(tree_result_list_t &aggregation,
                                     const tree_result_list_t &new_results) {
        aggregation.insert(aggregation.end(), new_results.begin(), new_results.end());
    }
};

template<typename T>
std::ostream& operator<<(std::ostream &strm, const std::set<T>& set) {
    strm << "{";
    bool first = true;
    for(const T &elem : set) {
        if(first) {
            first = false;
        } else {
            strm << ",";
        }
        strm << elem;
    }
    return strm << "}";
}

template<typename T>
std::ostream& operator<<(std::ostream &strm, const std::vector<T>& set) {
    strm << "[";
    bool first = true;
    for(const T &elem : set) {
        if(first) {
            first = false;
        } else {
            strm << ",";
        }
        strm << elem;
    }
    return strm << "]";
}

std::ostream& operator<<(std::ostream &strm, const constraint& tree);

std::ostream& operator<<(std::ostream &strm,
                         const std::vector<std::shared_ptr<std::set<leaf_number>> >& set);

#endif /* IFUGAO_H */
