#include <fstream>
#include <iostream>
#include <sstream>

#include <terraces/parser.hpp>
#include <terraces/rooting.hpp>
#include <terraces/subtree_extraction.hpp>
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

	std::stringstream ss;
	ss << argv[1] << ".gml";
	std::ofstream output{ss.str()};
	terraces::print_tree_gml(data.tree, data.names, output);
} catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
}
