#ifndef MULTITREE_ITERATOR_H
#define MULTITREE_ITERATOR_H

#include "unconstrained_enumerator.hpp"
#include <terraces/multitree.hpp>

namespace terraces {

struct multitree_iterator_choicepoint {
	const multitree_node* alternatives;
	const multitree_node* current;

	multitree_iterator_choicepoint() : alternatives{nullptr}, current{nullptr} {}

	multitree_iterator_choicepoint(const multitree_node* node) {
		if (node->type == multitree_node_type::alternative_array) {
			const auto aa = node->alternative_array;
			alternatives = aa.num_alternatives() > 1 ? node : nullptr;
			current = aa.begin;
		} else {
			alternatives = nullptr;
			current = node;
		}
	}

	bool has_choices() const { return alternatives != nullptr; }

	bool has_next() const {
		return has_choices() && (current + 1) != alternatives->alternative_array.end;
	}
	void next() {
		assert(has_next());
		++current;
	}
};

class multitree_iterator {
private:
	multitree_node* m_root_node;
	tree* m_tree;
	permutation* m_leaves;
	std::vector<index> m_choice_points;
	std::vector<multitree_iterator_choicepoint> m_choices;

	index init_subtree(index subtree_root, index single_leaf);
	index init_subtree(index subtree_root, multitree_nodes::two_leaves two_leaves);
	index init_subtree(index subtree_root, multitree_nodes::inner_node inner);
	index init_subtree(index subtree_root, multitree_nodes::unconstrained unconstrained);
	index init_subtree(index subtree_root);

public:
	multitree_iterator(const multitree_node* root, tree& tree, permutation& leaves);
	void next();
	bool has_next() const;
};

} // namespace terraces

#endif // MULTITREE_ITERATOR_H
