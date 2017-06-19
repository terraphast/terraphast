#include <terraces/bipartitions.hpp>
#include <terraces/supertree.hpp>
#include <terraces/union_find.hpp>

#include "supertree_enumerator.hpp"
#include "supertree_variants.hpp"

namespace terraces {

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
	tree_enumerator<tree_count_callback> counter{{}};
	return counter.run(num_leaves, c, root);
}

mpz_class tree_master::count_supertree(index count, const constraints& c) {
	tree_enumerator<tree_count_callback> counter{{}};
	return counter.run(count, c);
}

} // namespace terraces
