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
			if (i != none) {
				++degree;
			}
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

std::ostream& operator<<(std::ostream& s, newick_t tree_pair) {
	const auto& t = *tree_pair.t;
	const auto& names = *tree_pair.names;
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
			stack.pop();
		}
	}
	return s << ';';
}

std::vector<index> preorder(const tree& t) {
	std::vector<index> result;
	foreach_preorder(t, [&](index i) { result.push_back(i); });
	return result;
}

std::vector<index> postorder(const tree& t) {
	std::vector<index> result;
	foreach_postorder(t, [&](index i) { result.push_back(i); });
	return result;
}

void print_tree_dot_unrooted(const tree& t, const name_map& names, std::ostream& output,
                             std::string name_prefix) {
	output << "strict graph {\n";
	index i = 0;
	for (auto n : t) {
		if (is_root(n)) {
			index u = n.lchild();
			index v = n.rchild();
			if (names[u] == "") {
				output << "\"" << name_prefix << u << "\"";
			} else {
				output << "\"" << name_prefix << names[u] << "\"";
			}
			output << " -- ";
			if (names[v] == "") {
				output << "\"" << name_prefix << v << "\"";
			} else {
				output << "\"" << name_prefix << names[v] << "\"";
			}
			output << ";\r\n";
		} else {
			for (auto v : n.data) {
				if (v != none && !is_root(t[v])) {
					if (names[i] == "") {
						output << "\"" << name_prefix << i << "\"";
					} else {
						output << "\"" << name_prefix << names[i] << "\"";
					}
					output << " -- ";
					if (names[v] == "") {
						output << "\"" << name_prefix << v << "\"";
					} else {
						output << "\"" << name_prefix << names[v] << "\"";
					}
					output << ";\r\n";
				}
			}
			if (names[i] == "") {
				output << "\"" << name_prefix << i << "\" [shape=point];\r\n";
			} else {
				output << "\"" << name_prefix << names[i] << "\" [label=\""
				       << names[i] << "\"];\r\n";
			}
		}
		++i;
	}
	output << "}";
}

void print_tree_dot(const tree& t, const name_map& names, std::ostream& output) {
	output << "graph {\n";
	index i = 0;
	for (auto n : t) {
		for (auto v : n.data) {
			if (v != none) {
				if (names[i] == "") {
					output << i;
				} else {
					output << "\"" << names[i] << "\"";
				}
				output << " -- ";
				if (names[v] == "") {
					output << v;
				} else {
					output << "\"" << names[v] << "\"";
				}
				output << ";\r\n";
			}
		}
		if (names[i] == "") {
			output << i << " [shape=point];\r\n";
		}
		++i;
	}
	output << "}";
}

void print_tree_gml(const tree& t, const name_map& names, std::ostream& output) {
	output << "graph [\n";
	for (index i = 0; i < t.size(); ++i) {
		output << "\tnode [\n\t\tid " << i << "\n";
		if (names[i] != "") {
			output << "\t\tlabel \"" << names[i] << "\"\n";
		}
		output << "\t]\n";
	}
	index i = 0;
	for (auto n : t) {
		for (auto v : n.data) {
			if (v != none) {
				output << "\tedge [\n\t\tsource " << i << "\n\t\ttarget " << v
				       << "\n\t]\n";
			}
		}
		++i;
	}
	output << "]";
}

} // namespace terraces
