#include <bitset>
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

using terraces::tree_enumerator;
using terraces::multitree_callback;
using terraces::logging_decorator;
using terraces::utils::as_comma_separated_output;
using terraces::full_set;

int main(int argc, char** argv) try {
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <tree-file> <occurrence file>" << std::endl;
		return 1;
	}
	auto tree_file = std::ifstream{argv[1]};
	auto tree_string = std::string{};
	std::getline(tree_file, tree_string);
	auto data = terraces::parse_nwk(tree_string);

	auto data_file = std::ifstream{argv[2]};
	const auto data_res = terraces::parse_bitmatrix(data_file, data.indices, data.tree.size());
	const auto num_nodes = data_res.first.rows();
	auto num_species = data.indices.size();
	const auto num_sites = data_res.first.cols();
	auto& tree = data.tree;
	auto& names = data.names;
	auto& site_occ = data_res.first;
	auto root_species = data_res.second;
	std::cout << "Input data: " << num_species << " species with " << num_sites << " sites ("
	          << num_nodes << " nodes)\n";

	if (root_species == terraces::none) {
		std::cout << "Cannot find a suitable root for the tree\n";
		return 1;
	}
	std::cout << "New root: " << root_species << std::endl;

	terraces::reroot_inplace(tree, root_species);

	std::cout << "In Newick-Format:\n" << as_newick(tree, names) << "\n";

	const auto subtrees = terraces::subtrees(tree, site_occ);
	std::cout << "Subtrees in Newick format:\n";
	for (auto& subtree : subtrees) {
		std::cout << as_newick(subtree, names) << "\n";
	}
	auto constraints = terraces::compute_constraints(subtrees);
	auto num_constraints = constraints.size();

	std::cout << "Constraints:\n"
	          << as_comma_separated_output(full_set(num_constraints), constraints, names)
	          << "\n";

	auto duplicate = terraces::deduplicate_constraints(constraints);
	num_constraints = constraints.size();
	std::cout << "Deleted " << duplicate << " unnecessary constraints, " << num_constraints
	          << " remaining\n";

	std::cout << "Remaining constraints:\n"
	          << as_comma_separated_output(full_set(num_constraints), constraints, names)
	          << "\n";

	num_species = terraces::remap_to_leaves(tree, constraints, names, root_species);

	std::cout << "Supertree:\n";
	multitree_callback cb{std::cout, names};
	tree_enumerator<decltype(cb)> enumerator{cb};
	enumerator.run(num_species, constraints, root_species);

} catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << "\n";
}
