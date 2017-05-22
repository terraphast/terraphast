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

struct supertree_node* new_node(std::vector<index>);

std::string pretty_preorder(struct supertree_node*);

constraints map_constraints(std::vector<index>, constraints);

std::vector<std::vector<index>> map_sets(std::vector<index>, std::vector<std::vector<index>>);

std::vector<struct supertree_node*> construct_supertree(std::vector<index>, constraints);

} // namespace terraces

#endif
