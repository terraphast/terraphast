#include <terraces/union_find.hpp>

#include <algorithm>
#include <numeric>

namespace terraces {

union_find::union_find(index n) : id(n), size(n, 1) { std::iota(id.begin(), id.end(), index{}); }

index union_find::find(index x) const {
	index root = x;
	while (root != id.at(root)) {
		root = id.at(root);
	}
	while (x != root) {
		x = std::exchange(id.at(x), root);
	}
	return root;
}

void union_find::merge(index x, index y) {
	index i = find(x);
	index j = find(y);
	if (i == j) {
		return;
	} else if (size.at(i) >= size.at(j)) {
		std::swap(i, j);
	}
	id.at(i) = j;
	size.at(j) += size.at(i);
}

} // namespace terraces
