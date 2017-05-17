#include <terraces/union_find.hpp>

namespace terraces {

union_find make_set(std::size_t n) {
        std::vector<std::size_t> id;
        std::vector<std::size_t> size;
        for (std::size_t i = 0; i < n; i++) {
                id.push_back(i);
                size.push_back(1);
        }
        union_find leaves {
                id,
                size
        };
        return leaves;
}

index find(union_find& leaves, index x) {
        index root = x;
        while (root != leaves.id.at(root)) {
                root = leaves.id.at(root);
        }
        while (x != root) {
                index new_x = leaves.id.at(x);
                leaves.id.at(x) = root;
                x = new_x;
        }
        return root;
}

void merge(union_find& leaves, index x, index y) {
        index i = find(leaves, x);
        index j = find(leaves, y);
        if (i == j) return;
        if (leaves.size.at(i) < leaves.size.at(j)) {
                leaves.id.at(i) = j;
                leaves.size.at(j) += leaves.size.at(i);
        } else  {
                leaves.id.at(j) = i;
                leaves.size.at(i) += leaves.size.at(j);
        }
}

} // namespace terraces
