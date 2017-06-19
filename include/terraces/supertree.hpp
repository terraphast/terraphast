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
  * @brief Remaps contstraints, names and root to leaf-based indexing (remove inner nodes).
  * @returns the number of leaves.
  */
index remap_to_leaves(const tree& t, constraints& c, name_map& names, index& root);

class tree_master {
private:
public:
	mpz_class count_supertree(index num_leaves, const constraints&, index root);
	mpz_class count_supertree(index count, const constraints&);
};

} // namespace terraces

#endif
