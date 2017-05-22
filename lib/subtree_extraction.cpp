#include <terraces/subtree_extraction.hpp>

namespace terraces {

std::vector<tree> subtrees(const tree& t, const bitmatrix& occ, const std::vector<index> species) {
	auto num_species = occ.rows();
	auto num_sites = occ.cols();
	assert(t.size() == 2 * num_species - 2);
	assert(num_species == species.size());
	assert(is_rooted_tree(t));

	std::vector<tree> out_trees{num_sites, tree{}};

	bitmatrix node_occ{t.size(), num_sites};

	for (index i = 0; i < species.size(); ++i) {
		node_occ.copy_row(i, species[i], occ);
	}

	foreach_postorder(t, [&](index i) {
		auto lchild = t[i].lchild();
		auto rchild = t[i].rchild();
		node_occ.row_or(lchild, rchild, i);
		for (index site = 0; site < num_sites; ++site) {
			if (node_occ.get(lchild, site) && node_occ.get(rchild, site)) {
				// TODO insert node in the tree for site
			}
			if (!node_occ.get(i, site)) {
				// TODO ignore this branch from here on for site
			}
		}

		// TODO work out clever indexing scheme for subtrees ;)
	});

	return out_trees;
}

} // namespace terraces
