#include <terraces/bitvector.hpp>
#include <terraces/validation.hpp>

#include <numeric>
#include <unordered_map>

#include <algorithm>
#include <numeric>

#include "supertree_helpers.hpp"

namespace terraces {

std::vector<bitvector> tree_bipartitions(const tree& t, const std::vector<index>& mapping,
                                         const fast_index_set& leaves) {
	std::vector<bitvector> bips(t.size(), {0});
	std::vector<bitvector> subtrees(t.size(), {leaves.size()});
	foreach_postorder(t, [&](index i) {
		auto n = t[i];
		if (is_leaf(n)) {
			subtrees[i].set(leaves.rank(mapping[i]));
		} else {
			subtrees[i].set_bitwise_or(subtrees[n.lchild()], subtrees[n.rchild()]);
		}
	});
	foreach_preorder(t, [&](index i) {
		auto n = t[i];
		bool at_root = is_root(n);
		bool at_rhs_of_root =
		        !at_root && (is_root(t[n.parent()]) && t[n.parent()].rchild() == i);
		if (!(at_root || at_rhs_of_root)) {
			if (subtrees[i].get(0)) {
				subtrees[i].invert();
			}
			bips[i] = std::move(subtrees[i]);
		}
	});
	bips[t[0].rchild()] = std::move(subtrees[t[0].rchild()]);
	bips[t[0].rchild()].blank();
	bips[0] = std::move(subtrees[0]);
	bips[0].blank();
	std::sort(bips.begin(), bips.end());
	return bips;
}

bool is_isomorphic(const tree_set& fst, const tree_set& snd) {
	assert(fst.tree.size() == snd.tree.size());

	auto leaves = leave_occ(fst.tree);
	std::vector<index> mapping(fst.tree.size());
	std::iota(mapping.begin(), mapping.end(), 0);
	auto fst_bip = tree_bipartitions(fst.tree, mapping, leaves);
	for (index i = 0; i < snd.tree.size(); ++i) {
		if (is_leaf(snd.tree[i])) {
			const std::string& name = snd.names[i];
			mapping[i] = fst.indices.at(name);
		}
	}
	auto snd_bip = tree_bipartitions(snd.tree, mapping, leaves);

	return std::equal(fst_bip.begin(), fst_bip.end(), snd_bip.begin(), snd_bip.end());
}

} // namespace terraces
