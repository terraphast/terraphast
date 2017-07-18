#include "multitree_iterator.hpp"

namespace terraces {

index combine_choice_points(index left, index right, index parent) {
	return left != none ? left : (right != none ? right : parent);
}

multitree_iterator::multitree_iterator(const multitree_node* root, tree& tree, permutation& leaves)
        : m_tree{&tree}, m_leaves{&leaves}, m_choice_points(tree.size(), none),
          m_choices(tree.size()) {
	m_choices[0] = {root};
	init_subtree(0);
}

index multitree_iterator::init_subtree(index i, index single_leaf) {
	auto& tree = *m_tree;
	auto& leaves = *m_leaves;
	tree[i].lchild() = none;
	tree[i].rchild() = none;
	leaves[i] = single_leaf;
	return none;
}

index multitree_iterator::init_subtree(index i, multitree_nodes::two_leaves two_leaves) {
	auto& tree = *m_tree;
	auto& leaves = *m_leaves;
	const auto l = i + 1;
	const auto r = i + 2;
	tree[i].lchild() = l;
	tree[i].rchild() = r;
	tree[l] = {i, none, none};
	tree[r] = {i, none, none};
	leaves[i] = none;
	leaves[l] = two_leaves.left_leaf;
	leaves[r] = two_leaves.right_leaf;
	return none;
}

index multitree_iterator::init_subtree(index i, multitree_nodes::unconstrained unconstrained) {
	unconstrained_tree_iterator(unconstrained, *m_tree, *m_leaves, i);
	return none;
}

index multitree_iterator::init_subtree(index i, multitree_nodes::inner_node inner) {
	auto& tree = *m_tree;
	auto& leaves = *m_leaves;
	const auto left = inner.left;
	const auto right = inner.right;
	const auto lindex = i + 1;
	const auto rindex = lindex + (2 * left->num_leaves - 1);
	tree[i].lchild() = lindex;
	tree[i].rchild() = rindex;
	tree[lindex].parent() = i;
	tree[rindex].parent() = i;
	m_choices[lindex] = {left};
	m_choices[rindex] = {right};
	leaves[i] = none;
	auto lcp = init_subtree(lindex);
	auto rcp = init_subtree(rindex);
	auto pcp = m_choices[i].has_next() ? i : none;
	return combine_choice_points(lcp, rcp, pcp);
}

index multitree_iterator::init_subtree(index i) {
	const auto mt_node = m_choices[i].current;
	auto& cp = m_choice_points[i];
	switch (mt_node->type) {
	case multitree_node_type::base_single_leaf: {
		assert(!m_choices[i].has_choices());
		return (cp = init_subtree(i, mt_node->single_leaf));
		break;
	}
	case multitree_node_type::base_two_leaves: {
		assert(!m_choices[i].has_choices());
		return (cp = init_subtree(i, mt_node->two_leaves));
		break;
	}
	case multitree_node_type::base_unconstrained: {
		assert(!m_choices[i].has_choices());
		return (cp = init_subtree(i, mt_node->unconstrained));
		break;
	}
	case multitree_node_type::inner_node: {
		return (cp = init_subtree(i, mt_node->inner_node));
		break;
	}
	case multitree_node_type::alternative_array: {
		assert(false && "Nested alternative_arrays");
		break;
	}
	case multitree_node_type::unexplored: {
		assert(false && "Must not use multitree_iterator with unexplored nodes");
		break;
	}
	}
	return cp;
}

} // namespace terraces
