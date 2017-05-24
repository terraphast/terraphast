#include <terraces/subtree_extraction.hpp>

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
	});

	// collect leaves and inner nodes: bitwise and of the children
	foreach_preorder(t, [&](index i) {
		auto node = t[i];
		for (index site = 0; site < num_sites; ++site) {
			auto& out_tree = out_trees[site];
			auto& boundary = tree_boundaries[site];

			bool leaf_occ = is_leaf(node) && node_occ.get(i, site);
			bool inner_occ = !is_leaf(node) && node_occ.get(node.lchild(), site) &&
			                 node_occ.get(node.rchild(), site);
			if (leaf_occ || (inner_occ & !is_root(node))) {
				// TODO: this fires with the Caryophyllaceae-input
				assert(!boundary.empty());
				auto parent = boundary.back();
				out_tree[i].parent() = parent;
				if (out_tree[parent].lchild() == none) {
					out_tree[parent].lchild() = i;
				} else {
					assert(out_tree[parent].rchild() == none);
					out_tree[parent].rchild() = i;
					boundary.pop_back();
				}
			}
			if (inner_occ) {
				boundary.push_back(i);
			}
		}
	});

	return out_trees;
}

} // namespace terraces
