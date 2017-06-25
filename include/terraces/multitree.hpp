#ifndef MULTITREE_HPP
#define MULTITREE_HPP

#include "trees.hpp"

namespace terraces {

enum class multitree_node_type {
	base_single_leaf,
	base_two_leaves,
	base_unconstrained,
	inner_node,
	alternative_array,
	alternative_list,
	alternative_list_node
};

struct multitree_node;

namespace multitree_nodes {
struct leaves {
	index left_leaf;
	index right_leaf;
};
struct unconstrained {
	index* begin;
	index* end;
};
struct inner_node {
	multitree_node* left;
	multitree_node* right;
};
struct alternative_array {
	multitree_node* begin;
	multitree_node* end;
};
struct alternative_list {
	index size;
	multitree_node* begin;
};
struct alternative_list_node {
	multitree_node* data;
	multitree_node* next;
};
}

struct multitree_node {
	multitree_node_type type;
	union {
		index single_leaf;
		multitree_nodes::leaves two_leaves;
		multitree_nodes::unconstrained unconstrained;
		multitree_nodes::inner_node inner_node;
		multitree_nodes::alternative_array alternative_array;
		multitree_nodes::alternative_list alternative_list;
		multitree_nodes::alternative_list_node alternative_list_node;
	};

	multitree_node* as_single_leaf(index i) {
		type = multitree_node_type::base_single_leaf;
		single_leaf = i;
		return this;
	}
	multitree_node* as_two_leaves(index i, index j) {
		type = multitree_node_type::base_two_leaves;
		two_leaves = {i, j};
		return this;
	}
	multitree_node* as_unconstrained(index* begin, index* end) {
		type = multitree_node_type::base_unconstrained;
		unconstrained = {begin, end};
		return this;
	}
	multitree_node* as_inner_node(multitree_node* left, multitree_node* right) {
		type = multitree_node_type::inner_node;
		inner_node = {left, right};
		return this;
	}
	multitree_node* as_alternative_array(multitree_node* begin, multitree_node* end) {
		type = multitree_node_type::alternative_array;
		alternative_array = {begin, end};
		return this;
	}
	multitree_node* as_alternative_list(index size) {
		type = multitree_node_type::alternative_list;
		alternative_list = {size, nullptr};
		return this;
	}
	multitree_node* as_alternative_list_node(multitree_node* data) {
		type = multitree_node_type::alternative_list_node;
		alternative_list_node = {data, nullptr};
		return this;
	}
};

struct newick_multitree_t {
	const multitree_node* root;
	const name_map* names;
};

std::ostream& operator<<(std::ostream& stream, newick_multitree_t tree);

inline newick_multitree_t as_newick(const multitree_node* root, const name_map& names) {
	return {root, &names};
}

} // namespace terraces

#endif // MULTITREE_HPP
