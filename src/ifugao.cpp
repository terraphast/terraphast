#include "ifugao.h"
#include "debug.h"
#include "terraces.h"

#include <sstream>
#include <iomanip>

TreeList FindAllRootedTrees::add_leaf_to_tree(const Tree &current_tree,
                                              const LeafPtr &leaf) {
    TreeList result;
    
    if (!current_tree->is_leaf()) {
        auto inner = std::static_pointer_cast<InnerNode>(current_tree);
        
        // add leaf left
        auto result_left = add_leaf_to_tree(inner->left, leaf);
        for(auto left : result_left) {
            // for each new combination left create tree with old right subtree
            result.push_back(std::make_shared<InnerNode>(left, inner->right));
        }
        
        // add leaf right
        auto result_right = add_leaf_to_tree(inner->right, leaf);
        for(auto right : result_right) {
            // for each new combination left create tree with old right subtree
            result.push_back(std::make_shared<InnerNode>(inner->left, right));
        }
        
        // add leaf above
        result.push_back(std::make_shared<InnerNode>(inner, leaf));
    } else {
        // current_tree is Leaf
        result.push_back(std::make_shared<InnerNode>(leaf, current_tree));
    }
    
    return result;
}

TreeList FindAllRootedTrees::get_all_binary_trees(LeafSet &leaves) {
    assert(leaves.size() > 0);
    
    TreeList result;
    
    leaf_number leaf_id = leaves.pop();
    const auto leaf = std::make_shared<Leaf>(leaf_id);
    if (leaves.size() == 0) {
        result.push_back(leaf);
    } else {
        for (const auto& t : get_all_binary_trees(leaves)) {
            auto new_trees = add_leaf_to_tree(t, leaf);
            result.insert(result.end(), new_trees.begin(), new_trees.end());
        }
    }
    
    return result;
}

TreeList FindAllRootedTrees::merge_subtrees(const TreeList &left,
                                            const TreeList &right){
    TreeList merged_trees;
    
    assert(left.size() != 0);
    assert(right.size() != 0);
    
    for (const auto& l : left) {
        for (const auto& r : right) {
            merged_trees.push_back(std::make_shared<InnerNode>(l, r));
        }
    }

    assert(merged_trees.size() == left.size() * right.size());
    return merged_trees;
}

std::vector<constraint> find_constraints(const LeafSet &leaves,
                                         const std::vector<constraint> &constraints) {

    std::vector<constraint> valid_constraints;

    for (constraint cons : constraints) {
        if (leaves.contains(cons.smaller_left)
                && leaves.contains(cons.smaller_right)
                && leaves.contains(cons.bigger)) {
                // constraint is valid on leaf set
                valid_constraints.push_back(cons);
        }
    }
    return valid_constraints;
}

std::string missing_data_to_nexus(const missingData* m) {

    std::stringstream ss;
    size_t max_len = 0;
    for (size_t i = 0; i < m->numberOfSpecies; i++) {
        if(strlen(m->speciesNames[i]) > max_len) {
            max_len = strlen(m->speciesNames[i]);
        }
    }

    ss << "#NEXUS\n";
    ss << "Begin data;\n";
    ss << "    Dimensions ntax=" << m->numberOfSpecies << " nchar=" << m->numberOfPartitions << ";\n";
    ss << "    Format datatype=dna gap=-;\n";
    ss << "    Matrix\n";
    for (size_t i = 0; i < m->numberOfSpecies; i++) {
        ss << std::left << std::setw(static_cast<int>(max_len+1)) << m->speciesNames[i];
        for (size_t j = 0; j < m->numberOfPartitions; j++) {
            ss << (getDataMatrix(m, i, j)>0 ? 'A' : '-');
        }
        ss << std::endl;
    }
    ss << "\t;\n";
    ss << "End;\n";
    ss << "\n";

    ss << "BEGIN SETS;\n";
    for (size_t j = 0; j < m->numberOfPartitions; j++) {
        ss << "\tCHARSET  P" << std::to_string(j+1) << " = "
             << std::to_string(j+1) << "-" << std::to_string(j+1) << ";\n";
    }
    ss << "END;\n";
    return ss.str();
}

