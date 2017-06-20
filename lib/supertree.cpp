#include <terraces/bipartitions.hpp>
#include <terraces/supertree.hpp>
#include <terraces/union_find.hpp>

#include "supertree_enumerator.hpp"
#include "supertree_variants.hpp"
#include "supertree_variants_debug.hpp"

namespace terraces {

template class tree_enumerator<debug::variants::logging_decorator<variants::multitree_callback>>;
template class tree_enumerator<
        debug::variants::stack_state_decorator<variants::multitree_callback>>;

template class tree_enumerator<debug::variants::logging_decorator<variants::count_callback>>;
template class tree_enumerator<debug::variants::stack_state_decorator<variants::count_callback>>;

index remap_to_leaves(const tree& t, constraints& c, name_map& names, index& root) {
	auto leaves = leave_occ(t);
	c = map_constraints(leaves, c);
	auto new_names = name_map(leaves.size());
	index i = 0;
	for (auto leaf : leaves) {
		new_names[i] = std::move(names[leaf]);
		++i;
	}
	names = std::move(new_names);
	root = leaves.rank(root);
	return leaves.size();
}

mpz_class tree_master::count_supertree(index num_leaves, const constraints& c, index root) {
	tree_enumerator<variants::count_callback> counter{{}};
	return counter.run(num_leaves, c, root);
}

mpz_class tree_master::count_supertree(index count, const constraints& c) {
	tree_enumerator<variants::count_callback> counter{{}};
	return counter.run(count, c);
}

} // namespace terraces
