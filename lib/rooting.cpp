
#include <stack>
#include <iostream>
#include <terraces/rooting.hpp>
#include "utils.hpp"

namespace terraces {

namespace rooting {

	using node_stack = std::stack< index, std::vector<index>>;

} // namespace rooting

void reroot_inplace(tree& t, index root_leaf) {
	if (t[root_leaf].lchild() != none || t[root_leaf].rchild() != none) {
		std::string msg =  "The given index root_leaf does not point ot a leaf";
		throw std::invalid_argument( msg );
	}
	
	node old_root = t[0];
	node leaf_node = t[root_leaf];
	node new_root = node{none, leaf_node.parent(), root_leaf};
	
	t[old_root.lchild()].parent() = old_root.rchild();
	t[old_root.rchild()].lchild() = old_root.lchild();

	rooting::node_stack inner_nodes = rooting::node_stack{};

	index current_node = t[root_leaf].parent();
	while (current_node != 0) {
		inner_nodes.push(current_node);
		current_node = t[current_node].parent();
	}
	
	while (inner_nodes.size() > 0) {
		index i = inner_nodes.top();
		index old_parent = t[i].parent();
		t[i].parent() = t[i].lchild();
		t[i].lchild() = old_parent;
		inner_nodes.pop();
	}

	old_root = new_root;
	t[root_leaf].parent() = 0;
}

} // namespace terraces


