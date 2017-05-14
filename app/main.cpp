#include <fstream>
#include <iomanip>
#include <iostream>

#include <terraces/parser.hpp>
#include <terraces/trees.hpp>

int main(int argc, char** argv) try {
	if (argc != 3) {
		return 1;
	}
	auto tree_file = std::ifstream{argv[1]};
	auto tree_string = std::string{};
	std::getline(tree_file, tree_string);
	const auto data = terraces::parse_nwk(tree_string);

	auto data_file = std::ifstream{argv[2]};
	const auto mat = terraces::parse_bitmatrix(data_file, data.indices, data.tree.size());

	for (auto i = terraces::index{}; i < data.tree.size(); ++i) {
		std::cout << std::setw(3) << i << ": " << std::setw(1) << std::noboolalpha;
		for (auto j = terraces::index{}; j < mat.cols(); ++j) {
			std::cout << mat.get(i, j) << ' ';
		}
		std::cout << '(' << std::setw(3) << data.tree.at(i).parent() << ") : " << data.names.at(i) << '\n';
	}
} catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << '\n';
}
