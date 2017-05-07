#include "ifugao.h"
#include "assert.h"

std::vector<binary_tree> merge_subtrees(std::vector<binary_tree> &left, std::vector<binary_tree> &right) {

	std::vector<binary_tree> merged_trees;

	for(binary_tree l: left) {
		for(binary_tree r: right) {
			binary_tree new_tree;
			new_tree.is_leaf = false;
			new_tree.label = NULL;
			new_tree.left_subtree = &l;
			new_tree.right_subtree = &r;
			merged_trees.push_back(new_tree);
		}
	}

	assert(merged_trees.size() == left.size() * right.size());
	return merged_trees;
}
