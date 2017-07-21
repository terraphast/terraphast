#include <bitset>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>

#include <terraces/simple.hpp>

int main(int argc, char** argv) try {
	auto tree_file_name = std::string{};
	auto data_file_name = std::string{};
	if (argc == 2) {
		tree_file_name = argv[1] + std::string{".nwk"};
		data_file_name = argv[1] + std::string{".data"};
	} else if (argc == 3) {
		tree_file_name = argv[1];
		data_file_name = argv[2];
	} else {
		std::cerr << "Usage: \n"
		          << argv[0] << " <tree-file> <occurrence file>\n"
		          << argv[0] << " <common-basename>\n";
		return 1;
	}
	const auto on_terrace = terraces::is_on_terrace_from_file(tree_file_name, data_file_name);
	std::cout << "Tree is on terrace: " << on_terrace << '\n';
	const auto terraces_count =
	        terraces::get_terrace_size_as_bigint_from_file(tree_file_name, data_file_name);
	std::cout << "There are " << terraces_count << " trees on the terrace.\n\n\n";

	terraces::print_terrace_from_file(tree_file_name, data_file_name, std::cout);
	std::cout << '\n';
} catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << "\n";
}
