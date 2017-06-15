#ifndef IFUGAO_H
#define IFUGAO_H

#include "tree.h"

#include <memory>
#include <vector>
#include <set>
#include <assert.h>
#include <iostream>
#include <iterator>

//TODO bad performance when doing string comparison all the time I guess...
typedef std::string leaf_number;
typedef std::set<leaf_number> leaf_set;

//lca(smaller_left, smaller_right) < lca(bigger_left, bigger_right)
struct constraint {
    leaf_number smaller_left;
    leaf_number bigger_left;
    leaf_number smaller_right;
    leaf_number bigger_right;
};

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
 * Calculates the number of trees on the terrace.
 *
 * @param constraints All constraints to fulfill.
 * @param leafs All leaves of the tree.
 * @param root_species_name the root species
 * @param file File to write all trees in newick format into, iff file != nullptr.
 * @return Number of all trees on the terrace.
 */
size_t list_trees(const std::vector<constraint> &constraints,
                  const leaf_number &root_species_name,
                  const leaf_set &leafs, FILE *file);

/**
 * Applies the given constraints on a set of given leaves, by merging them if
 * they two of them are on the left side of a constraint.
 *
 * @param leaves Leaves to apply the constraints on.
 * @param constraints Constraints to apply.
 * @return Sets merged from given sets according to the given contraints.
 */
std::vector<std::shared_ptr<leaf_set> > apply_constraints(
        const leaf_set &leaves, const std::vector<constraint> &constraints);

/** Combines all sets (constraints need to be applied already) */
std::vector<std::shared_ptr<Tree> > find_all_rooted_trees(const leaf_set &leaves,
                                                          const std::vector<constraint> &constraints);

/** Combines all sets (constraints need to be applied already) */
std::vector<std::shared_ptr<UnrootedTree> > find_all_unrooted_trees(const leaf_set &leaves,
                                                                    const std::vector<constraint> &constraints,
                                                                    const leaf_number &root_species_name);

/**
 * Returns a vector containing all constraints infered from the given supertree.
 *
 * @param supertree All supertree from which the constraints will be extracted
 * @return All constraints of the given super tree.
 */
std::vector<constraint> extract_constraints_from_tree(
        const std::shared_ptr<Tree> supertree);

std::vector<std::shared_ptr<Tree> > get_all_binary_trees(const leaf_set &leafs);

/**
 * Returns a vector containing all constraints that still are valid for the given set of leaves.
 *
 * @param leaves Leaves to search valid constraints for.
 * @param constraints All constraints that could still be valid.
 * @return All constraints that are still valid.
 */
std::vector<constraint> find_constraints(const leaf_set &leaves,
                                         const std::vector<constraint> &constraints);

/** merges two sub-trees */
std::vector<std::shared_ptr<Tree> > merge_subtrees(
        std::vector<std::shared_ptr<Tree> > &left,
        std::vector<std::shared_ptr<Tree> > &right);

/** write the given tree to the gien file */
void write_tree(Tree &tree, FILE &file);

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
        std::vector<std::shared_ptr<leaf_set> > &partitions) {
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
std::tuple<std::shared_ptr<leaf_set>, std::shared_ptr<leaf_set> > get_nth_partition_tuple(
        std::vector<std::shared_ptr<leaf_set> > &partitions, size_t n);

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
                         const std::vector<std::shared_ptr<leaf_set> >& set);

#endif /* IFUGAO_H */
