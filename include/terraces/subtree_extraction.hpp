#ifndef SUBTREE_EXTRACTION_H
#define SUBTREE_EXTRACTION_H

#include <vector>

#include "bitmatrix.hpp"
#include "trees.hpp"

namespace terraces {

std::vector<tree> subtrees(const tree& t, const bitmatrix& occ);

} // namespace terraces

#endif // SUBTREE_EXTRACTION_H
