#include<vector>
#include<string>
#include<iostream>

#include <terraces/trees.hpp>

namespace terraces{

tree append_subtree(tree& t,
                                        index tree_index,
                                        index tree_node_ref,
                                        tree& subtree,
                                        index subtree_index,
                                        index subtree_node_index
                                        ) {
    (void) tree_index;
    (void) tree_node_ref;
    (void) subtree_index;
    (void) subtree_node_index;
    // TODO: Assert that every identifier is unique
    // TODO: Optimize by using moves
    auto out = t;
    out.insert(out.end(), subtree.begin(), subtree.end());
    /* TODO: Set references so that node tree_index and subtree_index poindex to
             each other via references tree_node_ref and subtree_node_ref.
    */
    return out;
}

} // namespace terraces
