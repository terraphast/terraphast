#ifndef IFUGAO_H
#define IFUGAO_H

#include "types.h"

#include <iostream>
#include <iterator>
#include <gmpxx.h>

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

/**
 * Returns a vector containing all constraints infered from the given supertree.
 *
 * @param supertree All supertree from which the constraints will be extracted
 * @return All constraints of the given super tree.
 */
std::vector<constraint> extract_constraints_from_tree(
        const std::shared_ptr<Tree> supertree);

std::vector<std::shared_ptr<Tree> > get_all_binary_trees(LeafSet &leaves);

/**
 * Returns a vector containing all constraints that still are valid for the given set of leaves.
 *
 * @param leaves Leaves to search valid constraints for.
 * @param constraints All constraints that could still be valid.
 * @return All constraints that are still valid.
 */
std::vector<constraint> find_constraints(const LeafSet &leaves,
                                         const std::vector<constraint> &constraints);

/** merges two sub-trees */
std::vector<std::shared_ptr<Tree> > merge_subtrees(
        const std::vector<std::shared_ptr<Tree> > &left,
        const std::vector<std::shared_ptr<Tree> > &right);

template <typename T>
class TerraceAlgorithm {
public:
    virtual ~TerraceAlgorithm() = default;

    T scan_terrace(LeafSet &leaves, const std::vector<constraint> &constraints) {
        if (constraints.empty()) {
            return scan_unconstraint_leaves(leaves);
        } else {
            leaves.apply_constraints(constraints);
            return traverse_partitions(constraints, leaves);
        }
    }

protected:
    inline
    virtual T traverse_partitions(const std::vector<constraint> &constraints,
                                  LeafSet &leaves) {
        T result = initialize_result_type();
        for (size_t i = 1; i <= leaves.number_partition_tuples(); i++) {
            std::shared_ptr<LeafSet> part_left;
            std::shared_ptr<LeafSet> part_right;
            std::tie(part_left, part_right) = leaves.get_nth_partition_tuple(i);

            auto constraints_left = find_constraints(*part_left, constraints);
            auto constraints_right = find_constraints(*part_right, constraints);

            auto subtrees_left = scan_terrace(*part_left, constraints_left);
            auto subtrees_right = scan_terrace(*part_right, constraints_right);
            auto trees = combine_part_results(subtrees_left, subtrees_right);

            combine_bipartition_results(result, trees);
        }

        return result;
    }

    virtual T initialize_result_type() = 0;

    virtual T scan_unconstraint_leaves(LeafSet &leaves) = 0;

    virtual T combine_part_results(const T &left_part,
                                   const T &right_part) = 0;

    virtual void combine_bipartition_results(T &aggregation,
                                             const T &new_results) = 0;
};

typedef std::vector<std::shared_ptr<Tree>> tree_result_list;

class FindCompressedTree : public TerraceAlgorithm<std::shared_ptr<Tree>> {
protected:
    inline
    std::shared_ptr<Tree> initialize_result_type() {
        return std::make_shared<PartitionNode>();
    }

    inline
    std::shared_ptr<Tree> scan_unconstraint_leaves(LeafSet &leaves) {
        // formula to count all trees is ((2n-5)!)!
        return std::make_shared<LeafSetNode>(leaves.to_set());
    }

    inline
    std::shared_ptr<Tree> combine_part_results(const std::shared_ptr<Tree> &left_part,
                                          const std::shared_ptr<Tree> &right_part) {
        return std::make_shared<Tree>(left_part, right_part);
    }

    inline
    void combine_bipartition_results(std::shared_ptr<Tree> &aggregation,
                                     const std::shared_ptr<Tree> &new_results) {
        auto p = std::static_pointer_cast<PartitionNode>(aggregation);
        p->partitions.push_back(new_results);
    }
};

class FindAllRootedTrees : public TerraceAlgorithm<tree_result_list> {
protected:
    inline
    tree_result_list initialize_result_type() {
        return tree_result_list();
    }

    inline
    tree_result_list scan_unconstraint_leaves(LeafSet &leaves) {
        return get_all_binary_trees(leaves);
    }

    inline
    tree_result_list combine_part_results(const tree_result_list &left_part,
                                            const tree_result_list &right_part) {
        return merge_subtrees(left_part, right_part);
    }

    inline
    void combine_bipartition_results(tree_result_list &aggregation,
                                     const tree_result_list &new_results) {
        aggregation.insert(aggregation.end(), new_results.begin(), new_results.end());
    }
};

class CountAllRootedTrees : public TerraceAlgorithm<mpz_class> {
protected:
    inline
    mpz_class initialize_result_type() {
        return mpz_class(0);
    }
    inline
    mpz_class scan_unconstraint_leaves(LeafSet &leaves) {
        mpz_class result = 1;
        for(size_t i = 4; i <= (leaves.size() + 1); i++) {
            result *= (2*i-5);
        }
        return result;
    }

    inline
    mpz_class combine_part_results(const mpz_class &left_part, const mpz_class &right_part) {
        return left_part * right_part;
    }

    inline
    void combine_bipartition_results(mpz_class &aggregation, const mpz_class &new_results) {
        aggregation += new_results;
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
    bool scan_unconstraint_leaves(LeafSet &leaves) {
        return leaves.size() >= 3;
    }

    inline
    bool combine_part_results(const bool &left_part, const bool &right_part) {
        return left_part || right_part;
    }

    inline
    void combine_bipartition_results(bool &aggregation, const bool &new_results) {
        aggregation = (aggregation || new_results);
    }
};

#endif /* IFUGAO_H */
