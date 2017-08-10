#ifndef TERRACES_ROOTING_HPP
#define TERRACES_ROOTING_HPP

#include "trees.hpp"

namespace terraces {

/**
 * Re-roots the given tree in-place.
 * The given \p root_leaf will be placed to the right of the root.
 * child of our new root, with the rest of the tree being the left subtree.
 */
void reroot_inplace(tree& t, index root_leaf);

} // namespace terraces

#endif
