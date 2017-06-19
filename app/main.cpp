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
	auto num_nodes = data_res.first.rows();
	auto num_species = data.names.size();
	auto num_sites = data_res.first.cols();
	std::cout << "Input data: " << num_species << " species with " << num_sites << " sites ("
	          << num_nodes << " nodes)" << std::endl;

	if (data_res.second == terraces::none) {
		std::cout << "Cannot find a suitable root for the tree" << std::endl;
		return 1;
	}
	std::cout << "New root: " << data_res.second << std::endl;

	terraces::reroot_inplace(data.tree, data_res.second);

	std::cout << "In Newick-Format:\n" << as_newick(data.tree, data.names) << std::endl;

	const auto subtrees = terraces::subtrees(data.tree, data_res.first);
	std::cout << "Subtrees in Newick format:\n";
	for (auto& subtree : subtrees) {
		std::cout << as_newick(subtree, data.names) << "\n";
	}
	auto constraints = terraces::compute_constraints(subtrees);

	std::cout << as_comma_separated_output(terraces::full_set(constraints.size()), constraints,
	                                       data.names);

	auto duplicated = terraces::deduplicate_constraints(constraints);
	std::cout << "Deleted " << duplicated << " unnecessary constraints, " << constraints.size()
	          << " remaining" << std::endl;

	std::cout << as_comma_separated_output(terraces::full_set(constraints.size()), constraints,
	                                       data.names);

	logging_decorator<multitree_callback> cb{{std::cout, data.names}, std::cerr, data.names};
	tree_enumerator<decltype(cb)> enumerator{cb};
	std::cout << "(";
	enumerator.run(data.tree, constraints, data_res.second);
	std::cout << "," << data.names[data_res.second] << ")\n";

} catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
}
