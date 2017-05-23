#ifndef TERRACES_ROOTING_HPP
#define TERRACES_ROOTING_HPP

#include "trees.hpp"

namespace terraces {

std::ostream& operator<<(std::ostream& ss, const tree& t);

// Re-roots the given tree in-place. The given root_leaf will be the right 
// child of our new root, with the res tof the tree being the left subtree.
// NOTE that after re-rooting, the tree will no longer possess min-heap
// structure!
void reroot_inplace(tree& t, index root_leaf);
} // namespace terraces

#endif
