#include "types/tree.h"

#include <sstream>

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

std::shared_ptr<Tree> Tree::root() {
    auto parent = this->parent.lock();
    if (parent == nullptr) {
        return shared_from_this();
    }

    return parent->root();
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

std::shared_ptr<Tree> Tree::deep_copy(std::map<std::shared_ptr<Tree>, std::shared_ptr<Tree>> &cover_map) {
    if (cover_map.count(shared_from_this()) > 0) {
        return cover_map[shared_from_this()];
    }

    auto node = std::make_shared<Tree>();
    node->id = this->id;
    cover_map[node] = node;
    cover_map[shared_from_this()] = node;

    if(this->left != nullptr) {
        node->left = this->left->deep_copy(cover_map);
    }
    if(this->right != nullptr) {
        node->right = this->right->deep_copy(cover_map);
    }
    auto parent  = this->parent.lock();
    if(parent != nullptr) {
        node->parent = parent->deep_copy(cover_map);
    }
    return node;
}

std::tuple<std::shared_ptr<Tree>, std::shared_ptr<Tree>> Tree::deep_copy() {
    std::map<std::shared_ptr<Tree>, std::shared_ptr<Tree>> cover_map;
    auto result = this->deep_copy(cover_map);
    return std::make_tuple(result, result->root());
}
