#ifndef MULTITREE_HPP
#define MULTITREE_HPP

#include "trees.hpp"

namespace terraces {

enum class multitree_node_type { base_leaves, base_unconstrained, inner_node, alternatives };

struct multitree_node {
	multitree_node_type type;
	union {
		struct {
			index left_leaf;
			index right_leaf;
		} leaves;
		struct {
			index* begin;
			index* end;
		} unconstrained;
		struct {
			multitree_node* left_child;
			multitree_node* right_child;
		} inner_node;
		struct {
			multitree_node* begin;
			multitree_node* end;
		} alternatives;
	};

	~multitree_node() {
		switch (type) {
		case multitree_node_type::base_leaves:
			break;
		case multitree_node_type::base_unconstrained:
			delete[](unconstrained.begin);
			break;
		case multitree_node_type::inner_node:
			delete (inner_node.left_child);
			delete (inner_node.right_child);
			break;
		case multitree_node_type::alternatives:
			delete[](alternatives.begin);
			break;
		}
	}
};

} // namespace terraces

#endif // MULTITREE_HPP
