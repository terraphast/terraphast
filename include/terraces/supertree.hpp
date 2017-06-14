#ifndef TERRACES_SUPERTREE_HPP
#define TERRACES_SUPERTREE_HPP

#include "constraints.hpp"
#include "fast_set.hpp"
#include "trees.hpp"
#include "union_find.hpp"

namespace terraces {

class tree_master {
private:
	fast_index_set filter_constraints(const fast_index_set& leaves, const fast_index_set& c_occ,
	                                  const constraints& c) const;

	union_find apply_constraints(const fast_index_set& leaves, const fast_index_set& c_occ,
	                             const constraints& c) const;

public:
	tree_master();

	size_t count_supertree(const tree&, const constraints&, index root);

	/**
	 * Count trees given a number and a vector of constraints. Entry point for tree counting.
	 */
	size_t count_supertree(index count, const constraints&);

	/**
	 * Count trees.
	 */
	size_t count_supertree(const fast_index_set&, const fast_index_set&, const constraints&);

	size_t check_supertree(const fast_index_set& leaves, const fast_index_set& in_c_occ,
	                       const constraints& in_c, index init_number);
};

} // namespace terraces

#endif
