
#include <catch.hpp>

#include <algorithm>
#include <sstream>

#include <terraces/parser.hpp>
#include <terraces/rooting.hpp>
#include <terraces/subtree_extraction.hpp>
#include <terraces/supertree.hpp>

#include "validation_utils.hpp"

#include "../lib/multitree_iterator.hpp"
#include "../lib/supertree_enumerator.hpp"
#include "../lib/supertree_variants_multitree.hpp"
#include "../lib/validation.hpp"

namespace terraces {
namespace tests {

TEST_CASE("multitree_iterator init simple", "[multitree]") {
	auto tree = terraces::parse_nwk("((((s2,s4),((s13,s1),s7)),s3),s5);");
	auto data_stream = std::istringstream{"7 4\n1 1 1 1 s1\n0 0 1 0 s2\n1 1 0 0 s3\n1 1 1 0 "
	                                      "s4\n1 1 0 1 s5\n1 0 0 1 s7\n0 0 0 1 s13"};
	auto data_res = terraces::parse_bitmatrix(data_stream, tree.indices, tree.tree.size());
	auto& site_occ = data_res.first;
	auto root_species = data_res.second;

	terraces::reroot_inplace(tree.tree, root_species);

	const auto subtrees = terraces::subtrees(tree.tree, site_occ);
	auto constraints = terraces::compute_constraints(subtrees);
	terraces::deduplicate_constraints(constraints);
	auto num_species =
	        terraces::remap_to_leaves(tree.tree, constraints, tree.names, root_species);
	tree_enumerator<variants::multitree_callback> enumerator{
	        {}, num_species, constraints.size()};
	auto result = enumerator.run(num_species, constraints, root_species);

	check_unique_trees(result, 9);
}

TEST_CASE("multitree_iterator init unconstrained", "[multitree]") {
	name_map names{"1", "2", "3", "4", "5", "6", "7", "8"};
	constraints constraints{{0, 1, 2}};
	index root_species = 0;
	tree_enumerator<variants::multitree_callback> enumerator{
	        {}, names.size(), constraints.size()};
	auto result = enumerator.run(names.size(), constraints, root_species);

	check_unique_trees(result, count_unrooted_trees<index>(7));
}

} // namespace tests
} // namespace terraces
