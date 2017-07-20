#include "types/tree.h"

/********************/
/*** Node methods ***/
/********************/
std::string Node::to_newick_string(const label_mapper &id_to_label) const {
    std::stringstream ss;
    this->to_newick_string(ss, id_to_label);
    ss << ";";
    return ss.str();
}

std::vector<constraint> Node::extract_constraints() const {
    std::vector<constraint> constraints;
    
    this->get_constraints(constraints);
    
    return constraints;
}

/********************/
/*** Leaf methods ***/
/********************/
void Leaf::to_newick_string(std::stringstream &ss,
                            const label_mapper &id_to_label) const {
    ss << id_to_label[this->leaf_id];
}

std::tuple<leaf_number, leaf_number> Leaf::get_constraints(
        std::vector<constraint> &constraints) const {
    // may not be called
    assert(false);
    return std::make_tuple(0, 0);
}

/*************************/
/*** InnerNode methods ***/
/*************************/
void InnerNode::to_newick_string(std::stringstream &ss,
                                 const label_mapper &id_to_label) const {
    ss << "(";
    this->left->to_newick_string(ss, id_to_label);
    ss << ",";
    this->right->to_newick_string(ss, id_to_label);
    ss << ")";
}

std::tuple<leaf_number, leaf_number> InnerNode::get_constraints(
        std::vector<constraint> &constraints) const {
    leaf_number left_most_leaf;
    leaf_number right_most_leaf;
    
    bool left_leaf = this->left->is_leaf();
    bool right_leaf = this->right->is_leaf(); 
    
    if (left_leaf && right_leaf) {
        // no inner edge, just return values
        left_most_leaf = this->left->get_leaf();
        right_most_leaf = this->right->get_leaf();
    } else if(left_leaf) {
        // right edge is an inner edge
        left_most_leaf = this->left->get_leaf();
        // recurse
        auto right_tuple = this->right->get_constraints(constraints);
        right_most_leaf = std::get<1>(right_tuple);
        
        // constraint for right inner edge
        constraints.emplace_back(std::get<0>(right_tuple), right_most_leaf,
                                 left_most_leaf, right_most_leaf);
    } else if(right_leaf) {
        // left edge is an inner edge
        // recurse
        auto left_tuple = this->left->get_constraints(constraints);
        left_most_leaf = std::get<0>(left_tuple);
        right_most_leaf = this->right->get_leaf();
        
        // constraint for left inner edge
        constraints.emplace_back(left_most_leaf, std::get<1>(left_tuple),
                                 left_most_leaf, right_most_leaf);
    } else {
        // left and right edge are inner edges
        auto left_tuple = this->left->get_constraints(constraints);
        auto right_tuple = this->right->get_constraints(constraints);
        left_most_leaf = std::get<0>(left_tuple);
        right_most_leaf = std::get<1>(right_tuple);
        // constraint for left inner edge
        constraints.emplace_back(left_most_leaf, std::get<1>(left_tuple),
                                 left_most_leaf, right_most_leaf);
        // constraint for right inner edge
        constraints.emplace_back(std::get<0>(right_tuple), right_most_leaf,
                                 left_most_leaf, right_most_leaf);
    }
    
    return std::make_tuple(left_most_leaf, right_most_leaf);
}

/****************************/
/*** UnrootedNode methods ***/
/****************************/
void UnrootedNode::to_newick_string(std::stringstream &ss,
                                    const label_mapper &id_to_label) const {
    ss << "(";
    this->elem1->to_newick_string(ss, id_to_label);
    ss << ",";
    this->elem2->to_newick_string(ss, id_to_label);
    ss << ",";
    this->elem3->to_newick_string(ss, id_to_label);
    ss << ")";
}

std::tuple<leaf_number, leaf_number> UnrootedNode::get_constraints(
        std::vector<constraint> &constraints) const {
    // may not be called
    assert(false);
    return std::make_tuple(0, 0);
}

/***************************************/
/*** AllLeafCombinationsNode methods ***/
/***************************************/
void AllLeafCombinationsNode::to_newick_string(
        std::stringstream &ss, const label_mapper &id_to_label) const {
    ss << "{";
    ss << id_to_label[this->leaves[0]];// guaranteed to exist (see init assert)
    for(size_t i = 1; i < this->leaves.size(); ++i) {
        ss << "," << id_to_label[this->leaves[i]];
    }
    ss << "}";
}

std::tuple<leaf_number, leaf_number> AllLeafCombinationsNode::get_constraints(
        std::vector<constraint> &constraints) const {
    // may not be called
    assert(false);
    return std::make_tuple(0, 0);
}

/***************************************/
/*** AllTreeCombinationsNode methods ***/
/***************************************/
void AllTreeCombinationsNode::to_newick_string(
        std::stringstream &ss, const label_mapper &id_to_label) const {
    this->trees[0]->to_newick_string(ss, id_to_label);
    for (size_t i = 1; i < this->trees.size(); ++i) {
        ss << "|";
        this->trees[i]->to_newick_string(ss, id_to_label);
    }
}

std::tuple<leaf_number, leaf_number> AllTreeCombinationsNode::get_constraints(
        std::vector<constraint> &constraints) const {
    // may not be called
    assert(false);
    return std::make_tuple(0, 0);
}

