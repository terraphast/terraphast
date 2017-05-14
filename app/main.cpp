#include <fstream>
#include <iostream>

#include <terraces/parser.hpp>
#include <terraces/trees.hpp>

int main(int argc, char** argv) {
	for (int i = 1; i < argc; ++i) {
		std::cout << "reading file " << argv[i] << "\n";
		auto line = std::string{};
		auto file = std::ifstream{argv[i]};
		while (std::getline(file, line)) {
			auto data = terraces::parse_nwk(line);
			for (auto i = terraces::index{}; i < data.tree.size(); ++i) {
				std::cout << '\t' << i << " (" << data.names.at(i)
				          << ") : " << data.tree.at(i).parent() << "\n";
			}
		}
	}
}
