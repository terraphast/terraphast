#include "ifugao.h"

#include <assert.h>
#include <terraces.h>
#include <map>

long list_trees(const std::vector<constraint> &constraints,
		const std::set<leaf_number> &leaves, bool count_only, FILE &file) {

	std::vector<binary_tree> all_trees = combine_sets(
			apply_constraints(leaves, constraints), constraints);

	if (!count_only) {
		// TODO: print all trees to file
	}
	return all_trees.size();
}

std::vector<binary_tree> combine_sets(
		const std::vector<std::shared_ptr<std::set<leaf_number> > > &set_array,
		const std::vector<constraint> &constraints) {
	//TODO
	return std::vector<binary_tree>();
}

std::vector<binary_tree> merge_subtrees(std::vector<binary_tree> &left,
		std::vector<binary_tree> &right) {

	std::vector<binary_tree> merged_trees;

	for (binary_tree &l : left) {
		for (binary_tree &r : right) {
			binary_tree new_tree;
			new_tree.is_leaf = false;
			new_tree.label = nullptr;
			new_tree.left_subtree = &l;
			new_tree.right_subtree = &r;
			merged_trees.push_back(new_tree);
		}
	}

	assert(merged_trees.size() == left.size() * right.size());
	return merged_trees;
}

std::vector<std::shared_ptr<std::set<leaf_number> > > apply_constraints(
		const std::set<leaf_number> &leaves,
		const std::vector<constraint> &constraints) {

	std::vector<std::shared_ptr<std::set<leaf_number> > > sets;

	for (leaf_number l : leaves) {
		// create an empty set for each leave
		std::shared_ptr<std::set<leaf_number>> set(new std::set<leaf_number>);
		set->insert(l);
		sets.push_back(set);
	}

	int index_containing_left_constraint;
	int index_containing_right_constraint;

	for (constraint cons : constraints) {
		for (int i = 0; i < sets.size(); i++) {
			if (sets[i]->find(cons.smaller_left) != sets[i]->end()) {
				// set contains the left constraint
				index_containing_left_constraint = i;
			}
			if (sets[i]->find(cons.smaller_right) != sets[i]->end()) {
				// set contains the right constraint
				index_containing_right_constraint = i;
			}
		}
		assert(
				index_containing_left_constraint >= 0
						&& index_containing_left_constraint < sets.size());
		assert(
				index_containing_right_constraint >= 0
						&& index_containing_right_constraint < sets.size());
		if (index_containing_left_constraint
				!= index_containing_right_constraint) {
			// sets need to be merged
			sets[index_containing_left_constraint]->insert(
					sets[index_containing_right_constraint]->begin(),
					sets[index_containing_right_constraint]->end());
			sets.erase(sets.begin() + index_containing_right_constraint);
		}
	}
	return sets;
}

static void extract_label_leave_mapping(const binary_tree* node,
		std::map<char*, leaf_number, cmp_str>& mapping, leaf_number& counter) {

	if (node != nullptr && node->is_leaf) {
		mapping[node->label] = counter;
		counter++;
	} else {
		extract_label_leave_mapping(node->left_subtree, mapping, counter);
		extract_label_leave_mapping(node->right_subtree, mapping, counter);
	}
}

static std::tuple<leaf_number, leaf_number> extract_constraints_from_supertree_rec(
		const binary_tree *node, std::map<char*, leaf_number, cmp_str>& mapping,
		std::vector<constraint> &constraints) {

	assert(node != nullptr);

	if (node->is_leaf) {
		auto this_leaf_number = mapping[node->label];
		return std::make_tuple(this_leaf_number, this_leaf_number);
	}

	// (l,r) of the left child node
	leaf_number l_left_most;
	leaf_number l_right_most;
	std::tie(l_left_most, l_right_most) =
			extract_constraints_from_supertree_rec(node->left_subtree, mapping,
					constraints);

	// (l,r) of the right child node
	leaf_number r_left_most;
	leaf_number r_right_most;
	std::tie(r_left_most, r_right_most) =
			extract_constraints_from_supertree_rec(node->right_subtree, mapping,
					constraints);

	if (l_left_most != l_right_most) {
		constraint c;
		c.smaller_left = l_left_most;
		c.smaller_right = l_right_most;
		c.bigger_left = l_left_most;
		c.bigger_right = r_right_most;
		constraints.push_back(c);
	}

	if (r_left_most != r_right_most) {
		constraint c;
		c.smaller_left = r_left_most;
		c.smaller_right = r_right_most;
		c.bigger_left = l_left_most;
		c.bigger_right = r_right_most;
		constraints.push_back(c);
	}

	return std::make_tuple(l_left_most, r_right_most);
}

std::tuple<std::set<leaf_number>, std::vector<constraint> > extract_constraints_from_supertree(
		const binary_tree *supertree) {
	std::set<leaf_number> leaf_numbers;
	std::vector<constraint> constraints;

	std::map<char*, leaf_number, cmp_str> mapping;
	leaf_number counter = 0;
	extract_label_leave_mapping(supertree, mapping, counter);

	extract_constraints_from_supertree_rec(supertree, mapping, constraints);

	std::set<leaf_number> leafs;
	for (const auto& pair : mapping) {
	    leafs.insert(pair.second);
	}

	return std::make_tuple(leafs, constraints);
}

std::vector<constraint> find_constraints(const std::set<leaf_number> &leaves,
		const std::vector<constraint> &constraints) {

	std::vector<constraint> valid_constraints;

	for (constraint cons : constraints) {
		if (cons.smaller_left == cons.bigger_left) {
			if (leaves.find(cons.smaller_left) != leaves.end()
					&& leaves.find(cons.smaller_right) != leaves.end()
					&& leaves.find(cons.bigger_right) != leaves.end()) {
				// constraint is valid on leaf set
				valid_constraints.push_back(cons);
			}
		} else {
			// smaller_right == bigger_right
			if (leaves.find(cons.smaller_left) != leaves.end()
					&& leaves.find(cons.smaller_right) != leaves.end()
					&& leaves.find(cons.bigger_left) != leaves.end()) {
				// constraint is valid on leaf set
				valid_constraints.push_back(cons);
			}
		}
	}
	return valid_constraints;
}
