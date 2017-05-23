#include <terraces/constraints.hpp>

namespace terraces {

constraints compute_constraints(const std::vector<tree>& trees) {
	constraints result;
	std::vector<std::pair<index, index>> outermost_nodes;

	for (auto t : trees) {
		// collect outermost nodes for each subtree (these have lca i)
		foreach_postorder(t, [&](index i) {
			auto node = t[i];
			if (is_leaf(node)) {
				outermost_nodes[i].first = i;
				outermost_nodes[i].second = i;
			} else {
				outermost_nodes[i].first = outermost_nodes[node.lchild()].first;
				outermost_nodes[i].second = outermost_nodes[node.rchild()].second;
			}
		});

		// extract constraints for each edge
		foreach_preorder(t, [&](index i) {
			auto node = t[i];
			if (!is_leaf(node)) {
				// TODO extract constraint for left and right child
			}
		});
	}

	return result;
}

} // namespace terraces
