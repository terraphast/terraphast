#include "constraints.h"

std::vector<constraint> extract_constraints_from_tree(
        const std::shared_ptr<Tree> supertree) {
    std::vector<constraint> constraints;

    extract_constraints_from_tree_rec(supertree, constraints);

    return constraints;
}

std::vector<constraint> find_constraints(const LeafSet &leaves,
                                         const std::vector<constraint> &constraints) {
    std::vector<constraint> valid_constraints;

    for (constraint cons : constraints) {
        if (cons.smaller_left == cons.bigger_left) {
            if (leaves.contains(cons.smaller_left) != leaves.end()
                && leaves.contains(cons.smaller_right) != leaves.end()
                && leaves.contains(cons.bigger_right) != leaves.end()) {
                // constraint is valid on leaf set
                valid_constraints.push_back(cons);
            }
        } else {
            // smaller_right == bigger_right
            if (leaves.contains(cons.smaller_left) != leaves.end()
                && leaves.contains(cons.smaller_right) != leaves.end()
                && leaves.contains(cons.bigger_left) != leaves.end()) {
                // constraint is valid on leaf set
                valid_constraints.push_back(cons);
            }
        }
    }
    return valid_constraints;
}


static std::tuple<size_t, size_t> extract_constraints_from_tree_rec(
        const InnerNodePtr node, std::vector<constraint> &constraints) {
    //TODO adopt to new tree structure
    assert(node != nullptr);
    // (l,r) of the left child node
    size_t l_left_most;
    size_t l_right_most;
    std::tie(l_left_most, l_right_most) = extract_constraints_from_tree_rec(
            node->left, constraints);

    // (l,r) of the right child node
    size_t r_left_most;
    size_t r_right_most;
    std::tie(r_left_most, r_right_most) = extract_constraints_from_tree_rec(
            node->right, constraints);

    if (l_left_most != l_right_most) {
        constraint c;
        c.smaller_left = l_left_most;
        c.smaller_right = l_right_most;
        c.bigger_left = l_left_most;
        c.bigger_right = r_right_most;
        constraints.push_back(c);
    }

    if (r_left_most != r_right_most) {
        constraint c;
        c.smaller_left = r_left_most;
        c.smaller_right = r_right_most;
        c.bigger_left = l_left_most;
        c.bigger_right = r_right_most;
        constraints.push_back(c);
    }

    return std::make_tuple(l_left_most, r_right_most);
}

static std::tuple<size_t, size_t> extract_constraints_from_tree_rec(
        const LeafPtr node, std::vector<constraint> &constraints) {
    assert(node != nullptr);
    
    size_t leaf = node.leaf_id;
    return std::make_tuple(leaf, leaf);
}
