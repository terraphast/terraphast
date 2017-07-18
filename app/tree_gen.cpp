#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <unordered_set>
#include <vector>

using index = std::size_t;
constexpr static auto none = std::numeric_limits<index>::max();

std::default_random_engine& get_rng() {
	thread_local auto rng = [] {
		const auto seed = std::random_device{}();
		return std::default_random_engine{seed};
	}();
	return rng;
}

index rand_index(index max) {
	auto dist = std::uniform_int_distribution<index>{0u, max};
	return dist(get_rng());
}

struct tree_node {
	index left = none;
	index right = none;
	index parent = none;
	std::uint8_t visited = 0u;

	void set_children(index l, index r) {
		left = l;
		right = r;
	}

	bool is_leaf() const { return left == none and right == none; }
};

using tree = std::vector<tree_node>;
using index_list = std::vector<index>;

index extract_index(index_list& list, index i) {
	assert(i < list.size());
	const auto ret = list[i];
	list[i] = list.back();
	list.pop_back();
	return ret;
}

index extract_random(index_list& list) { return extract_index(list, rand_index(list.size() - 1u)); }

index extract_random(index_list& l1, index_list& l2) {
	const auto s1 = l1.size();
	const auto s2 = l2.size();
	const auto st = s1 + s2;
	auto i = rand_index(st - 1u);
	if (i < s1) {
		return extract_index(l1, i);
	} else {
		return extract_index(l2, i - s1);
	}
}

std::array<index, 3u> random_family(index_list& free_nodes, index_list& middle_nodes) {
	assert(free_nodes.size() + middle_nodes.size() >= 3u);
	assert(free_nodes.size() >= 1u);
	const auto parent = extract_random(free_nodes);
	const auto c1 = extract_random(free_nodes, middle_nodes);
	const auto c2 = extract_random(free_nodes, middle_nodes);
	middle_nodes.push_back(parent);
	return {{parent, c1, c2}};
}

void link(index parent, index lchild, index rchild, tree& t, index_list& leafs) {
	t[parent].left = lchild;
	t[lchild].parent = parent;
	t[parent].right = rchild;
	t[rchild].parent = parent;
	if (t[lchild].is_leaf()) {
		leafs.push_back(lchild);
	}
	if (t[rchild].is_leaf()) {
		leafs.push_back(rchild);
	}
}

void print(std::ostream& out, tree& t, const std::vector<std::string>& names) {
	auto current = index{};
	while (current != none) {
		auto& cur = t[current];
		switch (cur.visited) {
		case 0:
			if (cur.is_leaf()) {
				out << names[current];
				current = cur.parent;
			} else {
				out << "(";
				current = cur.left;
			}
			break;
		case 1:
			out << ", ";
			current = cur.right;
			break;
		case 2:
			out << ')' << names[current];
			current = cur.parent;
			break;
		default:
			throw std::logic_error{"WAT?"};
		}
		++cur.visited;
	}
}

int main(int argc, char** argv) {
	if (argc < 2) {
		return 1;
	}
	const auto num_leafs = index{std::stoul(argv[1])};
	if (num_leafs < 2) {
		return 2;
	}
	// enforce num_leafs >= 1
	const auto num_nodes = (num_leafs * 2u) - 1u;
	while (true) {
		auto t = tree(num_nodes);
		auto free_nodes = index_list(num_nodes - 1u);
		std::iota(free_nodes.begin(), free_nodes.end(), 1u);
		auto leafs = index_list{};
		auto middle_nodes = index_list{};
		while (free_nodes.size() + middle_nodes.size() > 2u and not free_nodes.empty()) {
			const auto family = random_family(free_nodes, middle_nodes);
			link(family[0], family[1], family[2], t, leafs);
		}
		if (free_nodes.empty()) {
			continue;
		}
		const auto cl = extract_random(free_nodes, middle_nodes);
		const auto cr = extract_random(free_nodes, middle_nodes);
		link(0u, cl, cr, t, leafs);
		free_nodes.clear();
		auto names = std::vector<std::string>(num_nodes);
		for (auto i = index{}; i < leafs.size(); ++i) {
			names[leafs[i]] = "s" + std::to_string(i);
		}
		print(std::cout, t, names);
		std::cout << '\n';
		break;
	}
}
