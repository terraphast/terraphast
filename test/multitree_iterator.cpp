
#include <catch.hpp>

#include <fstream>

#include <terraces/parser.hpp>
#include <terraces/rooting.hpp>
#include <terraces/subtree_extraction.hpp>

#include "../lib/multitree_iterator.hpp"
#include "../lib/supertree_enumerator.hpp"
#include "../lib/supertree_variants_multitree.hpp"

namespace terraces {
namespace tests {

std::tuple<name_map, constraints, index> load(std::string tree_file_path,
                                              std::string data_file_path) {
	auto tree_file = std::ifstream{"../../data/data/" + tree_file_path};
	auto tree_string = std::string{};
	std::getline(tree_file, tree_string);
	auto data = terraces::parse_nwk(tree_string);

	auto data_file = std::ifstream{"../../data/data/" + data_file_path};
	const auto data_res = terraces::parse_bitmatrix(data_file, data.indices, data.tree.size());
	auto& tree = data.tree;
	auto& names = data.names;
	auto& site_occ = data_res.first;
	auto root_species = data_res.second;

	terraces::reroot_inplace(tree, root_species);

	const auto subtrees = terraces::subtrees(tree, site_occ);
	auto constraints = terraces::compute_constraints(subtrees);
	terraces::deduplicate_constraints(constraints);
	terraces::remap_to_leaves(tree, constraints, names, root_species);

	return std::make_tuple(names, constraints, root_species);
}

TEST_CASE("multitree_iterator init simple", "[multitree]") {
	auto data = load("Eucalyptus.nwk.3", "Eucalyptus.data.3");
	auto names = std::get<0>(data);
	auto constraints = std::get<1>(data);
	auto root_species = std::get<2>(data);
	tree_enumerator<variants::multitree_callback> enumerator{
	        {}, names.size(), constraints.size()};
	auto result = enumerator.run(names.size(), constraints, root_species);

	tree t(2 * names.size() - 1);
	permutation leaves(t.size(), none);
	multitree_iterator(result, t, leaves);

	std::cout << as_newick(t, names, leaves);
}

TEST_CASE("multitree_iterator init unconstrained", "[multitree]") {
	name_map names{"1", "2", "3", "4", "5", "6"};
	constraints constraints{{0, 1, 2}};
	index root_species = 0;
	tree_enumerator<variants::multitree_callback> enumerator{
	        {}, names.size(), constraints.size()};
	auto result = enumerator.run(names.size(), constraints, root_species);

	tree t(2 * names.size() - 1);
	permutation leaves(t.size(), none);
	multitree_iterator(result, t, leaves);

	std::cout << as_newick(t, names, leaves);
}

} // namespace tests
} // namespace terraces
