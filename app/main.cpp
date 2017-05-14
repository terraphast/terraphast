#include <iostream>
#include <fstream>

#include <terraces/nodes.hpp>
#include <terraces/parser.hpp>

int main(int argc, char** argv) {
	for (int i = 1; i < argc; ++i) {
		std::cout << "reading file " << argv[i] << "\n";
		auto line = std::string{};
		auto file = std::ifstream{argv[i]};
		while(std::getline(file, line)) {
			auto data = terraces::parse_nwk(line);
			for (auto i = terraces::index{}; i < data.first.size(); ++i) {
				std::cout
					<< i << " (" << data.second.at(i)
					<< ") : " << data.first.at(i).data.at(0) << "\n";
			}
		}
	}

}
