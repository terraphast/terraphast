#include "types/tree.h"

#include <sstream>
#include <map>
#include "debug.h"

void Tree::to_newick_string(std::stringstream &ss,
                                   const std::vector<std::string> &ids_to_lables) const {
    if (this->is_leaf()) {
        ss << ids_to_lables[this->id];
    } else {
        ss << "(";
        this->left->to_newick_string(ss, ids_to_lables);
        ss << ",";
        this->right->to_newick_string(ss, ids_to_lables);
        ss << ")";
    }
}

std::shared_ptr<Tree> root(std::shared_ptr<Tree> t) {
    if (t->parent.lock() == nullptr) {
        return t;
    }

    return root(t->parent.lock());
}

std::string Tree::to_newick_string(const std::vector<std::string> &ids_to_lables) const {
    std::stringstream ss;
    this->to_newick_string(ss, ids_to_lables);
    ss << ";";
    return ss.str();
}

std::string Tree::to_newick_string(const std::vector<std::string> &ids_to_lables,
                                   const std::string &root_label) const {
    std::stringstream ss;
    ss << "(";
    ss << root_label;
    ss << ",";
    if (this->is_leaf()) {
        ss << ids_to_lables[this->id];
    } else {
        this->left->to_newick_string(ss, ids_to_lables);
        ss << ",";
        this->right->to_newick_string(ss, ids_to_lables);
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
    node->id = tree->id;
    cover_map[tree] = node;
    cover_map[node] = node;

    node->left = deep_copy(tree->left, cover_map);
    node->right = deep_copy(tree->right, cover_map);
    node->parent = deep_copy(tree->parent.lock(), cover_map);

    return node;
}

std::tuple<std::shared_ptr<Tree>, std::shared_ptr<Tree>> deep_copy(std::shared_ptr<Tree> tree) {
    std::map<std::shared_ptr<Tree>, std::shared_ptr<Tree>> cover_map;
    auto result = deep_copy(tree, cover_map);
    return std::make_tuple(result, root(result));
}

static void d_print_tree_rec(std::ostream &strm,
                             const std::shared_ptr<Tree> tree,
                             const int depth) {
    assert(
            depth == 1
            || (tree->parent.lock() != nullptr
                && (tree->parent.lock()->left == tree
                    || tree->parent.lock()->right == tree)));

    if (tree->is_leaf()) {
        strm << "Label:" << tree->id << std::endl;
    } else {
        strm << "Node" << std::endl;
    }
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

std::ostream &operator<<(std::ostream &strm, const std::shared_ptr<Tree> tree) {
    strm << "Dump Tree:" << std::endl;
    d_print_tree_rec(strm, tree, 1);
    return strm << std::endl;
}
