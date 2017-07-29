#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <terraces/parser.hpp>
#include <terraces/rooting.hpp>
#include <terraces/trees.hpp>

#include "../lib/trees_impl.hpp"

int main(int argc, char* argv[]) {
	if (argc != 3) {
		return 1;
	}
	auto tree_file = std::ifstream{argv[1]};
	auto output_file = std::ofstream{argv[2]};
	auto tree_string = std::string{};
	int i = 0;
	do {
		std::getline(tree_file, tree_string);
		if (tree_string.empty()) {
			continue;
		}
		auto tree_set = terraces::parse_nwk(tree_string);
		terraces::index root = (terraces::index)std::distance(
		        tree_set.names.begin(),
		        std::find_if(tree_set.names.begin(), tree_set.names.end(),
		                     [](const auto& el) { return !el.empty(); }));
		terraces::reroot_inplace(tree_set.tree, root);
		terraces::print_tree_dot_unrooted(tree_set.tree, tree_set.names, output_file,
		                                  std::to_string(i) + "_");
		++i;
		output_file << "\n";
	} while (!tree_file.eof());
}
