#include <terraces/subtree_extraction.hpp>

#include <iomanip>
#include <iostream>

using std::vector;
using std::stack;
using std::pair;
using std::make_pair;

namespace terraces {

std::vector<tree> subtrees(const tree& t, const bitmatrix& occ) {
	auto num_nodes = occ.rows();
	auto num_sites = occ.cols();
	assert(t.size() == num_nodes);
	assert(is_rooted_tree(t));

	vector<tree> out_trees(num_sites, tree(num_nodes, node{}));
	vector<vector<index>> tree_boundaries{num_sites, vector<index>{}};

	auto node_occ = occ;

	// compute occurrences on inner nodes: bitwise or of the children
	foreach_postorder(t, [&](index i) {
		auto node = t[i];
		if (!is_leaf(node)) {
			node_occ.row_or(node.lchild(), node.rchild(), i);
		}
		std::cout << std::setw(4) << i << " ";
		for (index site = 0; site < num_sites; ++site) {
			std::cout << (node_occ.get(i, site) * 1);
		}
		std::cout << "\n";
	});

	// collect leaves and inner nodes: bitwise and of the children
	foreach_preorder(t, [&](index i) {
		auto node = t[i];
		std::cout << "visiting " << i << " " << node << "\n";
		for (index site = 0; site < num_sites; ++site) {
			auto& out_tree = out_trees[site];
			auto& boundary = tree_boundaries[site];
			std::cout << "site " << site << " boundary: {";
			for (auto v : boundary) {
				std::cout << " " << v;
			}
			std::cout << "} , ";

			bool leaf_occ = is_leaf(node) && node_occ.get(i, site);
			bool inner_occ = !is_leaf(node) && node_occ.get(node.lchild(), site) &&
			                 node_occ.get(node.rchild(), site);
			std::cout << (leaf_occ ? "leaf at site " : (inner_occ ? "inner node" : ""))
			          << "\n";
			if (leaf_occ || (inner_occ & !is_root(node))) {
				// TODO: this fires with the Caryophyllaceae-input
				assert(!boundary.empty());
				auto parent = boundary.back();
				out_tree[i].parent() = parent;
				std::cout << "we insert a node with parent " << parent
				          << " on the ";
				if (out_tree[parent].lchild() == none) {
					out_tree[parent].lchild() = i;
					std::cout << "left";
				} else {
					assert(out_tree[parent].rchild() == none);
					out_tree[parent].rchild() = i;
					std::cout << "right and are done with " << parent;
					boundary.pop_back();
				}
				std::cout << "\n";
			}
			if (inner_occ) {
				std::cout << "we insert " << i << " into the boundary\n";
				boundary.push_back(i);
			}
		}
	});

	return out_trees;
}

} // namespace terraces
