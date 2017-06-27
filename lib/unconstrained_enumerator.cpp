#include <terraces/unconstrained_enumerator.hpp>

namespace terraces {

index unconstrained_tree_iterator::local_lchild(index i) const {
	index result = m_tree[i + m_root].lchild();
	assert(result > i + m_root);
	return result - m_root;
}
index unconstrained_tree_iterator::local_rchild(index i) const {
	index result = m_tree[i + m_root].rchild();
	assert(result > m_choice_pioint + m_root);
	return result - m_root;
}
index unconstrained_tree_iterator::local_rsibling(index i) const {
	return local_rchild(local_parent(i));
}
index unconstrained_tree_iterator::local_parent(index i) const {
	index result = m_tree[i + m_root].parent();
	assert(result < i + m_root && result >= m_root);
	return result - m_root;
}

bool unconstrained_tree_iterator::is_local_leaf(index i) const {
	return is_leaf(m_tree[m_root + i]);
}

void unconstrained_tree_iterator::init_subtree(index i) {
	assert(m_bips[i].num_leaves() > 0);
	auto& bip = m_bips[i];
	auto global_i = i + m_root;
	auto& node = m_tree[global_i];
	if (bip.num_leaves() == 1) {
		m_leaves[global_i] = m_unconstrained.begin[bip.leftmost_leaf()];
		node.lchild() = none;
		node.rchild() = none;
	} else {
		auto lbip = small_bipartition{bip.left_mask()};
		auto rbip = small_bipartition{bip.right_mask()};
		node.lchild() = global_i + 1;
		node.rchild() = global_i + 1 + (2 * lbip.num_leaves() - 1);
		m_bips[node.lchild()] = lbip;
		m_bips[node.rchild()] = rbip;
		m_tree[node.lchild()].parent() = global_i;
		m_tree[node.rchild()].parent() = global_i;
		init_subtree(node.lchild());
		init_subtree(node.rchild());
	}
}

void unconstrained_tree_iterator::descend_subtree(index i) {
	m_choice_point = i;
	assert(!is_local_leaf(m_choice_point));
	// first descend left as long as we have multiple choices
	while (m_bip[local_lchild(m_choice_point)].num_leaves() > 2 &&
	       m_bip[local_lchild(m_choice_point)].is_valid()) {
		m_choice_point = local_lchild(m_choice_point);
		m_it_stack[++m_it_level] = false;
	}
	assert(!is_local_leaf(m_choice_point));
	assert(m_bip[m_choice_point].num_leaves() > 2);
	/* now we are at an inner node where we have
	 * only one possible subtree on the left */

	// then descend right as long as we have multiple choices
	while (m_bip[local_rchild(m_choice_point)].num_leaves() > 2 &&
	       m_bip[local_rchild(m_choice_point)].is_valid()) {
		m_choice_point = local_rchild(m_choice_point);
		m_it_stack[++m_it_level] = true;
	}
	assert(!is_local_leaf(m_choice_point));
	assert(m_bip[m_choice_point].num_leaves() > 2);
}

unconstrained_tree_iterator::unconstrained_tree_iterator(const multitree_nodes::unconstrained node,
                                                         tree& tree, std::vector<index>& leaves,
                                                         index root)
        : m_unconstrained{node}, m_num_leaves{node.num_leaves()}, m_tree{tree}, m_leaves{leaves},
          m_root{root}, m_choice_point{0}, m_it_stack{}, m_it_level{0},
          m_bips(2 * m_num_leaves - 1, small_bipartition::full_set(m_num_leaves)) {
	assert(m_num_leaves > 2);
	init_subtree(m_choice_point);
	descend_subtree(m_choice_point);
}

void unconstrained_tree_iterator::next() {
	assert(is_valid());
	m_bips[m_choice_point].next();
	if (m_bips[m_choice_point].is_valid()) {
		init_subtree(m_choice_point);
		descend_subtree(m_choice_point);
	} else {
		// TODO sentinels or stop at root
		while ((m_it_stack[m_it_level] && !m_bips[m_choice_point].is_valid()) ||
		       (!m_it_stack[m_it_level] &&
		        !m_bips[local_rsibling(m_choice_point)].is_valid())) {
			m_choice_point = local_parent(m_choice_point);
			--m_it_level;
		}
		m_bips[local_lchild(m_choice_point)].reset();
		m_bips[local_rchild(m_choice_point)].next();
		init_subtree(m_choice_point);
		descend_subtree(m_choice_point);
	}
	assert(is_valid() || m_choice_point == 0);
}

bool unconstrained_tree_iterator::is_valid() const { return m_bips[m_choice_point].is_valid(); }

} // namespace terraces
