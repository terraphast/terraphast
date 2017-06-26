#ifndef SUPERTREE_VARIANTS_MULTITREE_HPP
#define SUPERTREE_VARIANTS_MULTITREE_HPP

#include "supertree_variants.hpp"

#include <memory>
#include <stack>

namespace terraces {
namespace variants {

template <typename T>
class storage_block {
private:
	std::unique_ptr<T[]> begin;
	index size;
	index max_size;

public:
	storage_block(index max_size)
	        : begin{std::make_unique<T[]>(max_size)}, size{0}, max_size{max_size} {}

	bool has_space(index required = 1) { return size + required <= max_size; }

	T* get() {
		assert(has_space());
		return begin.get() + (size++);
	}

	T* get_range(index required) {
		assert(has_space(required));
		auto result = begin.get() + size;
		size += required;
		return result;
	}
};

template <typename T>
class storage_blocks {
private:
	std::vector<storage_block<T>> m_blocks;
	index m_block_size;

public:
	storage_blocks(index block_size = 1024) : m_blocks{}, m_block_size{block_size} {
		m_blocks.emplace_back(m_block_size);
	}

	T* get() {
		if (!m_blocks.back().has_space()) {
			m_blocks.emplace_back(m_block_size);
		}
		return m_blocks.back().get();
	}

	T* get_range(index required) {
		if (!m_blocks.back().has_space(required)) {
			m_blocks.emplace_back(required);
			auto result = m_blocks.back().get_range(required);
			auto last_it = --m_blocks.end();
			auto prev_it = --(--m_blocks.end());
			std::iter_swap(last_it, prev_it);
			return result;
		}
		return m_blocks.back().get_range(required);
	}
};

class multitree_callback : public abstract_callback<multitree_node*> {
private:
	storage_blocks<multitree_node> m_nodes;
	storage_blocks<index> m_leaves;
	std::stack<multitree_node*> m_accumulators;

	multitree_node* alloc_node() { return m_nodes.get(); }

	multitree_node* alloc_nodes(index num) { return m_nodes.get_range(num); }

	std::pair<index*, index*> alloc_leaves(const bitvector& leaves) {
		auto size = leaves.count();
		auto a_leaves = m_leaves.get_range(size);
		index i = 0;
		for (auto el : leaves) {
			a_leaves[i++] = el;
		}
		return {a_leaves, a_leaves + size};
	}

public:
	using return_type = multitree_node*;

	return_type base_one_leaf(index i) { return alloc_node()->as_single_leaf(i); }
	return_type base_two_leaves(index i, index j) { return alloc_node()->as_two_leaves(i, j); }
	return_type base_unconstrained(const bitvector& leaves) {
		return alloc_node()->as_unconstrained(alloc_leaves(leaves));
	}

	return_type fast_return_value(const bipartition_iterator& bip_it) {
		return alloc_node()->as_unexplored(alloc_leaves(bip_it.leaves()));
	}

	return_type begin_iteration(const bipartition_iterator& bip_it, const bitvector&,
	                            const constraints&) {
		auto acc = alloc_node()->as_alternative_array(alloc_nodes(bip_it.num_bip()),
		                                              bip_it.leaves().count());
		m_accumulators.push(acc);
		return acc;
	}

	return_type accumulate(multitree_node* acc, multitree_node* node) {
		assert(acc == m_accumulators.top());
		assert(node == (acc->alternative_array.end - 1));
		assert(acc->num_leaves == node->num_leaves);
		acc->num_trees += node->num_trees;
		// node is already linked from combine(.,.)
		return acc;
	}

	return_type combine(multitree_node* left, multitree_node* right) {
		auto acc = m_accumulators.top();
		auto& aa = acc->alternative_array;
		auto result = aa.end->as_inner_node(left, right);
		++aa.end;
		return result;
	}

	void finish_iteration() { m_accumulators.pop(); }
};

} // namespace variants
} // namespace terraces

#endif // SUPERTREE_VARIANTS_MULTITREE_HPP
