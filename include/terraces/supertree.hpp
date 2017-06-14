#ifndef TERRACES_SUPERTREE_HPP
#define TERRACES_SUPERTREE_HPP

#include "constraints.hpp"
#include "fast_set.hpp"
#include "trees.hpp"
#include <string>

namespace terraces {

class tree_master {
public:
	tree_master();

	size_t count_supertree(const tree&, const constraints&);

	/**
	 * Count trees given a number and a vector of constraints. Entry point for tree counting.
	 */
	size_t count_supertree(index count, const constraints&);

	/**
	 * Count trees.
	 */
	size_t count_supertree(const fast_index_set&, const fast_index_set&, const constraints&);

	std::string printable_tree_representation(const fast_index_set&, const name_map&);
	std::string printable_leaf_pair_representation(const fast_index_set&, const name_map&);

	/*
	 * Enumerate trees. Working copy.
	 */
	counted_supertree enumerate_supertrees(const fast_index_set&, const fast_index_set&,
	                                       const constraints&, const name_map&);
};

} // namespace terraces

#endif
