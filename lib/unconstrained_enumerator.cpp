#include <terraces/unconstrained_enumerator.hpp>

namespace terraces {

index unconstrained_tree_iterator::local_lchild(index i) const {
	index result = (*m_tree)[i + m_root].lchild();
	assert(result > i + m_root);
	return result - m_root;
}
index unconstrained_tree_iterator::local_rchild(index i) const {
	index result = (*m_tree)[i + m_root].rchild();
	assert(result > i + m_root);
	return result - m_root;
}
index unconstrained_tree_iterator::local_rsibling(index i) const {
	return local_rchild(local_parent(i));
}
index unconstrained_tree_iterator::local_parent(index i) const {
	index result = (*m_tree)[i + m_root].parent();
	assert(result < i + m_root && result >= m_root);
	return result - m_root;
}

bool unconstrained_tree_iterator::is_local_leaf(index i) const {
	return is_leaf((*m_tree)[m_root + i]);
}

bool unconstrained_tree_iterator::is_local_root(index i) const { return i == 0; }

bool unconstrained_tree_iterator::is_local_left_child(index i) const {
	return local_rsibling(i) != i;
}

bool unconstrained_tree_iterator::subtree_has_next(index i) const {
	return m_choice_points[i] != none && m_bips[m_choice_points[i]].has_next();
}

index unconstrained_tree_iterator::init_subtree(index i) {
	assert(m_bips[i].num_leaves() > 0);
	const auto& bip = m_bips[i];
	auto& cp = m_choice_points[i];
	const auto global_i = i + m_root;
	auto& node = (*m_tree)[global_i];
	if (bip.num_leaves() == 1) {
		(*m_leaves)[global_i] = m_unconstrained.begin[bip.leftmost_leaf()];
		node.lchild() = none;
		node.rchild() = none;
		return (cp = none);
	} else {
		const auto lbip = small_bipartition{bip.left_mask()};
		const auto rbip = small_bipartition{bip.right_mask()};
		const index loffset = 1;
		const index roffset = loffset + 2 * lbip.num_leaves() - 1;
		const auto left = i + loffset;
		const auto right = i + roffset;
		node.lchild() = global_i + loffset;
		node.rchild() = global_i + roffset;
		m_bips[left] = lbip;
		m_bips[right] = rbip;
		(*m_leaves)[global_i] = none;
		(*m_tree)[node.lchild()].parent() = global_i;
		(*m_tree)[node.rchild()].parent() = global_i;
		const auto l_cp = init_subtree(left);
		const auto r_cp = init_subtree(right);
		const auto p_cp = bip.has_next() ? i : none;
		return (cp = (l_cp != none ? l_cp : (r_cp != none ? r_cp : p_cp)));
	}
}

unconstrained_tree_iterator::unconstrained_tree_iterator(const multitree_nodes::unconstrained node,
                                                         tree& tree, std::vector<index>& leaves,
                                                         index root)
        : m_unconstrained{node}, m_num_leaves{node.num_leaves()}, m_tree{&tree}, m_leaves{&leaves},
          m_root{root}, m_choice_points(2 * m_num_leaves - 1, 0),
          m_bips(m_choice_points.size(), small_bipartition::full_set(m_num_leaves)) {
	assert(m_num_leaves > 2);
	init_subtree(0);
}

void unconstrained_tree_iterator::next() {
	auto cur = m_choice_points[0];
	assert(cur != none);
	index new_cp;
	if (m_bips[cur].has_next()) {
		m_bips[cur].next();
		init_subtree(cur);
	} else {
		index l;
		index r;
		// find first node where we have a choice
		do {
			cur = local_parent(cur);
			l = local_lchild(cur);
			r = local_rchild(cur);
		} while (!(m_bips[cur].has_next() || subtree_has_next(l) || subtree_has_next(r)));
		// increment the first possible choice point
		if (subtree_has_next(l)) {
			m_bips[m_choice_points[l]].next();
			new_cp = init_subtree(l);
		} else if (subtree_has_next(r)) {
			m_bips[m_choice_points[l]].reset();
			m_bips[m_choice_points[r]].next();
			const auto l_cp = init_subtree(l);
			const auto r_cp = init_subtree(r);
			new_cp = l_cp != none ? l_cp : r_cp;
		} else {
			m_bips[cur].next();
			new_cp = init_subtree(cur);
		}

		// propagate new choice point upwards
		while (cur != none) {
			m_choice_points[cur] = new_cp;
			cur = is_local_root(cur) ? none : local_parent(cur);
		}
	}
}

bool unconstrained_tree_iterator::has_next() const { return m_choice_points[0] != none; }

} // namespace terraces
