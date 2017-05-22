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
	for (size_t i = 0; i < leaves.size(); i++) {
		m[leaves.at(i)] = i;
	}
	for (size_t i = 0; i < c.size(); i++) {
		auto& constraint = c.at(i);
		constraint.left = m[constraint.left];
		constraint.right = m[constraint.right];
		constraint.shared = m[constraint.shared];
	}
	return c;
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

bool check_supertree(index number, constraints c) {
	std::vector<index> leaves(number);
	for (size_t i = 0; i < number; i++) {
		leaves.at(i) = i;
	}
	return check_supertree(leaves, c);
}

bool check_supertree(std::vector<index> leaves, constraints c) {
	// only one tree possible for two leaves
	if (leaves.size() <= 2) {
		return false;
	}

	// on a terrace if more than two leaves and no contraints
	if (c.size() == 0) {
		return true;
	}

	constraints new_c = map_constraints(leaves, c);
	std::vector<std::vector<index>> sets = apply_constraints(leaves.size(), new_c);
	sets = map_sets(leaves, sets);
	std::vector<bipartition> bips = sets_to_bipartitions(sets);

	// on a terrace if more than one bipartition
	if (bips.size() > 1) {
		return true;
	}

	std::vector<index> left_set = std::get<0>(bips.at(0));
	std::vector<index> right_set = std::get<1>(bips.at(0));

	constraints left_bips = filter_constraints(left_set, c);
	constraints right_bips = filter_constraints(right_set, c);

	if (check_supertree(left_set, left_bips)) {
		return true;
	} else {
		return check_supertree(right_set, right_bips);
	}
}

size_t count_supertree(index number, constraints c) {
	std::vector<index> leaves(number);
	for (size_t i = 0; i < number; i++) {
		leaves.at(i) = i;
	}
	return count_supertree(leaves, c);
}

size_t count_supertree(std::vector<index> leaves, constraints c) {
	size_t number = 0;

	if (leaves.size() == 1) {
		return 1;
	}

	if (c.size() == 0) {
		size_t res = 1;
		for (size_t i = 3; i <= leaves.size() + 1; i++) {
			res *= (2 * i - 5);
		}
		return res;
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

		number += count_supertree(left_set, left_bips);
		number += count_supertree(right_set, right_bips);
	}

	return number - bips.size();
}

std::vector<struct supertree_node*> construct_supertree(index number, constraints c) {
	std::vector<index> leaves(number);
	for (size_t i = 0; i < number; i++) {
		leaves.at(i) = i;
	}
	return construct_supertree(leaves, c);
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
