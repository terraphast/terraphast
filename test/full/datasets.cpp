#include "datasets.hpp"

#include <catch.hpp>

#include <terraces/rooting.hpp>
#include <terraces/subtree_extraction.hpp>
#include <terraces/supertree.hpp>

#include "../lib/supertree_helpers.hpp"

namespace terraces {
namespace tests {

using std::make_tuple;

const std::vector<std::tuple<std::string, std::string, mpz_class>> data_sets{
        make_tuple("Allium.nwk", "Allium.data", 8038035),
        make_tuple("Allium_Reduced.nwk", "Allium_Reduced.data", 730680125),
        make_tuple("Allium_Tiny.nwk", "Allium_Tiny.data", 35),
        make_tuple("Asplenium.nwk.1", "Asplenium.data.1", 1),
        make_tuple("Asplenium.nwk.2", "Asplenium.data.2", 95),
        make_tuple("Bouchenak.nwk", "Bouchenak.data", 61261515),
        make_tuple("Burleigh.birds.small.nwk", "Burleigh.birds.small.data",
                   mpz_class{"411774618891880337624609803345755559842526093359375"}),
        make_tuple("Caryophyllaceae.nwk", "Caryophyllaceae.data", 718346120625),
        make_tuple("Eucalyptus.nwk.1", "Eucalyptus.data.1", 229),
        make_tuple("Eucalyptus.nwk.2", "Eucalyptus.data.2", 267),
        make_tuple("Eucalyptus.nwk.3", "Eucalyptus.data.3", 9),
        make_tuple("Euphorbia.nwk.1", "Euphorbia.data.1", 759),
        make_tuple("Euphorbia.nwk.2", "Euphorbia.data.2", 759),
        make_tuple("Ficus.nwk.1", "Ficus.data.1", 283815),
        make_tuple("Ficus.nwk.2", "Ficus.data.2", 851445),
        make_tuple("Ficus.nwk.3", "Ficus.data.3", 851445), make_tuple("Iris.nwk", "Iris.data", 1),
        make_tuple("Meredith.mammals.nwk", "Meredith.mammals.data", 1),
        make_tuple("Meusemann.nwk", "Meusemann.data", 1),
        make_tuple("Miadlikowska.fungi.nwk", "Miadlikowska.fungi.data", 11655),
        make_tuple("Misof.insects.nwk", "Misof.insects.data", 1),
        make_tuple("Primula.nwk", "Primula.data", 2835),
        make_tuple("Pyron.nwk", "Pyron.data", 2205),
        make_tuple("Rabosky.scincids.nwk", "Rabosky.scincids.data", 3),
        make_tuple("Ranunculus.nwk", "Ranunculus.data", 3),
        make_tuple("Rhododendron.nwk", "Rhododendron.data", 81),
        make_tuple("Rosaceae.nwk", "Rosaceae.data", mpz_class{"171924246129452853684375"}),
        make_tuple("Shi.bats.nwk", "Shi.bats.data",
                   mpz_class{"242234933881884349476362274404296875"}),
        make_tuple("Solanum.nwk", "Solanum.data", 211865625),
        make_tuple("Springer.primates.nwk", "Springer.primates.data", 70840575),
        make_tuple("Szygium.nwk.1", "Szygium.data.1", 45),
        make_tuple("Szygium.nwk.2", "Szygium.data.2", 45),
        make_tuple("Tolley.chameleons.nwk", "Tolley.chameleons.data", 1),
        make_tuple("Wick.1kp.few.genes.nwk", "Wick.1kp.few.genes.data", 1),
        make_tuple("Wick.1kp.many.genes.nwk", "Wick.1kp.many.genes.data", 1),
        make_tuple("Yang.caryo.1122.nwk", "Yang.caryo.1122.data", 1),
        make_tuple("Yang.caryo.209.nwk", "Yang.caryo.209.data", 1)};

const std::vector<std::tuple<std::string, std::string, index>> small_data_sets{
        make_tuple("Allium_Tiny.nwk", "Allium_Tiny.data", 35),
        make_tuple("Asplenium.nwk.1", "Asplenium.data.1", 1),
        make_tuple("Asplenium.nwk.2", "Asplenium.data.2", 95),
        make_tuple("Eucalyptus.nwk.1", "Eucalyptus.data.1", 229),
        make_tuple("Eucalyptus.nwk.2", "Eucalyptus.data.2", 267),
        make_tuple("Eucalyptus.nwk.3", "Eucalyptus.data.3", 9),
        make_tuple("Euphorbia.nwk.1", "Euphorbia.data.1", 759),
        make_tuple("Euphorbia.nwk.2", "Euphorbia.data.2", 759),
        make_tuple("Iris.nwk", "Iris.data", 1),
        make_tuple("Meredith.mammals.nwk", "Meredith.mammals.data", 1),
        make_tuple("Meusemann.nwk", "Meusemann.data", 1),
        make_tuple("Miadlikowska.fungi.nwk", "Miadlikowska.fungi.data", 11655),
        make_tuple("Misof.insects.nwk", "Misof.insects.data", 1),
        make_tuple("Primula.nwk", "Primula.data", 2835),
        make_tuple("Pyron.nwk", "Pyron.data", 2205),
        make_tuple("Rabosky.scincids.nwk", "Rabosky.scincids.data", 3),
        make_tuple("Ranunculus.nwk", "Ranunculus.data", 3),
        make_tuple("Rhododendron.nwk", "Rhododendron.data", 81),
        make_tuple("Szygium.nwk.1", "Szygium.data.1", 45),
        make_tuple("Szygium.nwk.2", "Szygium.data.2", 45),
        make_tuple("Tolley.chameleons.nwk", "Tolley.chameleons.data", 1),
        make_tuple("Wick.1kp.few.genes.nwk", "Wick.1kp.few.genes.data", 1),
        make_tuple("Wick.1kp.many.genes.nwk", "Wick.1kp.many.genes.data", 1),
        make_tuple("Yang.caryo.1122.nwk", "Yang.caryo.1122.data", 1),
        make_tuple("Yang.caryo.209.nwk", "Yang.caryo.209.data", 1)};

const std::vector<std::tuple<std::string, std::string, index>> medium_data_sets{
        make_tuple("Allium.nwk", "Allium.data", 8038035),
        make_tuple("Allium_Reduced.nwk", "Allium_Reduced.data", 730680125),
        make_tuple("Bouchenak.nwk", "Bouchenak.data", 61261515),
        make_tuple("Caryophyllaceae.nwk", "Caryophyllaceae.data", 718346120625),
        make_tuple("Ficus.nwk.1", "Ficus.data.1", 283815),
        make_tuple("Ficus.nwk.2", "Ficus.data.2", 851445),
        make_tuple("Ficus.nwk.3", "Ficus.data.3", 851445),
        make_tuple("Solanum.nwk", "Solanum.data", 211865625),
        make_tuple("Springer.primates.nwk", "Springer.primates.data", 70840575),
};

loaded_dataset load(std::string tree_file_path, std::string data_file_path) {
	auto tree_file = std::ifstream{"../../data/data/" + tree_file_path};
	auto tree_string = std::string{};
	std::getline(tree_file, tree_string);
	auto data = terraces::parse_nwk(tree_string);

	auto data_file = std::ifstream{"../../data/data/" + data_file_path};
	auto data_res = terraces::parse_bitmatrix(data_file, data.indices, data.tree.size());
	auto& tree = data.tree;
	auto& names = data.names;
	auto& site_occ = data_res.first;
	auto root_species = data_res.second;

	terraces::reroot_inplace(tree, root_species);

	const auto subtrees = terraces::subtrees(tree, site_occ);
	auto constraints = terraces::compute_constraints(subtrees);
	terraces::deduplicate_constraints(constraints);
	auto num_species = terraces::remap_to_leaves(tree, constraints, names, root_species);

	return {data, site_occ, num_species, root_species, subtrees, constraints};
}

} // namespace tests
} // namespace terraces
