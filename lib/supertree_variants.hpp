#ifndef SUPERTREE_VARIANTS_HPP
#define SUPERTREE_VARIANTS_HPP

#include <terraces/bipartitions.hpp>
#include <terraces/constraints.hpp>
#include <terraces/multitree.hpp>
#include <terraces/trees.hpp>

#include <memory>
#include <stack>

#include <gmpxx.h>

#include <terraces/io_utils.hpp>

namespace terraces {
namespace variants {

template <typename Result>
class abstract_callback {
public:
	using result_type = Result;

	/** Called when a (sub)call begins. */
	void enter(const bitvector&) {}
	/** Called when a (sub)call finishes. */
	Result exit(Result val) { return val; }

	/** Returns the result for a single leaf. */
	Result base_one_leaf(index);
	/** Returns the result for two leaves. */
	Result base_two_leaves(index, index);
	/** Returns the result for multiple leaves without constraints. */
	Result base_unconstrained(const bitvector&);

	bool fast_return(const bipartition_iterator&) { return false; }
	Result fast_return_value(const bipartition_iterator&);

	/** Called when we begin iterating over the bipartitions. */
	Result begin_iteration(const bipartition_iterator&, const bitvector&, const constraints&) {
		return Result{};
	}
	/** Returns true iff the iteration should continue. */
	bool continue_iteration(Result) { return true; }
	/** Called when an iteration step begins. */
	void step_iteration(const bipartition_iterator&) {}
	/** Called when the last iteration step has finished. */
	void finish_iteration() {}
	/** Called before descending into the left subset. */
	void left_subcall() {}
	/** Called before descending into the right subset. */
	void right_subcall() {}
	/** Accumulates the results from multiple bipartitions. */
	Result accumulate(Result, Result);
	/** Combines the results from two subcalls. */
	Result combine(Result, Result);
};

class count_callback : public abstract_callback<mpz_class> {
public:
	using return_type = mpz_class;
	return_type base_one_leaf(index) { return 1; }
	return_type base_two_leaves(index, index) { return 1; }
	return_type base_unconstrained(const bitvector& leaves) {
		return count_unrooted_trees<return_type>(leaves.count());
	}

	index fast_return_value(const bipartition_iterator& bip_it) { return bip_it.num_bip(); }

	return_type accumulate(return_type acc, return_type val) { return acc + val; }
	return_type combine(return_type left, return_type right) { return left * right; }
};

class check_callback : public abstract_callback<index> {
public:
	using return_type = index;

	index base_one_leaf(index) { return 1; }
	index base_two_leaves(index, index) { return 1; }
	index base_unconstrained(const bitvector&) { return 2; }

	index fast_return_value(const bipartition_iterator& bip_it) { return bip_it.num_bip(); }

	bool continue_iteration(index acc) { return acc < 2; }

	index accumulate(index acc, index val) { return acc + val; }
	index combine(index left, index right) { return left * right; }
};

template <typename Callback>
class fast_check_decorator : public Callback {
public:
	using return_type = typename Callback::result_type;

	bool fast_return(const bipartition_iterator& bip_it) { return bip_it.num_bip() > 1; }
};

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
		(void)node;
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

#endif // SUPERTREE_VARIANTS_HPP
