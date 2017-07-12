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
using terraces::variants::count_callback;
using terraces::debug::variants::stack_state_decorator;

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
	auto num_species = data.indices.size();
	auto& tree = data.tree;
	auto& names = data.names;
	auto& site_occ = data_res.first;
	auto root_species = data_res.second;

	if (root_species == terraces::none) {
		std::cerr << "Cannot find a suitable root for the tree\n";
		return 1;
	}
	terraces::reroot_inplace(tree, root_species);
	const auto subtrees = terraces::subtrees(tree, site_occ);
	auto constraints = terraces::compute_constraints(subtrees);
	terraces::deduplicate_constraints(constraints);
	num_species = terraces::remap_to_leaves(tree, constraints, names, root_species);

	tree_enumerator<stack_state_decorator<count_callback>> enumerator{
	        {}, num_species, constraints.size()};
	std::thread status_thread([&]() {
		using namespace std::chrono_literals;
		auto& stack = enumerator.callback().stack();
		while (stack.empty()) {
			std::this_thread::sleep_for(100ms);
		}
		while (!stack.empty()) {
			std::this_thread::sleep_for(100ms);
			auto end = stack.end();
			auto begin = stack.begin();
			auto depth = 0u;
			std::cout << "\033[2J\033[;H";
			for (auto it = begin; it != end; ++it) {
				std::cout << std::setw(20) << it->current_bip << "/"
				          << std::setw(20) << it->max_bip << std::setw(40)
				          << it->intermediate << std::string(depth, ' ')
				          << (it->right ? '\\' : '|') << "\n";
				depth += it->right;
			}
			std::cout << std::flush;
		}
	});

	std::cout << enumerator.run(num_species, constraints, root_species);

} catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << "\n";
}
