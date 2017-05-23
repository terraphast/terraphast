#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <stack>
#include <terraces/rooting.hpp>

namespace terraces {

std::ostream& operator<<(std::ostream& ss, const tree& t) {
	ss << "TREE [\n";
	for (auto node : t) {
		ss << "\t{" << node.parent() << ", " << node.lchild() << ", " << node.rchild()
		   << "}\n";
	}
	ss << "]\n";
	return ss;
}

void reroot_inplace(tree& t, index root_leaf) {
	utils::ensure<std::invalid_argument>(t[root_leaf].lchild() == none,
	                                     "The given index root_leaf is no leaf index.");
	utils::ensure<std::invalid_argument>(t[root_leaf].rchild() == none,
	                                     "The given index root_leaf is no leaf index.");

	node old_root = t[0]; // Copy

	bool root_leaf_is_child_of_root =
	        (old_root.lchild() == root_leaf || old_root.rchild() == root_leaf);
	if (root_leaf_is_child_of_root) {
		return;
	}

	(void)old_root;
	node new_root = node{none, none, none};
	node& leaf_node = t[root_leaf]; // Alias/Reference

	bool coming_from_left = true;
	if (root_leaf == t[leaf_node.parent()].rchild()) {
		coming_from_left = false;
	}

	bool is_first_node = true;
	bool is_last_node = false;
	index current_node_index = t[root_leaf].parent();
	// std::cout << "Going upwards...\n";
	while (current_node_index != 0) {

		node& current_node = t[current_node_index];
		index original_parent_index = current_node.parent();
		is_last_node = (original_parent_index == 0);
		//		std::cout << "IS LAST " << is_last_node << "\n";
		//		std::cout << "IS FIRST " << is_first_node << "\n";
		//		std::cout << "FROM LEFT? " << coming_from_left << "\n";

		// standard case: Swap child and parent indices.
		if (coming_from_left) {
			current_node.parent() = current_node.lchild();
			current_node.lchild() = original_parent_index;
		} else {
			current_node.parent() = current_node.rchild();
			current_node.rchild() = original_parent_index;
		}

		// Direct parent of root_leaf
		// Overwrites our parent
		if (is_first_node) {
			leaf_node.parent() = 0;
			new_root.lchild() = current_node_index;
			new_root.rchild() = root_leaf;
			current_node.parent() = 0; // CHECK
			if (coming_from_left) {
				current_node.lchild() = original_parent_index;
			} else {
				current_node.rchild() = original_parent_index;
			}
			is_first_node = false;
		}

		// Directly under old_root: We're now the parent of the root's other child.
		// Overwrites the approrpiate child index.
		if (is_last_node) {
			if (coming_from_left) { // we swap out our left child...
				if (old_root.lchild() ==
				    current_node_index) { //...for the old root's right child.
					current_node.lchild() = old_root.rchild();
				} else { //...for the old root's left child
					current_node.lchild() = old_root.lchild();
				}
				t[current_node.lchild()].parent() = current_node_index;

			} else { // we swap out out right child...
				if (old_root.lchild() ==
				    current_node_index) { //...for the old root's right child.
					current_node.rchild() = old_root.rchild();
				} else { //...for the old root's left child
					current_node.rchild() = old_root.lchild();
				}
				t[current_node.rchild()].parent() = current_node_index;
			}
		}

		// CLEANUP
		if (t[original_parent_index].lchild() == current_node_index) {
			coming_from_left = true;
		} else {
			coming_from_left = false;
		}
		current_node_index = original_parent_index;
	}
	// std::cout << "finished\n";
	//	std::cout << "FROM LEFT END? " << coming_from_left << "\n";

	t[0] = new_root;
	//	std::cout << "ROOT END:" << t[0] << "\n";
}

} // namespace terraces