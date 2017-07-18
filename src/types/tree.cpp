#include "types/tree.h"

#include <sstream>

std::shared_ptr<Tree> Tree::root() {
    auto parent = this->parent.lock();
    if (parent == nullptr) {
        return shared_from_this();
    }

    return parent->root();
}

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

void PartitionNode::to_newick_string(std::stringstream &ss,
                                     const std::vector<std::string> &ids_to_lables) const {
    this->partitions[0]->to_newick_string(ss, ids_to_lables);
    for (size_t i = 1; i < this->partitions.size(); i++) {
        ss << "|";
        this->partitions[i]->to_newick_string(ss, ids_to_lables);
    }
}

void LeafSetNode::to_newick_string(std::stringstream &ss,
                                   const std::vector<std::string> &ids_to_lables) const {

    if(this->leaves.size() == 1) {
        ss << ids_to_lables[*this->leaves.begin()];
        return;
    }
    char start_symbol = '{';
    char end_symbol = '}';
    if (this->leaves.size() == 2) {
        start_symbol = '(';
        end_symbol = ')';
    }
    ss << start_symbol;
    bool first = true;
    for(const leaf_number &elem : this->leaves) {
        if(first) {
            first = false;
        } else {
            ss << ",";
        }
        ss << ids_to_lables[elem];
    }
    ss << end_symbol;
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
