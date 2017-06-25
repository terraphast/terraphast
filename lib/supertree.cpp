#include <terraces/bipartitions.hpp>
#include <terraces/supertree.hpp>
#include <terraces/union_find.hpp>

#include "supertree_enumerator.hpp"
#include "supertree_variants.hpp"
#include "supertree_variants_debug.hpp"

namespace terraces {

using check = variants::check_callback;
using count = variants::count_callback;

namespace debug {
namespace variants {
using namespace terraces::variants;

template class logging_decorator<check>;
template class logging_decorator<count>;
template class stack_state_decorator<check>;
template class stack_state_decorator<count>;
}
}

index remap_to_leaves(const tree& t, constraints& c, name_map& names, index& root) {
	auto leaves = leave_occ(t);
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
