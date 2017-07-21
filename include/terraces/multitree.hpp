#ifndef MULTITREE_HPP
#define MULTITREE_HPP

#include "trees.hpp"

#include <gmpxx.h>

namespace terraces {

enum class multitree_node_type {
	base_single_leaf,
	base_two_leaves,
	base_unconstrained,
	inner_node,
	alternative_array,
	unexplored,
};

struct multitree_node;

namespace multitree_nodes {
struct two_leaves {
	index left_leaf;
	index right_leaf;
};
struct unconstrained {
	index* begin;
	index* end;
	index num_leaves() const;
};
struct inner_node {
	multitree_node* left;
	multitree_node* right;
};
struct alternative_array {
	multitree_node* begin;
	multitree_node* end;
	index num_alternatives() const;
};
struct unexplored {
	index* begin;
	index* end;
	index num_leaves() const;
};
}

struct multitree_node {
	multitree_node_type type;
	index num_leaves;
	mpz_class num_trees;
	union {
		index single_leaf;
		multitree_nodes::two_leaves two_leaves;
		multitree_nodes::unconstrained unconstrained;
		multitree_nodes::inner_node inner_node;
		multitree_nodes::alternative_array alternative_array;
		multitree_nodes::unexplored unexplored;
	};

	multitree_node* as_single_leaf(index i) {
		type = multitree_node_type::base_single_leaf;
		single_leaf = i;
		num_leaves = 1;
		num_trees = 1;
		return this;
	}
	multitree_node* as_two_leaves(index i, index j) {
		type = multitree_node_type::base_two_leaves;
		two_leaves = {i, j};
		num_leaves = 2;
		num_trees = 1;
		return this;
	}
	multitree_node* as_unconstrained(std::pair<index*, index*> range) {
		auto begin = range.first;
		auto end = range.second;
		type = multitree_node_type::base_unconstrained;
		unconstrained = {begin, end};
		num_leaves = unconstrained.num_leaves();
		num_trees = count_unrooted_trees<index>(num_leaves);
		return this;
	}
	multitree_node* as_inner_node(multitree_node* left, multitree_node* right) {
		type = multitree_node_type::inner_node;
		inner_node = {left, right};
		num_leaves = left->num_leaves + right->num_leaves;
		num_trees = left->num_trees * right->num_trees;
		return this;
	}
	multitree_node* as_alternative_array(multitree_node* begin, index leaves) {
		type = multitree_node_type::alternative_array;
		alternative_array = {begin, begin};
		num_leaves = leaves;
		num_trees = 0;
		return this;
	}
	multitree_node* as_unexplored(std::pair<index*, index*> range) {
		auto begin = range.first;
		auto end = range.second;
		type = multitree_node_type::unexplored;
		unexplored = {begin, end};
		num_leaves = unexplored.num_leaves();
		num_trees = 0;
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

inline index multitree_nodes::alternative_array::num_alternatives() const {
	return (index)(end - begin);
}
inline index multitree_nodes::unexplored::num_leaves() const { return (index)(end - begin); }
inline index multitree_nodes::unconstrained::num_leaves() const { return (index)(end - begin); }

} // namespace terraces

#endif // MULTITREE_HPP
