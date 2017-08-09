#include <terraces/clamped_uint.hpp>
#include <terraces/supertree.hpp>

#include "bipartitions.hpp"
#include "bitvector.hpp"
#include "stack_allocator.hpp"
#include "union_find.hpp"

#include "supertree_enumerator.hpp"
#include "supertree_variants.hpp"
#include "supertree_variants_debug.hpp"
#include "supertree_variants_multitree.hpp"

namespace terraces {

namespace debug {
namespace variants {
using namespace terraces::variants;

template class logging_decorator<check_callback>;
template class logging_decorator<count_callback<index>>;
template class logging_decorator<count_callback<mpz_class>>;
template class logging_decorator<multitree_callback>;
template class stack_state_decorator<check_callback>;
template class stack_state_decorator<count_callback<index>>;
template class stack_state_decorator<count_callback<mpz_class>>;
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

} // namespace terraces
