#include <terraces/bipartitions.hpp>
#include <terraces/stack_allocator.hpp>
#include <terraces/supertree.hpp>
#include <terraces/union_find.hpp>

#include "supertree_enumerator.hpp"
#include "supertree_variants.hpp"
#include "supertree_variants_debug.hpp"
#include "supertree_variants_multitree.hpp"

namespace terraces {

namespace variants {
template class fast_check_decorator<check_callback>;
template class fast_check_decorator<count_callback>;
template class fast_check_decorator<multitree_callback>;
}

namespace debug {
namespace variants {
using namespace terraces::variants;

template class logging_decorator<check_callback>;
template class logging_decorator<count_callback>;
template class logging_decorator<multitree_callback>;
template class stack_state_decorator<check_callback>;
template class stack_state_decorator<count_callback>;
template class stack_state_decorator<multitree_callback>;
}
}

index remap_to_leaves(const tree& t, constraints& c, name_map& names, index& root) {
	auto fl = utils::free_list{};
	auto alloc = utils::stack_allocator<index>{fl, t.size()};
	auto leaves = leaf_occ(t, alloc);
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
	tree_enumerator<variants::count_callback> counter{{}, num_leaves, constraints.size()};
	return counter.run(num_leaves, constraints, root_leaf);
}

mpz_class count_supertree(index num_leaves, const constraints& constraints) {
	tree_enumerator<variants::count_callback> counter{{}, num_leaves, constraints.size()};
	return counter.run(num_leaves, constraints);
}

bool check_supertree(index num_leaves, const constraints& constraints, index root_leaf) {
	tree_enumerator<variants::check_callback> counter{{}, num_leaves, constraints.size()};
	return counter.run(num_leaves, constraints, root_leaf) > 1;
}

bool check_supertree(index num_leaves, const constraints& constraints) {
	tree_enumerator<variants::check_callback> counter{{}, num_leaves, constraints.size()};
	return counter.run(num_leaves, constraints) > 1;
}

} // namespace terraces
