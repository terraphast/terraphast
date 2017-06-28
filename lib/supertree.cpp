#include <terraces/bipartitions.hpp>
#include <terraces/stack_allocator.hpp>
#include <terraces/supertree.hpp>
#include <terraces/union_find.hpp>

#include "supertree_enumerator.hpp"
#include "supertree_variants.hpp"
#include "supertree_variants_debug.hpp"

namespace terraces {

template class tree_enumerator<variants::multitree_callback>;
template class tree_enumerator<debug::variants::logging_decorator<variants::multitree_callback>>;
template class tree_enumerator<
        debug::variants::stack_state_decorator<variants::multitree_callback>>;

template class tree_enumerator<variants::check_callback>;
template class tree_enumerator<debug::variants::logging_decorator<variants::check_callback>>;
template class tree_enumerator<debug::variants::stack_state_decorator<variants::check_callback>>;

template class tree_enumerator<variants::count_callback>;
template class tree_enumerator<debug::variants::logging_decorator<variants::count_callback>>;
template class tree_enumerator<debug::variants::stack_state_decorator<variants::count_callback>>;

index remap_to_leaves(const tree& t, constraints& c, name_map& names, index& root) {
	auto fl = utils::free_list{};
	auto alloc = utils::stack_allocator<index>{fl, t.size()};
	auto leaves = leave_occ(t, alloc);
	c = map_constraints(leaves, c);
	auto new_names = name_map(leaves.count());
	index i = 0;
	for (auto leaf = leaves.first_set(); leaf < leaves.last_set();
	     leaf = leaves.next_set(leaf)) {
		new_names[i] = std::move(names[leaf]);
		++i;
	}
	names = std::move(new_names);
	root = leaves.rank(root);
	return leaves.count();
}

mpz_class count_supertree(index num_leaves, const constraints& constraints, index root_leaf) {
	tree_enumerator<variants::count_callback> counter{{}};
	return counter.run(num_leaves, constraints, root_leaf);
}

mpz_class count_supertree(index num_leaves, const constraints& constraints) {
	tree_enumerator<variants::count_callback> counter{{}};
	return counter.run(num_leaves, constraints);
}

bool check_supertree(index num_leaves, const constraints& constraints, index root_leaf) {
	tree_enumerator<variants::check_callback> counter{{}};
	return counter.run(num_leaves, constraints, root_leaf) > 1;
}

bool check_supertree(index num_leaves, const constraints& constraints) {
	tree_enumerator<variants::check_callback> counter{{}};
	return counter.run(num_leaves, constraints) > 1;
}

} // namespace terraces
