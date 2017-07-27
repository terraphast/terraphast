#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <stack>
#include <terraces/rooting.hpp>

namespace terraces {

namespace rooting {

/** Our standard case: Swap child and parent indices depending on the tree traversal direction.
*/
void adjust_parent_child_edge(bool coming_from_left, node& current_node,
                              index original_parent_index) {
	if (coming_from_left) {
		current_node.parent() = current_node.lchild();
		current_node.lchild() = original_parent_index;
	} else {
		current_node.parent() = current_node.rchild();
		current_node.rchild() = original_parent_index;
	}
}

/** Inserts a new root as the parent of the root leaf and the subtree containint all other nodes.

        This operation is executed if we're currently at the direct parent of the root leaf.
        The operation overwrites our current parent.
*/
void insert_new_root(bool coming_from_left, node& new_root, node& leaf_node, node& current_node,
                     index current_node_index, index root_leaf, index original_parent_index) {
	leaf_node.parent() = 0;
	new_root.lchild() = current_node_index;
	new_root.rchild() = root_leaf;
	current_node.parent() = 0; // CHECK
	if (coming_from_left) {
		current_node.lchild() = original_parent_index;
	} else {
		current_node.rchild() = original_parent_index;
	}
}

/** Assuming the current node is a child of the root node (in the input tree), we
        properly insert the subtree under the root's other child int our rerooted tree.

        Re-rooting as time-complexity O(tree_height).
*/
void link_to_sibling_tree(bool coming_from_left, tree& t, node old_root, node& current_node,
                          index current_node_index) {
	if (coming_from_left) {                                // we swap out our left child...
		if (old_root.lchild() == current_node_index) { //...for the old root's right child.
			current_node.lchild() = old_root.rchild();
		} else { //...for the old root's left child
			current_node.lchild() = old_root.lchild();
		}
		t[current_node.lchild()].parent() = current_node_index;

	} else {                                               // we swap out out right child...
		if (old_root.lchild() == current_node_index) { //...for the old root's right child.
			current_node.rchild() = old_root.rchild();
		} else { //...for the old root's left child
			current_node.rchild() = old_root.lchild();
		}
		t[current_node.rchild()].parent() = current_node_index;
	}
}
}

void reroot_inplace(tree& t, index root_leaf) {
	utils::ensure<std::invalid_argument>(t[root_leaf].lchild() == none,
	                                     "The given index root_leaf is no leaf index.");
	utils::ensure<std::invalid_argument>(t[root_leaf].rchild() == none,
	                                     "The given index root_leaf is no leaf index.");
	utils::ensure<std::invalid_argument>(terraces::is_valid_tree(t),
	                                     "The given tree is no valid tree.");

	node old_root = t[0];

	bool root_leaf_is_child_of_root =
	        (old_root.lchild() == root_leaf || old_root.rchild() == root_leaf);
	if (root_leaf_is_child_of_root) {
		return;
	}

	(void)old_root;
	node new_root = node{none, none, none};
	node& leaf_node = t[root_leaf];

	bool coming_from_left = true;
	if (root_leaf == t[leaf_node.parent()].rchild()) {
		coming_from_left = false;
	}

	bool is_first_node = true;
	bool is_last_node = false;
	index current_node_index = t[root_leaf].parent();
	while (current_node_index != 0) {

		node& current_node = t[current_node_index];
		index original_parent_index = current_node.parent();
		is_last_node = (original_parent_index == 0);

		rooting::adjust_parent_child_edge(coming_from_left, current_node,
		                                  original_parent_index);
		if (is_first_node) {
			rooting::insert_new_root(coming_from_left, new_root, leaf_node,
			                         current_node, current_node_index, root_leaf,
			                         original_parent_index);
			is_first_node = false;
		}

		if (is_last_node) {
			rooting::link_to_sibling_tree(coming_from_left, t, old_root, current_node,
			                              current_node_index);
		}

		// CLEANUP
		coming_from_left = t[original_parent_index].lchild() == current_node_index;
		current_node_index = original_parent_index;
	}
	t[0] = new_root;

	utils::ensure<std::invalid_argument>(
	        terraces::is_valid_tree(t),
	        "The given tree is not a valid tree anymore. Please check rooting.cpp");
}

} // namespace terraces
