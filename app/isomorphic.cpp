#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <terraces/parser.hpp>
#include <terraces/rooting.hpp>
#include <terraces/trees.hpp>
#include <vector>

#include "../lib/validation.hpp"

using namespace terraces;

std::vector<tree_set> read_trees(std::string file) {
	std::vector<tree_set> trees;
	auto tree_file = std::ifstream{file};
	auto tree_string = std::string{};
	do {
		std::getline(tree_file, tree_string);
		if (tree_string.empty()) {
			continue;
		}
		trees.push_back(parse_nwk(tree_string));
	} while (!tree_file.eof());

	return trees;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		return 1;
	}
	auto fst = read_trees(argv[1]);
	auto snd = read_trees(argv[2]);
	std::cout << "pairwise comparison\n";
	std::cout << std::noboolalpha;
	for (auto& t_fst : fst) {
		for (auto& t_snd : snd) {
			std::cout << is_isomorphic(t_fst, t_snd) << " ";
		}
		std::cout << "\n";
	}
	std::cout << "comparison inside first group\n";
	for (auto& t_fst : fst) {
		for (auto& t_snd : fst) {
			std::cout << is_isomorphic(t_fst, t_snd) << " ";
		}
		std::cout << "\n";
	}
	std::cout << "comparison inside second group\n";
	for (auto& t_fst : snd) {
		for (auto& t_snd : snd) {
			std::cout << is_isomorphic(t_fst, t_snd) << " ";
		}
		std::cout << "\n";
	}
}
