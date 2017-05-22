#include <terraces/supertree.hpp>

#include <map>

namespace terraces {

struct supertree_node* new_node(std::vector<index> leaves) {
	struct supertree_node* temp = new supertree_node;
	temp->leaves = leaves;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

std::string pretty_preorder(struct supertree_node* s) {
	std::string res = "";
	if (s != NULL) {
		for (size_t i = 0; i < s->leaves.size(); i++) {
			res += std::to_string(s->leaves.at(i));
		}
		res += " ";
		res += pretty_preorder(s->left);
		res += pretty_preorder(s->right);
	}
	return res;
}

constraints map_constraints(std::vector<index> leaves, constraints c) {
	std::map<index, index> m;
	constraints res = c;
	for (size_t i = 0; i < leaves.size(); i++) {
		m[leaves.at(i)] = i;
	}
	for (size_t i = 0; i < c.size(); i++) {
		res.at(i).shared = m[c.at(i).shared];
		res.at(i).left = m[c.at(i).left];
		res.at(i).right = m[c.at(i).right];
	}
	return res;
}

std::vector<std::vector<index>> map_sets(std::vector<index> leaves,
                                         std::vector<std::vector<index>> sets) {
	std::vector<std::vector<index>> res;
	for (size_t i = 0; i < sets.size(); i++) {
		std::vector<index> set;
		for (size_t j = 0; j < sets.at(i).size(); j++) {
			set.push_back(leaves.at(sets.at(i).at(j)));
		}
		res.push_back(set);
	}
	return res;
}

std::vector<struct supertree_node*> construct_supertree(std::vector<index> leaves, constraints c) {
	std::vector<struct supertree_node*> list;

	if (leaves.size() == 1) {
		list.push_back(new_node(leaves));
		return list;
	}

	constraints new_c = map_constraints(leaves, c);
	std::vector<std::vector<index>> sets = apply_constraints(leaves.size(), new_c);
	sets = map_sets(leaves, sets);
	std::vector<bipartition> bips = sets_to_bipartitions(sets);

	for (size_t i = 0; i < bips.size(); i++) {
		std::vector<index> left_set = std::get<0>(bips.at(i));
		std::vector<index> right_set = std::get<1>(bips.at(i));

		constraints left_bips = filter_constraints(left_set, c);
		constraints right_bips = filter_constraints(right_set, c);

		std::vector<struct supertree_node*> left_supertree =
		        construct_supertree(left_set, left_bips);
		std::vector<struct supertree_node*> right_supertree =
		        construct_supertree(right_set, right_bips);

		for (size_t j = 0; j < left_supertree.size(); j++) {
			struct supertree_node* left = left_supertree.at(j);
			for (size_t k = 0; k < right_supertree.size(); k++) {
				struct supertree_node* right = right_supertree.at(k);

				struct supertree_node* node =
				        new_node(leaves); // making leaves as root
				node->left = left;        // connect left subtree
				node->right = right;      // connect right subtree
				list.push_back(node);     // add this tree to list
			}
		}
	}
	return list;
}

} // namespace terraces
