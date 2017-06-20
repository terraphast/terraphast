#ifndef TERRACES_SUPERTREE_HPP
#define TERRACES_SUPERTREE_HPP

#include "constraints.hpp"
#include "fast_set.hpp"
#include "trees.hpp"
#include "union_find.hpp"
#include <gmpxx.h>
#include <string>
#include <tuple>

namespace terraces {

/**
  * Remaps contstraints, names and root to leaf-based indexing (remove inner nodes).
  * \returns the number of leaves.
  */
index remap_to_leaves(const tree& t, constraints& c, name_map& names, index& root);

/**
 * Counts the number of trees compatible with the given constraints, while keeping the root fixed.
 * \param num_leaves The number of leaves of the tree
 * \param constraints The constraints on the leaves. The constraints must only contain valid leaf
 * indices.
 * \param root_leaf The fixed 'root leaf' which will be placed to the right of the root
 * to avoid isomorphic, differently rooted trees.
 * @return the number of trees.
 */
mpz_class count_supertree(index num_leaves, const constraints& constraints, index root_leaf);

/**
 * Counts the number of trees compatible with the given constraints.
 * This method might count a tree multiple times if it has a valid isomorphic representation.
 * \param num_leaves The number of leaves of the tree
 * \param constraints The constraints on the leaves. The constraints must only contain valid leaf
 * indices.
 * @return the number of trees.
 */
mpz_class count_supertree(index num_leaves, const constraints& constraints);

} // namespace terraces

#endif
