#include <terraces/constraints.hpp>

#include <algorithm>
#include <array>
#include <ostream>

#include <terraces/union_find.hpp>

namespace terraces {

bool operator==(const constraint& c1, const constraint& c2) {
	return std::tie(c1.shared, c1.left, c1.right) == std::tie(c2.shared, c2.left, c2.right);
}

std::ostream& operator<<(std::ostream& s, const constraint& c) {
	s << "lca(" << c.left << "," << c.shared << ") < lca(" << c.shared << "," << c.right << ")";
	return s;
}

constraints filter_constraints(const std::vector<index>& leaves, const constraints& c_list) {
	constraints new_c;
	// This may not be the nicest code, but it is a tad faster than using std::find thrice;
	// further improvements are of course always nice.
	//
	// Please keep in mind that this function is called with very small input-sizes most
	// of the time, so using std::unordered_set easily add a factor of ten to the typical
	// execution.
	for (const auto& c : c_list) {
		auto needles = std::array<index, 3>{{c.shared, c.left, c.right}};
		auto it = std::find_first_of(leaves.begin(), leaves.end(), needles.begin(),
		                             needles.end());
		if (it == leaves.end()) {
			continue;
		} else if (*it == needles[0]) {
			needles[0] = needles[2];
		} else if (*it == needles[1]) {
			needles[1] = needles[2];
		}
		it = std::find_first_of(std::next(it), leaves.end(), needles.begin(),
		                        needles.begin() + 2);
		if (it == leaves.end()) {
			continue;
		}
		const auto final_needle = (*it == needles[0]) ? needles[1] : needles[0];
		if (std::find(std::next(it), leaves.end(), final_needle) == leaves.end()) {
			continue;
		}
		new_c.push_back(c);
	}
	return new_c;
}

std::vector<std::vector<index>> apply_constraints(index number, const constraints& c) {
	union_find leaves = make_set(number);
	for (size_t i = 0; i < c.size(); i++) {
		merge(leaves, c.at(i).shared, c.at(i).left);
	}
	return to_set_of_sets(leaves);
}

constraints compute_constraints(const std::vector<tree>& trees) {
	constraints result;
	auto num_nodes = trees[0].size();
	std::vector<std::pair<index, index>> outermost_nodes{num_nodes, std::make_pair(none, none)};

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
				auto lchild = node.lchild();
				auto rchild = node.rchild();
				auto lnode = t[lchild];
				auto rnode = t[rchild];
				// leftmost descendant of i
				auto i1 = outermost_nodes[i].first;
				// rightmost descendant of lchild of i
				auto i2 = outermost_nodes[lchild].second;
				// leftmost descendant of rchild of i
				auto i3 = outermost_nodes[rchild].first;
				// rightmost descendant of i
				auto i4 = outermost_nodes[i].second;

				// if the left edge is an inner edge
				if (!is_leaf(lnode)) {
					result.emplace_back(i2, i1, i4);
				}
				// if the right edge is an inner edge
				if (!is_leaf(rnode)) {
					result.emplace_back(i3, i4, i1);
				}
			}
		});
	}

	return result;
}

} // namespace terraces
