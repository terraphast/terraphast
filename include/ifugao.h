#pragma once
#include "types.h"

#include <iostream>
#include <iterator>
#include <gmpxx.h>

/* TODO UNUSED
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
*/
/**
 * Returns a vector containing all constraints infered from the given supertree.
 *
 * @param supertree All supertree from which the constraints will be extracted
 * @return All constraints of the given super tree.
 */
std::vector<constraint> extract_constraints_from_tree(
        const Tree supertree);

/**
 * Returns a vector containing all constraints that still are valid for the given set of leaves.
 *
 * @param leaves Leaves to search valid constraints for.
 * @param constraints All constraints that could still be valid.
 * @return All constraints that are still valid.
 */
std::vector<constraint> find_constraints(const LeafSet &leaves,
                                         const std::vector<constraint> &constraints);


template <typename T>
class TerraceAlgorithm {
public:
    virtual ~TerraceAlgorithm() = default;

    T scan_terrace(LeafSet &leaves,
                            const std::vector<constraint> &constraints,
                            bool unrooted = false) {
        if (constraints.empty()) {
            return scan_unconstraint_leaves(leaves, unrooted);
        } else {
            leaves.apply_constraints(constraints);
            return traverse_partitions(constraints, leaves, unrooted);
        }
    }

protected:
    inline
    virtual T traverse_partitions(const std::vector<constraint> &constraints,
                                  LeafSet &leaves,
                                  bool unrooted = false) {
        T result = initialize_result_type();
        bool cont = true;
        for (size_t i = 1; cont && i <= leaves.number_partition_tuples(); i++) {
            std::shared_ptr<LeafSet> part_left;
            std::shared_ptr<LeafSet> part_right;
            std::tie(part_left, part_right) = leaves.get_nth_partition_tuple(i);

            auto constraints_left = find_constraints(*part_left, constraints);
            auto constraints_right = find_constraints(*part_right, constraints);

            auto subtrees_left = scan_terrace(*part_left,
                                                       constraints_left);
            auto subtrees_right = scan_terrace(*part_right,
                                                        constraints_right);
            auto trees = combine_part_results(subtrees_left, subtrees_right);

            cont = combine_bipartition_results(result, trees, unrooted);
        }

        return result;
    }

    virtual T initialize_result_type() = 0;

    virtual T scan_unconstraint_leaves(LeafSet &leaves,
                                       bool unrooted = false) = 0;

    virtual T combine_part_results(const T &left_part,
                                   const T &right_part) = 0;

    virtual bool combine_bipartition_results(T &aggregation,
                                             const T &new_results,
                                             bool unrooted = false) = 0;
};


class FindCompressedTree : public TerraceAlgorithm<Tree> {
protected:
    inline
    Tree initialize_result_type() {
        std::vector<std::shared_ptr<Node>> trees;
        return std::make_shared<AllTreeCombinationsNode>(trees);
    }

    inline
    Tree scan_unconstraint_leaves(LeafSet &leaves, bool unrooted = false) {
        // TODO reconsider the whole std::set usage vs std::vector
        auto set = leaves.to_set();
        std::vector<leaf_number> leaves_vec(set.begin(), set.end());
        const auto result =
                std::make_shared<AllLeafCombinationsNode>(leaves_vec);
        if(unrooted) {
            return std::make_shared<UnrootedCombinationsNode>(result);
        } else {
            return result;
        }
    }

    inline
    Tree combine_part_results(const Tree &left_part, const Tree &right_part) {
        return std::make_shared<InnerNode>(left_part, right_part);
    }

    inline
    bool combine_bipartition_results(Tree &aggregation,
                                     const Tree &new_results,
                                     bool unrooted = false) {
        if (unrooted) {
            //TODO simply overwrite new_results
            auto all_trees =
                std::static_pointer_cast<AllTreeCombinationsNode>(aggregation);
            // Guaranteed to work, otherwise no constraints would exist and this
            // method would never get called.
            const auto inner = std::static_pointer_cast<InnerNode>(new_results);
            const auto unrooted = std::make_shared<UnrootedNode>(inner);
            all_trees->add_tree(unrooted);
        } else {
            auto all_trees =
                std::static_pointer_cast<AllTreeCombinationsNode>(aggregation);
            all_trees->add_tree(new_results);
        }
        return true;
    }
};

typedef std::vector<Tree> TreeList;
class FindAllRootedTrees : public TerraceAlgorithm<TreeList> {
protected:
    inline
    TreeList initialize_result_type() {
        return TreeList();
    }

    inline
    TreeList scan_unconstraint_leaves(LeafSet &leaves, bool unrooted = false) {
        TreeList binary_trees = get_all_binary_trees(leaves);
        
        if(unrooted) {
            for (size_t i = 0; i < binary_trees.size() ; ++i) {
                const auto inner =
                    std::static_pointer_cast<InnerNode>(binary_trees[i]);
                binary_trees[i] = std::make_shared<UnrootedNode>(inner);
            }
        }
        
        return binary_trees;
    }

    inline
    TreeList combine_part_results(const TreeList &left_part,
                                  const TreeList &right_part) {
        return merge_subtrees(left_part, right_part);
    }

    inline
    bool combine_bipartition_results(TreeList &aggregation,
                                     const TreeList &new_results,
                                     bool unrooted = false) {
        if(unrooted) {
            TreeList unrooted(new_results.size());
            // Guaranteed to work, otherwise no constraints would exist and this
            // method would never get called.
            for(size_t i = 0; i < new_results.size(); ++i) {
                auto inner =
                    std::static_pointer_cast<InnerNode>(new_results[i]);
                unrooted[i] = std::make_shared<UnrootedNode>(inner);
            }
            aggregation.insert(aggregation.end(), unrooted.begin(),
                               unrooted.end());
        } else {
            aggregation.insert(aggregation.end(), new_results.begin(),
                               new_results.end());
        }
        return true;
    }
public:
    /**
     * Returns all possible binary trees that can be combined by using the
     * leaves in the LeafSet.
     */
    static TreeList get_all_binary_trees(LeafSet &leaves);

    /**
     * Adds a leaf to a tree at all possible positions, leading to a list of
     * resulting trees.
     */
    static TreeList add_leaf_to_tree(const Tree &current_tree,
                                     const LeafPtr &leaf);

    /** merges two sub-trees */
    static TreeList merge_subtrees(const TreeList &left, const TreeList &right);
};

class CountAllRootedTrees : public TerraceAlgorithm<mpz_class> {
protected:
    inline
    mpz_class initialize_result_type() {
        return mpz_class(0);
    }
    inline
    mpz_class scan_unconstraint_leaves(LeafSet &leaves,
                                       bool unrooted = false) {
        // formula to count all trees is ((2n-5)!)!
        mpz_class result = 1;
        for(size_t i = 4; i <= (leaves.size() + 1); i++) {
            result *= (2*i-5);
        }
        return result;
    }

    inline
    mpz_class combine_part_results(const mpz_class &left_part,
                                   const mpz_class &right_part) {
        return left_part * right_part;
    }

    inline
    bool combine_bipartition_results(mpz_class &aggregation,
                                     const mpz_class &new_results,
                                     bool unrooted = false) {
        aggregation += new_results;
        return true;
    }
};

class CheckIfTerrace : public TerraceAlgorithm<bool> {
protected:
    inline
    bool traverse_partitions(const std::vector<constraint> &constraints,
                             LeafSet &leaves) {
        if(leaves.number_partition_tuples() > 1) {
            return true;
        }
        return TerraceAlgorithm<bool>::traverse_partitions(constraints, leaves);
    }

    inline
    bool initialize_result_type() {
        return false;
    }

    inline
    bool scan_unconstraint_leaves(LeafSet &leaves, bool unrooted = false) {
        return leaves.size() >= 3;
    }

    inline
    bool combine_part_results(const bool &left_part, const bool &right_part) {
        return left_part || right_part;
    }

    inline
    bool combine_bipartition_results(bool &aggregation,
                                     const bool &new_results,
                                     bool unrooted = false) {
        aggregation |= new_results;
        return aggregation;
    }
};
