#include "tree.h"

#include <string>
#include <sstream>
#include <memory>

static void to_newick_string_rec(std::stringstream &ss, Tree &node) {
	if(node.is_leaf()) {
		ss << node.label;
	} else {
		ss << "(";
		to_newick_string_rec(ss, *node.left);
		ss << ",";
		to_newick_string_rec(ss, *node.right);
		ss << ")";
	}
}

std::string Tree::to_newick_string() {
	std::stringstream ss;
	to_newick_string_rec(ss, *this);
	ss << ";";
	return ss.str();
}
