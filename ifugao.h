typedef int leaf;

public struct tree {
  
} tree

public struct constraint {
  leaf smaller_left;
  leaf bigger_left;
  leaf smaller_right;
  leaf bigger_right;
}

/** Calls apply_constraints first, then combine_sets and writes all found trees.
  @param count_only Only count trees, don't write them
 */
long list_trees(const std::vector<constraint> &constraints, std::vector<long> &leafs, bool count_only, FILE &file);

/** Combines all sets (constraints need to be applied already) */
std::set<tree> combine_sets(const std::vector<std::shared_ptr<std::set>> &set_array, const std::vector<constraint> &constraints)

std::vector<std::shared_ptr<std::set>> apply_constraints(const std::vector<std::shared_ptr<std::set>> &set_array, const std::vector<constraint> &constraints);

/** Returns a vector containing all constraints that still are valid for the given set of leaves */
std::vector<constraint> find_constraints(const std::set &set, const std::vector<constraint> &constraints);

/** merges two sub-trees */
std::set<tree> merge_subtrees(std::set<tree> &left, std::set<tree> &right);

/** write the given tree to the gien file */
void write_tree(tree &tree, FILE &file);

