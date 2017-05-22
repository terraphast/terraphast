#include <algorithm>
#include <iostream>
#include <terraces/trees.hpp>

namespace terraces {

bool is_rooted_tree(const tree& t) {
	if (!is_valid_tree(t)) {
		return false;
	}

	// check if parent/child assignment is symmetric
	for (index i = 0; i < t.size(); ++i) {
		if (is_leaf(t[i])) {
			continue;
		}

		auto lc = t[i].lchild();
		auto rc = t[i].rchild();
		assert(lc < t.size());
		assert(rc < t.size());
		if (t[lc].parent() != i || t[rc].parent() != i) {
			return false;
		}
	}

	return is_root(t[t.size() - 1]);
}

bool is_valid_tree(const tree& t) {
	index n_edges = 0;
	bool found_root = false;
	// first: degree check
	for (auto& n : t) {
		index degree = 0;
		for (auto i : n.data) {
			if (i != none)
				++degree;
		}
		if (degree == 0) {
			return false;
		}
		// we must not have multiple roots
		if (degree == 2) {
			if (found_root) {
				return false;
			}
			found_root = true;
		}
		n_edges += degree;
	}

	// second: number of edges
	if (n_edges % 2 != 0) {
		// must be even
		return false;
	}
	n_edges /= 2;
	if (n_edges != t.size() - 1) {
		return false;
	}

	// third: reverse-edge for each edge
	for (index i = 0; i < t.size(); ++i) {
		for (index m = 0; m < 3; ++m) {
			index j = t[i].data[m];
			if (j == none) {
				continue;
			}
			assert(j < t.size());
			// we have the edge i->j and look for j->i
			auto nj = t[j].data;
			if (std::count(nj.begin(), nj.end(), i) != 1) {
				return false;
			}
		}
	}

	return true;
}

} // namespace terraces
