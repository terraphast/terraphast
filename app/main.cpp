#include <fstream>
#include <iomanip>
#include <iostream>

#include <terraces/constraints.hpp>
#include <terraces/parser.hpp>
#include <terraces/rooting.hpp>
#include <terraces/subtree_extraction.hpp>
#include <terraces/supertree.hpp>
#include <terraces/trees.hpp>

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
	const auto& mat = data_res.first;
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

	for (auto i = terraces::index{}; i < data.tree.size(); ++i) {
		std::cout << std::setw(3) << i << ": " << std::setw(1) << std::noboolalpha;
		for (auto j = terraces::index{}; j < mat.cols(); ++j) {
			std::cout << mat.get(i, j) << ' ';
		}
		std::cout << '(' << std::setw(3) << data.tree.at(i).parent()
		          << ") : " << data.names.at(i) << '\n';
	}

	std::cout << "In Newick-Format:\n" << as_newick(data.tree, data.names) << std::endl;

	terraces::constraints constraints;
	const auto subtrees = terraces::subtrees(data.tree, data_res.first);
	constraints = terraces::compute_constraints(subtrees);
	auto duplicated = terraces::deduplicate_constraints(constraints);
	std::cout << "Deleted " << duplicated << " unnecessary constraints, " << constraints.size()
	          << " remaining" << std::endl;
	terraces::tree_master tm;
	std::cout << "We counted " << tm.count_supertree(data.tree, constraints)
	          << " equivalent trees" << std::endl;

} catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
}
