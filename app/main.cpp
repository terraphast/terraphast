#include "../lib/data.cpp"
#include <iostream>
#include <string>

int main() {
    
    terraces::data::node n0 = {0, -1, -1, -1};

    std::vector<terraces::data::node> tree;
    tree.push_back(n0);
    terraces::data::node n1 = {1, 0, -1, -1};
    tree.push_back(n1);
    terraces::data::node n2 = {2, 0, -1, -1};
    tree.push_back(n2);
    terraces::data::node n3 = {3, 2, -1, -1};
    tree.push_back(n3);

    for(auto node: tree) {
        std::cout << terraces::printable_node(node) << "\n";
    }

    std::cout << "END";


}
