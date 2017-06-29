#include <terraces/unconstrained_enumerator.hpp>

namespace terraces {

index unconstrained_tree_iterator::local_lchild(index i) const {
	index result = m_tree[i + m_root].lchild();
	assert(result > i + m_root);
	return result - m_root;
}
index unconstrained_tree_iterator::local_rchild(index i) const {
	index result = m_tree[i + m_root].rchild();
	assert(result > i + m_root);
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

bool unconstrained_tree_iterator::is_local_left_child(index i) const {
	return local_rsibling(i) != i;
}

index unconstrained_tree_iterator::init_subtree(index i) {
	assert(m_bips[i].num_leaves() > 0);
	auto& bip = m_bips[i];
	auto& cp = m_choice_points[i];
	auto global_i = i + m_root;
	auto& node = m_tree[global_i];
	if (bip.num_leaves() == 1) {
		m_leaves[global_i] = m_unconstrained.begin[bip.leftmost_leaf()];
		node.lchild() = none;
		node.rchild() = none;
		return (cp = none);
	} else {
		auto lbip = small_bipartition{bip.left_mask()};
		auto rbip = small_bipartition{bip.right_mask()};
		index loffset = 1;
		index roffset = loffset + 2 * lbip.num_leaves() - 1;
		auto left = i + loffset;
		auto right = i + roffset;
		node.lchild() = global_i + loffset;
		node.rchild() = global_i + roffset;
		m_bips[left] = lbip;
		m_bips[right] = rbip;
		m_tree[node.lchild()].parent() = global_i;
		m_tree[node.rchild()].parent() = global_i;
		auto l_cp = init_subtree(left);
		auto r_cp = init_subtree(right);
		auto p_cp = bip.has_next() ? i : none;
		return (cp = (l_cp != none ? l_cp : (r_cp != none ? r_cp : p_cp)));
	}
}

unconstrained_tree_iterator::unconstrained_tree_iterator(const multitree_nodes::unconstrained node,
                                                         tree& tree, std::vector<index>& leaves,
                                                         index root)
        : m_unconstrained{node}, m_num_leaves{node.num_leaves()}, m_tree{tree}, m_leaves{leaves},
          m_root{root}, m_choice_points(2 * m_num_leaves - 1, 0),
          m_bips(m_choice_points.size(), small_bipartition::full_set(m_num_leaves)) {
	assert(m_num_leaves > 2);
	init_subtree(0);
}

void unconstrained_tree_iterator::next() {
	auto& cp = m_choice_points[0];
	assert(cp != none);
	if (m_bips[cp].has_next()) {
		m_bips[cp].next();
	} else {
		bool from_left;
		while (cp != 0 &&
		       ((is_local_left_child(cp) && m_choice_points[local_rsibling(cp)] == none) ||
		        (!is_local_left_child(cp) && (m_choice_points[cp] == none ||
		                                      !m_bips[m_choice_points[cp]].has_next())))) {
			from_left = is_local_left_child(cp);
			cp = local_parent(cp);
		}
		assert(cp != 0);
		assert(from_left);
		auto left = local_lchild(cp);
		auto right = m_choice_points[local_rchild(cp)];
		assert(m_bips[right].has_next());
		m_bips[left].reset();
		m_bips[right].next();
		auto l_cp = init_subtree(left);
		auto r_cp = init_subtree(right);
		// restore choice points along path to cp
		while (right != cp) {
			m_choice_points[right] = r_cp;
			right = local_parent(right);
		}
		auto it = cp;
		while (it != 0) {
			m_choice_points[it] = l_cp;
			it = local_parent(it);
		}
		// TODO if we have no choice point on the left side, this fails!
		cp = local_lchild(cp);
	}

	init_subtree(m_choice_points[0]);
}

bool unconstrained_tree_iterator::has_next() const { return m_choice_points[0] != none; }

} // namespace terraces
