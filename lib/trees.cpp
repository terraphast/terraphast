#include <algorithm>
#include <ostream>
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

	return is_root(t[0]);
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

std::ostream& operator<<(std::ostream& ss, const tree& t) {
	ss << "TREE [\n";
	for (auto node : t) {
		ss << "\t" << node << "\n";
	}
	ss << "]\n";
	return ss;
}

std::ostream& operator<<(std::ostream& s, std::pair<const tree&, const name_map&> tree_pair) {
	const auto& t = tree_pair.first;
	const auto& names = tree_pair.second;
	using state = std::pair<index, std::uint8_t>;
	auto stack = std::stack<state, std::vector<state>>{};
	stack.emplace(0u, 0u);
	while (not stack.empty()) {
		const auto i = stack.top().first;
		auto& visited = stack.top().second;
		if (is_leaf(t.at(i))) {
			s << names.at(i);
			stack.pop();
		} else if (visited == 0u) {
			visited = 1u;
			s << '(';
			stack.emplace(t.at(i).lchild(), 0);
		} else if (visited == 1u) {
			visited = 2u;
			s << ", ";
			stack.emplace(t.at(i).rchild(), 0);
		} else if (visited == 2u) {
			s << ')' << names.at(i);
		}
	}
	return s << ';';
}

} // namespace terraces
