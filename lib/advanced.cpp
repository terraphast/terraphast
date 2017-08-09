#include <terraces/advanced.hpp>

#include <terraces/clamped_uint.hpp>
#include <terraces/errors.hpp>
#include <terraces/subtree_extraction.hpp>
#include <terraces/supertree.hpp>

#include "supertree_enumerator.hpp"
#include "supertree_variants.hpp"
#include "supertree_variants_multitree.hpp"

namespace terraces {

supertree_data prepare_constraints(const tree& tree, const bitmatrix& data, name_map& names,
                                   index root) {
	auto trees = subtrees(tree, data);
	auto constraints = compute_constraints(trees);
	deduplicate_constraints(constraints);
	auto num_leaves = remap_to_leaves(tree, constraints, names, root);

	return {constraints, num_leaves, root};
}

bool check_terrace(const supertree_data& data) {
	tree_enumerator<variants::check_callback> enumerator{
	        {}, data.num_leaves, data.constraints.size()};
	try {
		return enumerator.run(data.num_leaves, data.constraints, data.root) > 1;
	} catch (terraces::tree_count_overflow_error&) {
		return true;
	}
}

uint64_t count_terrace(const supertree_data& data) {
	tree_enumerator<variants::clamped_count_callback> enumerator{
	        {}, data.num_leaves, data.constraints.size()};
	try {
		return enumerator.run(data.num_leaves, data.constraints, data.root).value();
	} catch (terraces::tree_count_overflow_error&) {
		return std::numeric_limits<uint64_t>::max();
	}
}

mpz_class count_terrace_bigint(const supertree_data& data) {
	tree_enumerator<variants::count_callback<mpz_class>> enumerator{
	        {}, data.num_leaves, data.constraints.size()};
	return enumerator.run(data.num_leaves, data.constraints, data.root);
}

mpz_class print_terrace(const supertree_data& data, const name_map& names, std::ostream& output) {
	tree_enumerator<variants::multitree_callback> enumerator{
	        {}, data.num_leaves, data.constraints.size()};
	auto result = enumerator.run(data.num_leaves, data.constraints, data.root);
	output << as_newick(result, names);

	return result->num_trees;
}

} // namespace terraces
