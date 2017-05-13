#include <iostream>

#include <terraces/nodes.hpp>

int main() {
	const auto tree = terraces::tree{
		{},
		{0},
		{0},
		{2},
	};

	for (const auto& node : tree) {
		std::cout << node << '\n';
	}

}
