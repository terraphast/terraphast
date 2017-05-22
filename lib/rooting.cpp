
#include <stack>
#include <iostream>
#include <terraces/rooting.hpp>
#include "utils.hpp"
#include <iostream>

namespace terraces {

namespace rooting {

	using node_stack = std::stack< index, std::vector<index>>;

} // namespace rooting

void reroot_inplace(tree& t, index root_leaf) {
	utils::ensure<std::invalid_argument>(t[root_leaf].lchild() == none, 
										 "The given index root_leaf is no leaf index.");
	utils::ensure<std::invalid_argument>(t[root_leaf].rchild() == none, 
										 "The given index root_leaf is no leaf index.");
	
	node old_root = t[0];
	node new_root = node{none, t[root_leaf].parent(), root_leaf};
	t[root_leaf].parent() = 0;

	
	bool coming_from_left = true;
	if (root_leaf == t[t[root_leaf].parent()].rchild()) {
		coming_from_left = false;
	}
	
	bool is_first_node = true;
	index current_node_index = t[root_leaf].parent();
	node current_node = t[current_node_index];
	index original_parent_index = current_node.parent();
	std::cout << "Going upwards...";
	while (current_node_index != 0) {
		std::cout << "current_node_index BEGIN: " << current_node_index << "\n";
		std::cout << "current_parent_index BEGIN: " << original_parent_index << "\n";
		std::cout << "current_node's parent BEGIN: " << current_node.parent() << "\n";
		std::cout << "current_node's lchild BEGIN: " << current_node.lchild() << "\n";
		std::cout << "current_node's rchild BEGIN: " << current_node.rchild() << "\n";
		if (coming_from_left) {
			current_node.parent() = current_node.lchild();
			current_node.lchild() = original_parent_index;
			t[current_node.lchild()].parent() = current_node_index;
		} else {
			current_node.parent() = current_node.rchild();
			current_node.rchild() = original_parent_index;
			t[current_node.rchild()].parent() = current_node_index;
		}
		if (is_first_node) {
			current_node.parent() = 0;
		}
		if (t[original_parent_index].rchild() == current_node_index) {
			coming_from_left = false;
		} else {
			coming_from_left = true;
		}
		std::cout << "current_node's parent END: " << current_node.parent() << "\n";
		std::cout << "current_node's lchild END: " << current_node.lchild() << "\n";
		std::cout << "current_node's rchild END: " << current_node.rchild() << "\n";
		current_node_index = original_parent_index;
		current_node = t[current_node_index];
		original_parent_index = current_node.parent();
		std::cout << "current_node_index END: " << current_node_index << "\n";
		std::cout << "current_parent_index END: " << original_parent_index << "\n";
	}
	std::cout << "finished\n";

	t[old_root.lchild()].parent() = old_root.rchild();
	t[old_root.rchild()].lchild() = old_root.lchild();
	t[0] = new_root;
}

} // namespace terraces


