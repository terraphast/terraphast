#ifndef TERRACES_SUPERTREE_HPP
#define TERRACES_SUPERTREE_HPP

#include <cstdint>
#include <string>
#include <tuple>

#include <gmpxx.h>

#include "constraints.hpp"
#include "trees.hpp"

namespace terraces {

/**
  * Remaps contstraints, names and root to leaf-based indexing (remove inner nodes).
  * \returns the number of leaves.
  */
index remap_to_leaves(const tree& t, constraints& c, name_map& names, index& root);

} // namespace terraces

#endif
