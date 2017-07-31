#include "types/tree.h"

/********************/
/*** Node methods ***/
/********************/
std::string Node::to_newick_string(const label_mapper &id_to_label) const {
    StringStream ss;
    this->to_newick_string(ss, id_to_label);
    ss << ";";
    return ss.to_string();
}

std::string Node::to_compressed_newick_string(
        const label_mapper &id_to_label) const {
    StringStream ss;
    this->to_newick_string(ss, id_to_label, true);
    ss << ";";
    return ss.to_string();
}

void Node::print_newick_string(FILE *file,
                               const label_mapper &id_to_label) const {
    FileStream fs(file);
    this->to_newick_string(fs, id_to_label);
}

void Node::print_compressed_newick_string(
            FILE *file, const label_mapper &id_to_label) const {
    //TODO
}
std::vector<constraint> Node::extract_constraints() const {
    std::vector<constraint> constraints;
    if (!this->is_leaf()) {
        this->get_constraints(constraints);
    }
    
    return constraints;
}

/********************/
/*** Leaf methods ***/
/********************/
void Leaf::to_newick_string(Stream &stream,
                            const label_mapper &id_to_label,
                            bool compressed /* = false */,
                            bool with_root /* = false */) const {
    stream << id_to_label[this->leaf_id];
};

/*************************/
/*** InnerNode methods ***/
/*************************/
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
                                 left_most_leaf);
    } else if(right_leaf) {
        // left edge is an inner edge
        // recurse
        auto left_tuple = this->left->get_constraints(constraints);
        left_most_leaf = std::get<0>(left_tuple);
        right_most_leaf = this->right->get_leaf();
        
        // constraint for left inner edge
        constraints.emplace_back(left_most_leaf, std::get<1>(left_tuple),
                                 right_most_leaf);
    } else {
        // left and right edge are inner edges
        auto left_tuple = this->left->get_constraints(constraints);
        auto right_tuple = this->right->get_constraints(constraints);
        left_most_leaf = std::get<0>(left_tuple);
        right_most_leaf = std::get<1>(right_tuple);
        // constraint for left inner edge
        constraints.emplace_back(left_most_leaf, std::get<1>(left_tuple),
                                 right_most_leaf);
        // constraint for right inner edge
        constraints.emplace_back(std::get<0>(right_tuple), right_most_leaf,
                                 left_most_leaf);
    }
    
    return std::make_tuple(left_most_leaf, right_most_leaf);
}

void InnerNode::to_newick_string(Stream &stream,
                                 const label_mapper &id_to_label,
                                 bool compressed /* = false */,
                                 bool with_root /* = false */) const {
    stream << "(";
    if (with_root) {
        stream << id_to_label.root_label << ",";
    }
    this->left->to_newick_string(stream, id_to_label, compressed);
    stream << ",";
    this->right->to_newick_string(stream, id_to_label, compressed);
    stream << ")";
};

/****************************/
/*** UnrootedNode methods ***/
/****************************/
void UnrootedNode::to_newick_string(Stream &stream,
                                    const label_mapper &id_to_label,
                                    bool compressed /* = false */,
                                    bool with_root /* = false */) const {
    this->node->to_newick_string(stream, id_to_label, compressed, true);
}

/***************************************/
/*** AllLeafCombinationsNode methods ***/
/***************************************/
void AllLeafCombinationsNode::to_newick_string(
        Stream &stream, const label_mapper &id_to_label,
        bool compressed /* = false */, bool with_root /* = false */) const {
    if (with_root) {
        stream << "(" << id_to_label.root_label << ",";
    }
    if (this->leaves.size() == 1) {
        stream << id_to_label[leaves[0]];
    } else if (this->leaves.size() == 2) {
        stream << "(" << id_to_label[leaves[0]] << "," << id_to_label[leaves[1]]
           << ")";
    } else {
        assert(this->leaves.size() > 2);
        if (compressed) {
            stream << "{";
            stream << id_to_label[this->leaves[0]];
            for(size_t i = 1; i < this->leaves.size(); ++i) {
                stream << "," << id_to_label[this->leaves[i]];
            }
            stream << "}";
        } else {
            assert(0);
            // TODO generate all binary trees
            
        }
    }
    if (with_root) {
        stream << ")"; 
    }
}

/***************************************/
/*** AllTreeCombinationsNode methods ***/
/***************************************/
void AllTreeCombinationsNode::to_newick_string(
        Stream &stream, const label_mapper &id_to_label,
        bool compressed /* = false */, bool with_root /* = false */) const {
    this->nodes[0]->to_newick_string(stream, id_to_label,
                                     compressed, with_root);
    for (size_t i = 1; i < this->nodes.size(); ++i) {
        stream << "|";
        this->nodes[i]->
                to_newick_string(stream, id_to_label, compressed, with_root);
    }
}
