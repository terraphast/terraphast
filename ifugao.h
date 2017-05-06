#ifndef IFUGAO_H
#define IFUGAO_H

#include "vector"
#include "set"
#include "tr1/memory"

typedef int leaf;

struct binary_tree {
	bool is_leaf; // 0 if node is an internal node, 1 if node is a leaf
	char* label; // name of the leaf, NULL pointer for an internal node
	binary_tree* left_subtree; // NULL pointer for leaf
	binary_tree* right_subtree; // NULL pointer for leaf
};

struct constraint {
  leaf smaller_left;
  leaf bigger_left;
  leaf smaller_right;
  leaf bigger_right;
};

/** Calls apply_constraints first, then combine_sets and writes all found trees.
  @param count_only Only count trees, don't write them
 */
long list_trees(const std::vector<constraint> &constraints, std::vector<long> &leafs, bool count_only, FILE &file);

/** Combines all sets (constraints need to be applied already) */
std::vector<binary_tree> combine_sets(const std::vector<std::tr1::shared_ptr<std::set<leaf> > > &set_array, const std::vector<constraint> &constraints);

std::vector<std::tr1::shared_ptr<std::set<leaf> > > apply_constraints(const std::vector<std::tr1::shared_ptr<std::set<leaf> > > &set_array, const std::vector<constraint> &constraints);

/** Returns a vector containing all constraints that still are valid for the given set of leaves */
std::vector<constraint> find_constraints(const std::set<leaf> &set, const std::vector<constraint> &constraints);

/** merges two sub-trees */
std::vector<binary_tree> merge_subtrees(std::vector<binary_tree> &left, std::vector<binary_tree> &right);

/** write the given tree to the gien file */
void write_tree(binary_tree &tree, FILE &file);

#endif /* IFUGAO_H */
