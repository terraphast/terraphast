#include "tree.h"

#include <string>
#include <sstream>
#include <memory>
#include <map>
#include <assert.h>

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

std::string UnrootedTree::to_newick_string() {
	std::stringstream ss;
	ss << "(";
	if(this->elem1 != nullptr) {
		to_newick_string_rec(ss, *this->elem1);
	}
	if(this->elem2 != nullptr) {
		if(this->elem1 != nullptr) {
			ss << ",";
		}
		to_newick_string_rec(ss, *this->elem2);
	}
	if(this->elem3 != nullptr) {
		if(this->elem1 != nullptr || this->elem2 != nullptr) {
			ss << ",";
		}
		to_newick_string_rec(ss, *this->elem3);
	}
	ss << ");";
	return ss.str();
}

static std::shared_ptr<Tree> deep_copy(std::shared_ptr<Tree> tree,
		std::map<std::shared_ptr<Tree>, std::shared_ptr<Tree>> &cover_map) {
	if (tree == nullptr) {
		return nullptr;
	}
	if (cover_map.count(tree) > 0) {
		return cover_map[tree];
	}

	auto node = std::make_shared<Tree>();
	node->label = tree->label;
	cover_map[tree] = node;
	cover_map[node] = node;

	node->parent = deep_copy(tree->parent, cover_map);
	node->left = deep_copy(tree->left, cover_map);
	node->right = deep_copy(tree->right, cover_map);

	return node;
}

std::shared_ptr<Tree> deep_copy(std::shared_ptr<Tree> tree) {
	std::map<std::shared_ptr<Tree>, std::shared_ptr<Tree>> cover_map;
	return deep_copy(tree, cover_map);
}

static void d_print_tree_rec(std::ostream &strm,
                                    const std::shared_ptr<Tree> tree,
                                    const int depth) {
	strm << "Label: " << tree->label << std::endl;
	assert(
			depth == 1
			|| (tree->parent != nullptr
				&& (tree->parent->left == tree
					|| tree->parent->right == tree)));
	if (tree->left != nullptr) {
		for (int j = 0; j < depth * 4; j++) {
            strm << " ";
		}
        strm << "L:";
		d_print_tree_rec(strm, tree->left, depth + 1);
	}
	if (tree->right != nullptr) {
		for (int j = 0; j < depth * 4; j++) {
			strm << " ";
		}
		strm << "R:";
		d_print_tree_rec(strm, tree->right, depth + 1);
	}
}

std::ostream& operator<<(std::ostream &strm, const std::shared_ptr<Tree> tree) {
    strm << "Dump Tree:" << std::endl;
    d_print_tree_rec(strm, tree, 1);
    return strm;
}
