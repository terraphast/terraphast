#ifndef TERRACES_SUPERTREE_HPP
#define TERRACES_SUPERTREE_HPP

#include "constraints.hpp"
#include "fast_set.hpp"
#include "trees.hpp"
#include "union_find.hpp"
#include <string>

#include <tuple>

namespace terraces {

class raii_stackstate {
public:
	// current_bip, max_bip, count, has_left_subcall
	using state = std::vector<std::tuple<uint64_t, uint64_t, index, bool>>;

private:
	state& m_state;

public:
	raii_stackstate(state& stackstate) : m_state{stackstate} {
		m_state.emplace_back(0, 0, 0, false);
	}
	~raii_stackstate() { m_state.pop_back(); }
	void init_iteration(uint64_t max_bip) { std::get<1>(m_state.back()) = max_bip; }
	void update(uint64_t cur_bip, uint64_t count) {
		std::get<0>(m_state.back()) = cur_bip;
		std::get<2>(m_state.back()) = count;
	}
	void go_left() { std::get<3>(m_state.back()) = false; }
	void go_right() { std::get<3>(m_state.back()) = true; }
};

class tree_master {
private:
	constraints map_constraints(const fast_index_set& leaves, const constraints& c);

	fast_index_set leave_occ(const tree& t);
	fast_index_set full_set(index size);

	raii_stackstate::state m_stack_state;

public:
	const raii_stackstate::state& get_state() const { return m_stack_state; }

	counted_supertree count_supertree(const tree&, const constraints&, index root);

	/**
	 * Count trees given a number and a vector of constraints. Entry point for tree counting.
	 */
	counted_supertree count_supertree(index count, const constraints&);

	/**
	 * Count trees.
	 */
	counted_supertree count_supertree(const fast_index_set&, const fast_index_set&,
	                                  const constraints&,
	                                  const name_map& = name_map(std::vector<std::string>()));
	std::string printable_tree_representation(const fast_index_set&, const name_map&);
	std::string printable_leaf_pair_representation(const fast_index_set&, const name_map&);

	/*
	 * Enumerate trees. Working copy.
	 */
	// counted_supertree enumerate_supertrees(const fast_index_set&, const fast_index_set&,
	//                                        const constraints&, const name_map&);

	size_t check_supertree(const fast_index_set& leaves, const fast_index_set& in_c_occ,
	                       const constraints& in_c, index init_number);
};

} // namespace terraces

#endif
