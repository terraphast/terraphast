#ifndef SUPERTREE_HELPERS_HPP
#define SUPERTREE_HELPERS_HPP

#include <terraces/constraints.hpp>
#include <terraces/fast_set.hpp>
#include <terraces/trees.hpp>
#include <terraces/union_find.hpp>

namespace terraces {

/** Removes all constraints from c_occ that don't lie fully within the leave set. */
fast_index_set filter_constraints(const fast_index_set& leaves, const fast_index_set& c_occ,
                                  const constraints& c);

/** Returns a union-find representation of the sets retrieved after applying all constraints from
 * c_occ. */
union_find apply_constraints(const fast_index_set& leaves, const fast_index_set& c_occ,
                             const constraints& c);

/** Maps the given constraints to leaf indices. */
constraints map_constraints(const fast_index_set& leaves, const constraints& cs);

/** Returns a set containing all leaves of the given tree. */
fast_index_set leave_occ(const tree& tree);

} // namespace terraces

#endif // SUPERTREE_HELPERS_HPP
