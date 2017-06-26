#ifndef SUPERTREE_VARIANTS_HPP
#define SUPERTREE_VARIANTS_HPP

#include <terraces/bipartitions.hpp>
#include <terraces/constraints.hpp>
#include <terraces/multitree.hpp>
#include <terraces/trees.hpp>

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

class multitree_callback : public abstract_callback<multitree_node*> {
private:
	multitree_node* alloc_node() { return new multitree_node; }
	std::pair<index*, index*> alloc_leaves(const bitvector& leaves) {
		auto size = leaves.count();
		auto a_leaves = new index[size];
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
		return alloc_node()->as_alternative_list(bip_it.num_bip(), bip_it.leaves().count());
	}

	return_type accumulate(multitree_node* acc, multitree_node* cur) {
		auto node = alloc_node()->as_alternative_list_node(cur);
		assert(acc->type == multitree_node_type::alternative_list);
		assert(acc->alternative_list.begin == nullptr ||
		       acc->alternative_list.begin->type ==
		               multitree_node_type::alternative_list_node);
		assert(acc->num_leaves == node->num_leaves);
		acc->num_trees += cur->num_trees;
		node->alternative_list_node.next = acc->alternative_list.begin;
		acc->alternative_list.begin = node;
		return acc;
	}

	return_type combine(multitree_node* left, multitree_node* right) {
		return alloc_node()->as_inner_node(left, right);
	}
};

} // namespace variants
} // namespace terraces

#endif // SUPERTREE_VARIANTS_HPP
