#ifndef SUPERTREE_ENUMERATOR_HPP
#define SUPERTREE_ENUMERATOR_HPP

#include <atomic>
#include <terraces/supertree.hpp>
#include <thread>

#include "bipartitions.hpp"
#include "stack_allocator.hpp"
#include "union_find.hpp"

#include "supertree_helpers.hpp"
#include "utils.hpp"

namespace terraces {

template <typename Callback, bool Parallel = false>
class tree_enumerator {
	using result_type = typename Callback::result_type;
	friend class tree_enumerator<Callback, true>;

private:
	Callback m_cb;

	utils::free_list fl1;
	utils::free_list fl2;
	utils::free_list fl3;
	utils::stack_allocator<index> m_leaf_allocator;
	utils::stack_allocator<index> m_c_occ_allocator;
	utils::stack_allocator<index> m_union_find_allocator;

	const constraints* m_constraints;

	std::vector<std::thread> m_thread_pool;
	std::vector<std::unique_ptr<tree_enumerator<Callback>>> m_runner_pool;
	std::vector<std::pair<ranked_bitvector, result_type>> m_work_pool;
	std::atomic_uint m_next_work;

	bool should_resume_parallel(const bipartition_iterator& bip_it) const;

	result_type iterate(bipartition_iterator& bip_it, const bitvector& new_constraint_occ);

public:
	tree_enumerator(Callback cb, index leaf_count, index constraint_count);
	result_type run(index num_leaves, const constraints& constraints, index root_leaf);
	result_type run(index num_leaves, const constraints& constraints);
	result_type run(const ranked_bitvector& leaves, const bitvector& constraint_occ);
	const Callback& callback() const { return m_cb; }
	const Callback& parallel_callback(index thread) const {
		return m_runner_pool.at(thread)->callback();
	}
};

template <typename Callback, bool Parallel>
tree_enumerator<Callback, Parallel>::tree_enumerator(Callback cb, index leaf_count,
                                                     index constraint_count)
        : m_cb{std::move(cb)}, m_leaf_allocator{fl1, bitvector::alloc_size(leaf_count)},
          m_c_occ_allocator{fl2, bitvector::alloc_size(constraint_count)},
          m_union_find_allocator{fl3, leaf_count} {
	if (Parallel) {
		auto num_threads = std::thread::hardware_concurrency();
		m_thread_pool.reserve(num_threads);
		for (index i = 0; i < num_threads; ++i) {
			m_runner_pool.push_back(std::make_unique<tree_enumerator<Callback>>(
			        m_cb, leaf_count, constraint_count));
		}
	}
}

template <typename Callback, bool Parallel>
auto tree_enumerator<Callback, Parallel>::run(index num_leaves, const constraints& constraints)
        -> result_type {
	auto leaves = full_ranked_set(num_leaves, m_leaf_allocator);
	auto c_occ = full_set(constraints.size(), m_c_occ_allocator);
	assert(filter_constraints(leaves, c_occ, constraints, m_c_occ_allocator) == c_occ);
	m_constraints = &constraints;
	for (auto& runner : m_runner_pool) {
		runner->m_constraints = m_constraints;
	}
	return run(leaves, c_occ);
}

template <typename Callback, bool Parallel>
auto tree_enumerator<Callback, Parallel>::run(index num_leaves, const constraints& constraints,
                                              index root_leaf) -> result_type {
	auto leaves = full_ranked_set(num_leaves, m_leaf_allocator);
	auto c_occ = full_set(constraints.size(), m_c_occ_allocator);
	assert(filter_constraints(leaves, c_occ, constraints, m_c_occ_allocator) == c_occ);
	// enter the call
	m_cb.enter(leaves);
	// no base cases
	assert(num_leaves > 2);
	assert(!constraints.empty());
	// build bipartition iterator:
	auto sets = union_find{num_leaves, m_union_find_allocator};
	index rep = root_leaf == 0 ? 1 : 0;
	// merge all non-root leaves into one set
	for (index i = rep + 1; i < num_leaves; ++i) {
		if (i != root_leaf) {
			sets.merge(rep, i);
		}
	}
	sets.compress();
	m_constraints = &constraints;
	for (auto& runner : m_runner_pool) {
		runner->m_constraints = m_constraints;
	}
	auto bip_it = bipartition_iterator{leaves, sets, m_leaf_allocator};
	return m_cb.exit(iterate(bip_it, c_occ));
}

template <typename Callback, bool Parallel>
auto tree_enumerator<Callback, Parallel>::run(const ranked_bitvector& leaves,
                                              const bitvector& constraint_occ) -> result_type {
	m_cb.enter(leaves);

	// base cases: only a few leaves
	assert(leaves.count() > 0);
	if (leaves.count() == 1) {
		return m_cb.exit(m_cb.base_one_leaf(leaves.first_set()));
	}

	if (leaves.count() == 2) {
		auto fst = leaves.first_set();
		auto snd = leaves.next_set(fst);
		return m_cb.exit(m_cb.base_two_leaves(fst, snd));
	}

	bitvector new_constraint_occ =
	        filter_constraints(leaves, constraint_occ, *m_constraints, m_c_occ_allocator);
	// base case: no constraints left
	if (new_constraint_occ.empty()) {
		return m_cb.exit(m_cb.base_unconstrained(leaves));
	}

	union_find sets = apply_constraints(leaves, new_constraint_occ, *m_constraints,
	                                    m_union_find_allocator);
	bipartition_iterator bip_it(leaves, sets, m_leaf_allocator);

	return iterate(bip_it, new_constraint_occ);
}

template <typename Callback, bool Parallel>
auto tree_enumerator<Callback, Parallel>::iterate(bipartition_iterator& bip_it,
                                                  const bitvector& new_constraint_occ)
        -> result_type {
	if (m_cb.fast_return(bip_it)) {
		return m_cb.exit(m_cb.fast_return_value(bip_it));
	}

	auto result = m_cb.begin_iteration(bip_it, new_constraint_occ, *m_constraints);
	if (should_resume_parallel(bip_it)) {
		// initialize work queue
		assert(m_work_pool.empty());
		while (bip_it.is_valid()) {
			m_work_pool.emplace_back(bip_it.get_current_set(), m_cb.null_result());
			bip_it.flip_sets();
			m_work_pool.emplace_back(bip_it.get_current_set(), m_cb.null_result());
			bip_it.increase();
		}
		m_next_work.store(0);

		// initialize threads
		const index num_threads =
		        std::min<index>(m_work_pool.size(), std::thread::hardware_concurrency());
		const index max_work = m_work_pool.size();
		for (index i = 0; i < num_threads; ++i) {
			m_thread_pool.emplace_back([&, i]() {
				index work_idx;
				auto& runner = m_runner_pool[i];
				while ((work_idx = m_next_work.fetch_add(1)) < max_work) {
					auto& work = m_work_pool[work_idx];
					work.second = runner->run(work.first, new_constraint_occ);
				}
			});
		}
		for (auto& thread : m_thread_pool) {
			thread.join();
		}
		for (index i = 0; i < max_work; i += 2) {
			auto& left = m_work_pool[i].second;
			auto& right = m_work_pool[i + 1].second;
			result = m_cb.accumulate(result, m_cb.combine(left, right));
		}
		m_thread_pool.clear();
		m_work_pool.clear();
	} else {
		// iterate over all possible bipartitions
		while (bip_it.is_valid() && m_cb.continue_iteration(result)) {
			m_cb.step_iteration(bip_it);
			m_cb.left_subcall();
			auto left = run(bip_it.get_current_set(), new_constraint_occ);

			bip_it.flip_sets();
			m_cb.right_subcall();
			auto right = run(bip_it.get_current_set(), new_constraint_occ);

			// accumulate result
			result = m_cb.accumulate(result, m_cb.combine(left, right));

			bip_it.increase();
		}
	}
	m_cb.finish_iteration();

	return m_cb.exit(result);
}

template <typename Callback, bool Parallel>
bool tree_enumerator<Callback, Parallel>::should_resume_parallel(
        const bipartition_iterator& bip_it) const {
	return Parallel && bip_it.num_bip() > std::thread::hardware_concurrency() &&
	       bip_it.leaves().count() > 8;
}

} // namespace terraces

#endif // SUPERTREE_ENUMERATOR_HPP
