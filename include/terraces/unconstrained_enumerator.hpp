#ifndef UNCONSTRAINED_ENUMERATOR_CPP
#define UNCONSTRAINED_ENUMERATOR_CPP

#include <terraces/bits.hpp>
#include <terraces/multitree.hpp>

#include <bitset>
#include <cassert>
#include <vector>

namespace terraces {

using bits::bitscan;
using bits::rbitscan;
using bits::popcount;

struct small_bipartition {
	index m_mask;
	index m_end_bip;
	index m_cur_bip;

	small_bipartition(index mask = 1) : m_mask{mask}, m_end_bip{1ull << rbitscan(mask)} {
		reset();
	}

	// credit goes to nglee
	// (see stackoverflow.com/questions/44767080/incrementing-masked-bitsets)
	index masked_increment(index bip) const { return -(bip ^ m_mask) & m_mask; }

	bool is_valid() const { return m_cur_bip < m_end_bip; }
	bool has_next() const { return masked_increment(m_cur_bip) < m_end_bip; }

	void next() {
		assert(is_valid());
		m_cur_bip = masked_increment(m_cur_bip);
	}
	void reset() { m_cur_bip = 1ull << bitscan(m_mask); }

	index mask() const { return m_mask; }
	index left_mask() const { return m_cur_bip; }
	index right_mask() const { return m_cur_bip ^ m_mask; }
	index leftmost_leaf() const { return bitscan(m_mask); }
	index rightmost_leaf() const { return rbitscan(m_mask); }
	index num_leaves() const { return popcount(m_mask); }

	static small_bipartition full_set(index num_leaves) {
		assert(num_leaves < bits::word_bits);
		return {(1ull << num_leaves) - 1};
	}
};

class unconstrained_tree_iterator {
private:
	const multitree_nodes::unconstrained m_unconstrained;
	index m_num_leaves;
	tree* m_tree;
	std::vector<index>* m_leaves;
	index m_root;
	std::vector<index> m_choice_points;
	std::vector<small_bipartition> m_bips;

	index local_lchild(index i) const;
	index local_rchild(index i) const;
	index local_rsibling(index i) const;
	index local_parent(index i) const;
	bool is_local_left_child(index i) const;
	bool is_local_leaf(index i) const;
	bool is_local_root(index i) const;
	bool subtree_has_next(index i) const;
	index first_cp(index i) const;

public:
	unconstrained_tree_iterator(const multitree_nodes::unconstrained node, tree& tree,
	                            std::vector<index>& leaves, index root);
	index init_subtree(index i);
	void next();
	bool has_next() const;
};
} // namespace terraces

#endif // UNCONSTRAINED_ENUMERATOR_CPP
