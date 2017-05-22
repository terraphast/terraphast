#include "ifugao.h"

#include <assert.h>
#include <iostream>
#include <terraces.h>
#include <map>
#include <string>

std::tuple<std::shared_ptr<leaf_set>,
		std::shared_ptr<leaf_set> > get_nth_partition_tuple(
		std::vector<std::shared_ptr<leaf_set> > &partitions,
		size_t n) {

	auto part_one = std::make_shared<leaf_set>();
	auto part_two = std::make_shared<leaf_set>();

	assert(n > 0 && n <= number_partition_tuples(partitions));

	for (size_t i = 0; i < partitions.size(); i++) {
		if (is_bit_set(n, i)) {
			part_one->insert(partitions[i]->begin(), partitions[i]->end());
		} else {
			part_two->insert(partitions[i]->begin(), partitions[i]->end());
		}
	}

	return std::make_tuple(part_one, part_two);
}

size_t list_trees(const std::vector<constraint> &constraints,
		const leaf_set &leaves, bool count_only, FILE *file) {
	auto all_trees = combine_sets(leaves, constraints);
	
	if (file != nullptr) {
		for (std::shared_ptr<Tree> t : all_trees) {
			fprintf(file, "%s\n", t->to_newick_string().c_str());
		}
	}
	return all_trees.size();

}

static std::shared_ptr<Tree> deep_copy(std::shared_ptr<Tree> tree,
		std::map<std::shared_ptr<Tree>, std::shared_ptr<Tree>> &cover_map) {
	if (tree == nullptr) {
		return nullptr;
	}
	if (cover_map.count(tree) > 0) {
		return cover_map[tree];
	}

	auto node = std::make_shared<Tree>();
	node->label = tree->label;
	cover_map[tree] = node;
	cover_map[node] = node;

	node->parent = deep_copy(tree->parent, cover_map);
	node->left = deep_copy(tree->left, cover_map);
	node->right = deep_copy(tree->right, cover_map);

	return node;
}

static std::shared_ptr<Tree> deep_copy(std::shared_ptr<Tree> tree) {
	std::map<std::shared_ptr<Tree>, std::shared_ptr<Tree>> cover_map;
	return deep_copy(tree, cover_map);
}

static std::shared_ptr<Tree> root(std::shared_ptr<Tree> t) {
	if (t->parent == nullptr) {
		return t;
	}

	return root(t->parent);
}

static std::vector<std::shared_ptr<Tree> > add_leaf_to_tree(
		std::shared_ptr<Tree> current_tree, const leaf_number leaf) {

	std::vector<std::shared_ptr<Tree> > result;

	if (!current_tree->is_leaf()) {
		auto result_left = add_leaf_to_tree(current_tree->left, leaf);
		result.insert(result.end(), result_left.begin(), result_left.end());
		auto result_right = add_leaf_to_tree(current_tree->right, leaf);
		result.insert(result.end(), result_right.begin(), result_right.end());
	}

	auto tree_copy = deep_copy(current_tree);

	auto to_insert = std::make_shared<Tree>(std::to_string(leaf));
	auto new_tree = std::make_shared<Tree>(tree_copy, to_insert);
	new_tree->parent = tree_copy->parent;
	if (tree_copy->parent != nullptr) {
		if (tree_copy->parent->left == tree_copy) {
			tree_copy->parent->left = new_tree;
		} else {
			tree_copy->parent->right = new_tree;
		}
	}
	tree_copy->parent = new_tree;
	to_insert->parent = new_tree;

	result.push_back(root(new_tree));

	return result;
}

std::vector<std::shared_ptr<Tree> > get_all_binary_trees(
		const leaf_set &leaves) {
	std::vector<std::shared_ptr<Tree> > result;
	if (leaves.size() == 0) {
		return result;
	}

	auto itr = leaves.begin();
	leaf_number next_leaf = *itr;
	itr++;
	leaf_set rest_leaves(itr, leaves.end());

	if (leaves.size() == 1) {
		auto t = std::make_shared<Tree>(std::to_string(next_leaf));
		result.push_back(t);
		return result;
	}

	for (auto t : get_all_binary_trees(rest_leaves)) {
		auto new_trees = add_leaf_to_tree(t, next_leaf);
		result.insert(result.end(), new_trees.begin(), new_trees.end());
	}

	return result;
}

std::vector<std::shared_ptr<Tree> > combine_sets(
		const leaf_set &leaves,
		const std::vector<constraint> &constraints) {
	{ // debug output
		fprintf(stderr, "Input: {");
		bool first = true;
		for (auto &elem : leaves) {
			if (!first) {
				fprintf(stderr, ",");
			}
			first = false;
			fprintf(stderr, "%d", elem);
		}
		fprintf(stderr, "} ");
	}
	
	{ // possible recursion stops
		if(leaves.size() <= 2) {
			if(leaves.size() == 2) {
				//TODO create Tree and return
			} else {
				//TODO error (should never happen)
			}
		}
		
		if (constraints.empty()) {
			auto result = get_all_binary_trees(leaves);
			fprintf(stderr, "END %lu\n", result.size());
			return result;
		}
	}
	
	// all requirments fulfilled, get partitions by applying constraints to leaves
	auto partitions = apply_constraints(leaves, constraints);
	
	{// debug output
		fprintf(stderr, "Partitions: {");
		bool first = true;
		for (auto &one_set : partitions) {
			if (!first) {
				fprintf(stderr, ",");
			}
			first = false;
			fprintf(stderr, "{");
			bool first_inner = true;
			for (auto &elem : *one_set) {
				if (!first_inner) {
					fprintf(stderr, ",");
				}
				first_inner = false;
				fprintf(stderr, "%d", elem);
			}
			fprintf(stderr, "}");
		}
		fprintf(stderr, "]\n");
	}
	
	std::vector<std::shared_ptr<Tree> > result;
	
	for (size_t i = 1; i <= number_partition_tuples(partitions); i++) {
		std::shared_ptr<leaf_set> part_left;
		std::shared_ptr<leaf_set> part_right;
		std::tie(part_left, part_right) = get_nth_partition_tuple(partitions, i);

		auto constraints_left = find_constraints(*part_left, constraints);
		auto constraints_right = find_constraints(*part_right, constraints);

		auto subtrees_left = combine_sets(*part_left, constraints_left);
		auto subtrees_right = combine_sets(*part_right, constraints_right);
		auto trees = merge_subtrees(subtrees_left, subtrees_right);
		result.insert(result.end(), trees.begin(), trees.end());
	}

	return result;
}

std::vector<std::shared_ptr<Tree> > merge_subtrees(
		std::vector<std::shared_ptr<Tree> > &left,
		std::vector<std::shared_ptr<Tree> > &right) {

	std::vector<std::shared_ptr<Tree> > merged_trees;

	for (std::shared_ptr<Tree> &l : left) {
		for (std::shared_ptr<Tree> &r : right) {
			auto new_tree = std::make_shared<Tree>(l, r);
			l->parent = new_tree;
			r->parent = new_tree;
			merged_trees.push_back(new_tree);
		}
	}

	assert(merged_trees.size() == left.size() * right.size());
	return merged_trees;
}

std::vector<std::shared_ptr<leaf_set> > apply_constraints(
		const leaf_set &leaves,
		const std::vector<constraint> &constraints) {

	std::vector<std::shared_ptr<leaf_set> > sets;

	for (leaf_number l : leaves) {
		// create an empty set for each leaf
		std::shared_ptr<leaf_set> set(new leaf_set);
		set->insert(l);
		sets.push_back(set);
	}

	bool found_left_constraint = false;
	bool found_right_constraint = false;
	size_t index_containing_left_constraint = 0;
	size_t index_containing_right_constraint = 0;

	for (constraint cons : constraints) {
		for (size_t i = 0; i < sets.size(); i++) {
			if (sets[i]->find(cons.smaller_left) != sets[i]->end()) {
				// set contains the left constraint
				found_left_constraint = true;
				index_containing_left_constraint = i;
			}
			if (sets[i]->find(cons.smaller_right) != sets[i]->end()) {
				// set contains the right constraint
				found_right_constraint = true;
				index_containing_right_constraint = i;
			}
		}
		assert(
				found_left_constraint
						&& index_containing_left_constraint < sets.size());
		assert(
				found_right_constraint
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

static void extract_label_leave_mapping(const std::shared_ptr<Tree> node,
		std::map<std::string, leaf_number>& mapping, leaf_number& counter) {
	if (node != nullptr && node->is_leaf()) {
		mapping[node->label] = counter;
		counter++;
	} else {
		extract_label_leave_mapping(node->left, mapping, counter);
		extract_label_leave_mapping(node->right, mapping, counter);
	}
}

static std::tuple<leaf_number, leaf_number> extract_constraints_from_supertree_rec(
		const std::shared_ptr<Tree> node,
		std::map<std::string, leaf_number>& mapping,
		std::vector<constraint> &constraints) {

	assert(node != nullptr);

	if (node->is_leaf()) {
		auto this_Leaf = mapping[node->label];
		return std::make_tuple(this_Leaf, this_Leaf);
	}

// (l,r) of the left child node
	leaf_number l_left_most;
	leaf_number l_right_most;
	std::tie(l_left_most, l_right_most) =
			extract_constraints_from_supertree_rec(node->left, mapping,
					constraints);

// (l,r) of the right child node
	leaf_number r_left_most;
	leaf_number r_right_most;
	std::tie(r_left_most, r_right_most) =
			extract_constraints_from_supertree_rec(node->right, mapping,
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

std::tuple<leaf_set, std::vector<constraint> > extract_constraints_from_supertree(
		const std::shared_ptr<Tree> supertree) {
	std::vector<constraint> constraints;

	std::map<std::string, leaf_number> mapping;
	leaf_number counter = 0;
	extract_label_leave_mapping(supertree, mapping, counter);

	extract_constraints_from_supertree_rec(supertree, mapping, constraints);

	leaf_set leaves;
	for (const auto& pair : mapping) {
		leaves.insert(pair.second);
	}

	return std::make_tuple(leaves, constraints);
}

std::vector<constraint> find_constraints(const leaf_set &leaves,
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
