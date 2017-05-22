#ifndef TERRACES_SUPERTREE_HPP
#define TERRACES_SUPERTREE_HPP

#include "bipartitions.hpp"
#include "constraints.hpp"

namespace terraces {

struct supertree_node {
	std::vector<index> leaves;
	struct supertree_node* left;
	struct supertree_node* right;
};

/**
 * Create a new node without any subnodes
 */
struct supertree_node* new_node(std::vector<index>);

/**
 * Return a string representing the given supertree via preorder traversal
 */
std::string pretty_preorder(struct supertree_node*);

/**
 * Map the given constraints according to the given vector of indices.
 * If the vector of indices is {2, 5, 6} for example, we get the mapping 2->0, 5->1, 6->2.
 */
constraints map_constraints(std::vector<index>, constraints);

/**
 * Contrary function to map_constraints.
 */
std::vector<std::vector<index>> map_sets(std::vector<index>, std::vector<std::vector<index>>);

/**
 * Check if on a terrace given a number and a vector of constraints. Entry point for terrace checking.
 */
bool check_supertree(index, constraints);

/**
 * Check if on a terrace given a number and a vector of constraints.
 */
bool check_supertree(std::vector<index>, constraints);

/**
 * Count trees given a number and a vector of constraints. Entry point for tree counting.
 */
size_t count_supertree(index, constraints);

/**
 * Count trees.
 */
size_t count_supertree(std::vector<index>, constraints);

/**
 * Generate the supertree given a number and a vector of contraints. Entry point for tree enumeration.
 */
std::vector<struct supertree_node*> construct_supertree(index, constraints);

/**
 * Generate the supertree given a vector of indices and a vector of contraints. 
 */
std::vector<struct supertree_node*> construct_supertree(std::vector<index>, constraints);

} // namespace terraces

#endif
