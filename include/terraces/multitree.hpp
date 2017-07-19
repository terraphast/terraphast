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
	unexplored,
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
struct unexplored {
	index* begin;
	index* end;
};
}

struct multitree_node {
	multitree_node_type type;
	index num_leaves;
	index num_trees; // TODO use mpz_class? Maybe double?
	union {
		index single_leaf;
		multitree_nodes::leaves two_leaves;
		multitree_nodes::unconstrained unconstrained;
		multitree_nodes::inner_node inner_node;
		multitree_nodes::alternative_array alternative_array;
		multitree_nodes::unexplored unexplored;
	};
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
