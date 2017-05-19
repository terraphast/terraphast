#ifndef IFUGAO_H
#define IFUGAO_H

#include "tree.h"

#include <memory>
#include <vector>
#include <set>

typedef Tree Leaf;

/*
struct Tree {
	bool is_leaf; // 0 if node is an internal node, 1 if node is a leaf
	char* label; // name of the leaf, NULL pointer for an internal node
	Tree* left_subtree; // NULL pointer for leaf
	Tree* right_subtree; // NULL pointer for leaf
};
*/
//lca(smaller_left, smaller_right) < lca(bigger_left, bigger_right)
struct constraint {
	Leaf smaller_left;
	Leaf bigger_left;
	Leaf smaller_right;
	Leaf bigger_right;
};

/**
 * Calculates the number of trees on the terrace.
 *
 * @param constraints All constraints to fulfill.
 * @param leaves All leaves of the tree.
 * @param count_only Return only the number of trees iff count_only = 1.
 * @param file File to write all trees in newick format into, iff count_only = 0.
 * @return Number of all trees on the terrace.
 */
long list_trees(const std::vector<constraint> &constraints,
		const std::set<Leaf> &leaves, bool count_only, FILE &file);

/**
 * Applies the given constraints on a set of given leaves.
 *
 * @param leaves Leaves to apply the constraints on.
 * @param constraints Constraints to apply.
 * @return Merged sets containing the leaves.
 */
std::vector<std::shared_ptr<std::set<Leaf> > > apply_constraints(
		const std::set<Leaf> &leaves,
		const std::vector<constraint> &constraints);

/** Combines all sets (constraints need to be applied already) */
std::vector<Tree> combine_sets(
		const std::vector<std::set<Leaf> > &set_array,
		const std::vector<constraint> &constraints);

/**
 * Returns a vector containing all constraints infered from the given supertree.
 *
 * @param supertree All supertree from which the constraints will be extracted
 * @return All constraints of the given super tree and a set of leave numbers.
 */
std::tuple<std::set<Leaf>, std::vector<constraint> > extract_constraints_from_supertree(
		const Tree *supertree);

/**
 * Returns a vector containing all constraints that still are valid for the given set of leaves.
 *
 * @param leaves Leaves to search valid constraints for.
 * @param constraints All constraints that could still be valid.
 * @return All constraints that are still valid.
 */
std::vector<constraint> find_constraints(const std::set<Leaf> &leaves,
		const std::vector<constraint> &constraints);

/** merges two sub-trees */
std::vector<Tree> merge_subtrees(std::vector<Tree> &left,
		std::vector<Tree> &right);

/** write the given tree to the gien file */
void write_tree(Tree &tree, FILE &file);

#endif /* IFUGAO_H */
