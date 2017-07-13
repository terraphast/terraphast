#include <bitset>
#include <catch.hpp>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>

#include <terraces/constraints.hpp>
#include <terraces/parser.hpp>
#include <terraces/rooting.hpp>
#include <terraces/subtree_extraction.hpp>
#include <terraces/trees.hpp>

#include "../lib/supertree_enumerator.hpp"
#include "../lib/supertree_variants.hpp"
#include "../lib/supertree_variants_debug.hpp"
#include "../lib/supertree_variants_multitree.hpp"

using terraces::tree_enumerator;
using terraces::variants::multitree_callback;
using terraces::variants::count_callback;
using terraces::utils::as_comma_separated_output;
using terraces::full_set;

namespace terraces {
namespace tests {

mpz_class helper(std::string, std::string);

std::vector<std::tuple<std::string, std::string, index>> data_sets = {
        {"Allium.nwk", "Allium.data", 8038035},
        {"Allium_Reduced.nwk", "Allium_Reduced.data", 730680125},
        {"Allium_Tiny.nwk", "Allium_Tiny.data", 35},
        {"Asplenium.nwk.1", "Asplenium.data.1", 1},
        {"Asplenium.nwk.2", "Asplenium.data.2", 95},
        {"Bouchenak.nwk", "Bouchenak.data", 61261515},
        {"Caryophyllaceae.nwk", "Caryophyllaceae.data", 718346120625},
        {"Eucalyptus.nwk.1", "Eucalyptus.data.1", 229},
        {"Eucalyptus.nwk.2", "Eucalyptus.data.2", 267},
        {"Eucalyptus.nwk.3", "Eucalyptus.data.3", 9},
        {"Euphorbia.nwk.1", "Euphorbia.data.1", 759},
        {"Euphorbia.nwk.2", "Euphorbia.data.2", 759},
        {"Ficus.nwk.1", "Ficus.data.1", 283815},
        {"Ficus.nwk.2", "Ficus.data.2", 851445},
        {"Ficus.nwk.3", "Ficus.data.3", 851445},
        {"Iris.nwk", "Iris.data", 1},
        {"Meredith.mammals.nwk", "Meredith.mammals.data", 1},
        {"Meusemann.nwk", "Meusemann.data", 1},
        {"Miadlikowska.fungi.nwk", "Miadlikowska.fungi.data", 11655},
        {"Misof.insects.nwk", "Misof.insects.data", 1},
        {"Primula.nwk", "Primula.data", 2835},
        {"Pyron.nwk", "Pyron.data", 2205},
        {"Rabosky.scincids.nwk", "Rabosky.scincids.data", 3},
        {"Ranunculus.nwk", "Ranunculus.data", 3},
        {"Rhododendron.nwk", "Rhododendron.data", 81},
        //        {"Rosaceae.nwk","Rosaceae.data",171924246129452853684375},
        //        {"Shi.bats.nwk","Shi.bats.data",242234933881884349476362274404296875},
        {"Solanum.nwk", "Solanum.data", 211865625},
        {"Springer.primates.nwk", "Springer.primates.data", 70840575},
        {"Szygium.nwk.1", "Szygium.data.1", 45},
        {"Szygium.nwk.2", "Szygium.data.2", 45},
        {"Tolley.chameleons.nwk", "Tolley.chameleons.data", 1},
        {"Wick.1kp.few.genes.nwk", "Wick.1kp.few.genes.data", 1},
        {"Wick.1kp.many.genes.nwk", "Wick.1kp.many.genes.data", 1},
        {"Yang.caryo.1122.nwk", "Yang.caryo.1122.data", 1},
        {"Yang.caryo.209.nwk", "Yang.caryo.209.data", 1}};

TEST_CASE("app", "[app]") {
	for (auto set : data_sets) {
		CHECK(helper(std::get<0>(set), std::get<1>(set)) == std::get<2>(set));
	}
}

mpz_class helper(std::string tree_file_path, std::string data_file_path) {
	auto tree_file = std::ifstream{"../../data/data/" + tree_file_path};
	auto tree_string = std::string{};
	std::getline(tree_file, tree_string);
	auto data = terraces::parse_nwk(tree_string);

	auto data_file = std::ifstream{"../../data/data/" + data_file_path};
	const auto data_res = terraces::parse_bitmatrix(data_file, data.indices, data.tree.size());
	const auto num_nodes = data_res.first.rows();
	auto num_species = data.indices.size();
	const auto num_sites = data_res.first.cols();
	auto& tree = data.tree;
	auto& names = data.names;
	auto& site_occ = data_res.first;
	auto root_species = data_res.second;

	terraces::reroot_inplace(tree, root_species);

	const auto subtrees = terraces::subtrees(tree, site_occ);

	auto constraints = terraces::compute_constraints(subtrees);
	auto num_constraints = constraints.size();

	auto duplicate = terraces::deduplicate_constraints(constraints);
	num_constraints = constraints.size();

	num_species = terraces::remap_to_leaves(tree, constraints, names, root_species);

	tree_enumerator<count_callback> enumerator{{}, num_species, num_constraints};
	return enumerator.run(num_species, constraints, root_species);
}

} // namespace tests
} // namespace terraces
