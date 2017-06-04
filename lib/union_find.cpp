#include <terraces/union_find.hpp>

#include <algorithm>
#include <numeric>

namespace terraces {

union_find make_set(index n) {
	std::vector<index> id(n);
	std::vector<index> size(n, 1);
	std::iota(id.begin(), id.end(), index{});
	union_find leaves{std::move(id), std::move(size)};
	return leaves;
}

index find(union_find& leaves, index x) {
	index root = x;
	while (root != leaves.id.at(root)) {
		root = leaves.id.at(root);
	}
	while (x != root) {
		x = std::exchange(leaves.id.at(x), root);
	}
	return root;
}

void merge(union_find& leaves, index x, index y) {
	index i = find(leaves, x);
	index j = find(leaves, y);
	if (i == j) {
		return;
	} else if (leaves.size.at(i) >= leaves.size.at(j)) {
		std::swap(i, j);
	}
	leaves.id.at(i) = j;
	leaves.size.at(j) += leaves.size.at(i);
}

std::vector<std::vector<index>> to_set_of_sets(union_find& leaves) {
	index size = leaves.id.size();
	std::vector<std::vector<index>> set(size);
	for (index i = 0; i < size; i++) {
		set.at(find(leaves, i)).push_back(i);
	}
	set.erase(std::remove_if(set.begin(), set.end(), is_empty()), set.end());
	return set;
}

} // namespace terraces
