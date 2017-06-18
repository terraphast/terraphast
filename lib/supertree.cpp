#include <terraces/bipartitions.hpp>
#include <terraces/supertree.hpp>
#include <terraces/union_find.hpp>

#include "supertree_enumerator.hpp"
#include "supertree_variants.hpp"

namespace terraces {

mpz_class tree_master::count_supertree(const tree& t, const constraints& c, index root) {
	tree_enumerator<tree_count_callback> counter{{}};
	return counter.run(t, c, root);
}

mpz_class tree_master::count_supertree(index count, const constraints& c) {
	tree_enumerator<tree_count_callback> counter{{}};
	return counter.run(count, c);
}

} // namespace terraces
