#include <terraces/supertree.hpp>

#include <iostream> // TODO: remove me

namespace terraces {

// TODO: remove
std::string
bips_to_string(std::vector<std::tuple<std::vector<std::size_t>, std::vector<std::size_t>>>& bips) {
	std::string res;
	for (std::size_t i = 0; i < bips.size(); i++) {
		for (std::size_t j = 0; j < std::get<0>(bips.at(i)).size(); j++) {
			res += std::to_string(std::get<0>(bips.at(i)).at(j));
			res += ",";
		}
		res += "     ";
		for (std::size_t j = 0; j < std::get<1>(bips.at(i)).size(); j++) {
			res += std::to_string(std::get<1>(bips.at(i)).at(j));
			res += ",";
		}
		res += "\n";
	}
	return res;
}

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

std::vector<struct supertree_node*> construct_supertree(std::vector<index> leaves, constraints c) {
	std::vector<struct supertree_node*> list;

	if (leaves.size() == 1) {
		std::cout << "a leave: " << leaves.at(0) << "\n";
		list.push_back(new_node(leaves));
		return list;
	}

	std::cout << "not a leave: ";
	for (size_t i = 0; i < leaves.size(); i++) {
		std::cout << leaves.at(i);
	}
	std::cout << "\n";

	std::vector<std::vector<index>> sets = apply_constraints(leaves, c);
	std::vector<bipartition> bips = sets_to_bipartitions(sets);

	std::cout << "bips:\n";
	std::cout << bips_to_string(bips);

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
