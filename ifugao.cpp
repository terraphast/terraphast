#include "ifugao.h"

#include <assert.h>
#include <terraces.h>
#include <map>
#include <string>

std::tuple<std::shared_ptr<std::set<leaf_number> >,
		std::shared_ptr<std::set<leaf_number> > > get_nth_partition_tuple(
		std::vector<std::shared_ptr<std::set<leaf_number> > > &partitions,
		size_t n) {

	auto part_one = std::make_shared<std::set<leaf_number> >();
	auto part_two = std::make_shared<std::set<leaf_number> >();

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
		const std::set<leaf_number> &leafs, bool count_only, FILE &file) {

	auto all_trees = combine_sets(leafs, constraints);

//	if (!count_only) {
		for (std::shared_ptr<Tree> t : all_trees) {
			d_print_tree(t);
		}
//	}
	return all_trees.size();

}

static std::vector<std::shared_ptr<Tree> > get_all_binary_trees(
		const std::set<leaf_number> &leafs) {
	std::vector<std::shared_ptr<Tree> > result;
	if (leafs.size() == 1) {
		result.push_back(
				std::make_shared<Tree>(std::to_string(*leafs.begin())));
		return result;
	}
	if (leafs.size() == 2) {
		auto leaf_itr = leafs.begin();
		auto leaf_left = std::make_shared<Tree>(std::to_string(*leaf_itr));
		leaf_itr++;
		auto leaf_right = std::make_shared<Tree>(std::to_string(*leaf_itr));
		auto tree = std::make_shared<Tree>(leaf_left, leaf_right);
		leaf_left->parent = tree;
		leaf_right->parent = tree;
		result.push_back(tree);
		return result;
	}
	for (auto it = ++leafs.begin(); it != leafs.end(); ++it) {
		std::set<leaf_number> leftList(leafs.begin(), it);
		std::set<leaf_number> rightList(it, leafs.end());

		auto left = get_all_binary_trees(leftList);
		auto right = get_all_binary_trees(rightList);

		auto subtrees = merge_subtrees(left, right);
		result.insert(result.end(), subtrees.begin(), subtrees.end());
	}
	return result;
}

std::vector<std::shared_ptr<Tree> > combine_sets(
		const std::set<leaf_number> &leafs,
		const std::vector<constraint> &constraints) {

	fprintf(stderr, "Input: {");
	bool first = true;
	for (auto &elem : leafs) {
		if (!first) {
			fprintf(stderr, ",");
		}
		first = false;
		fprintf(stderr, "%d", elem);
	}
	fprintf(stderr, "} ");

	if (constraints.empty()) {
		auto result = get_all_binary_trees(leafs);
		fprintf(stderr, "END %lu\n", result.size());
		return result;
	}

	auto partitions = apply_constraints(leafs, constraints);

	fprintf(stderr, "Partitions: {");
	bool first2 = true;
	for (auto &one_set : partitions) {
		if (!first2) {
			fprintf(stderr, ",");
		}
		first2 = false;
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

	std::vector<std::shared_ptr<Tree> > result;

	for (size_t i = 1; i <= number_partition_tuples(partitions); i++) {
		std::shared_ptr<std::set<leaf_number> > part_left;
		std::shared_ptr<std::set<leaf_number> > part_right;
		std::tie(part_left, part_right) = get_nth_partition_tuple(partitions,
				i);

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

std::tuple<std::set<leaf_number>, std::vector<constraint> > extract_constraints_from_supertree(
		const std::shared_ptr<Tree> supertree) {
	std::set<leaf_number> Leafs;
	std::vector<constraint> constraints;

	std::map<std::string, leaf_number> mapping;
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
