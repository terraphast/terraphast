
#include <catch.hpp>

#include <algorithm>
#include <fstream>

#include "datasets.hpp"

#include "../lib/multitree_iterator.hpp"
#include "../lib/supertree_enumerator.hpp"
#include "../lib/supertree_variants_multitree.hpp"
#include "../lib/validation.hpp"

namespace terraces {
namespace tests {

TEST_CASE("multitree_iterator init simple", "[multitree]") {
	auto data = load("Eucalyptus.nwk.3", "Eucalyptus.data.3");
	auto names = data.tree_set.names;
	auto constraints = data.constraints;
	auto root_species = data.root_species;
	tree_enumerator<variants::multitree_callback> enumerator{
	        {}, names.size(), constraints.size()};
	auto result = enumerator.run(names.size(), constraints, root_species);

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
