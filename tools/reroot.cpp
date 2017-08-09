#include <fstream>

#include <terraces/parser.hpp>
#include <terraces/rooting.hpp>
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
	auto& tree = data.tree;
	auto& names = data.names;
	auto root_species = data_res.second;

	if (root_species == terraces::none) {
		std::cerr << "Cannot find a suitable root for the tree\n";
		return 1;
	}

	terraces::reroot_inplace(tree, root_species);

	std::cout << as_newick(tree, names);
} catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << "\n";
}
