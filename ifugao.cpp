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
        const std::set<leaf_number> &leafs, FILE *file, std::string &root_species_name) {

    auto all_trees = combine_sets(leafs, constraints, root_species_name);

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

	auto to_insert = std::make_shared<Tree>(leaf);
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
		const std::set<leaf_number> &leafs) {

	std::vector<std::shared_ptr<Tree> > result;
	if (leafs.size() == 0) {
		return result;
	}

	auto itr = leafs.begin();
	leaf_number next_leaf = *itr;
	itr++;
	std::set<leaf_number> rest_leafs(itr, leafs.end());

	if (leafs.size() == 1) {
		auto t = std::make_shared<Tree>(next_leaf);
		result.push_back(t);
		return result;
	}

	for (auto t : get_all_binary_trees(rest_leafs)) {
		auto new_trees = add_leaf_to_tree(t, next_leaf);
		result.insert(result.end(), new_trees.begin(), new_trees.end());
	}

	return result;
}

std::vector<std::shared_ptr<Tree> > combine_sets(
		const std::set<leaf_number> &leafs,
        const std::vector<constraint> &constraints, std::string &root_species_name) {

	fprintf(stderr, "INPUT: {");
	bool first = true;
	for (auto &e : leafs) {
		if (first) {
			first = false;
		} else {
			fprintf(stderr, ",");
		}
		fprintf(stderr, "%s", e.c_str());
	}
	fprintf(stderr, "} ");

	fprintf(stderr, "CONSTRAINTS: {");
	bool first2 = true;
	for (auto &c : constraints) {
		if (first2) {
			first2 = false;
		} else {
			fprintf(stderr, ",");
		}
		fprintf(stderr, "(%s,%s)<(%s,%s)", c.smaller_left.c_str(),
				c.smaller_right.c_str(), c.bigger_left.c_str(),
				c.bigger_right.c_str());
	}
	fprintf(stderr, "}");

	if (constraints.empty()) {
		auto result = get_all_binary_trees(leafs);
		fprintf(stderr, "\n");
		return result;
	}

	auto partitions = apply_constraints(leafs, constraints);

	fprintf(stderr, " PARTS: {");
	bool first3 = true;
	for (auto &p : partitions) {
		if (first3) {
			first3 = false;
		} else {
			fprintf(stderr, ",");
		}
		fprintf(stderr, "{");
		bool first4 = true;
		for (auto &e : *p) {
			if (first4) {
				first4 = false;
			} else {
				fprintf(stderr, ",");
			}
			fprintf(stderr, "%s", e.c_str());
		}
		fprintf(stderr, "}");
	}
	fprintf(stderr, "}\n");

	std::vector<std::shared_ptr<Tree> > result;

	for (size_t i = 1; i <= number_partition_tuples(partitions); i++) {
		std::shared_ptr<std::set<leaf_number> > part_left;
		std::shared_ptr<std::set<leaf_number> > part_right;
		std::tie(part_left, part_right) = get_nth_partition_tuple(partitions,
				i);

		auto constraints_left = find_constraints(*part_left, constraints);
		auto constraints_right = find_constraints(*part_right, constraints);

        auto subtrees_left = combine_sets(*part_left, constraints_left, root_species_name);
        auto subtrees_right = combine_sets(*part_right, constraints_right, root_species_name);
		auto trees = merge_subtrees(subtrees_left, subtrees_right);

		fprintf(stderr, "    (L:{");
		bool first5 = true;
		for (auto &e : *part_left) {
			if (first5) {
				first5 = false;
			} else {
				fprintf(stderr, ",");
			}
			fprintf(stderr, "%s", e.c_str());
		}
		fprintf(stderr, "},R:{");
		bool first6 = true;
		for (auto &e : *part_right) {
			if (first6) {
				first6 = false;
			} else {
				fprintf(stderr, ",");
			}
			fprintf(stderr, "%s", e.c_str());
		}
		fprintf(stderr, "})");

		fprintf(stderr, "%lu\n", trees.size());
		result.insert(result.end(), trees.begin(), trees.end());
	}

    for (int i = 0; i < result.size(); i++) {
        if (result[i]->left->label.compare(root_species_name) != 0 && result[i]->right->label.compare(root_species_name) != 0) {
            //result.erase(result.begin() + i);    //this is super inefficient. TODO replace with something efficient
        }
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

static std::tuple<leaf_number, leaf_number> extract_constraints_from_tree_rec(
		const std::shared_ptr<Tree> node,
		std::vector<constraint> &constraints) {

	assert(node != nullptr);

	if (node->is_leaf()) {
		auto this_leaf = node->label;
		return std::make_tuple(this_leaf, this_leaf);
	}

	// (l,r) of the left child node
	leaf_number l_left_most;
	leaf_number l_right_most;
	std::tie(l_left_most, l_right_most) = extract_constraints_from_tree_rec(
			node->left, constraints);

	// (l,r) of the right child node
	leaf_number r_left_most;
	leaf_number r_right_most;
	std::tie(r_left_most, r_right_most) = extract_constraints_from_tree_rec(
			node->right, constraints);

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

std::set<leaf_number> extract_leaf_labels_from_supertree(
		std::shared_ptr<Tree> tree) {

	std::set<leaf_number> result;

	if (tree == nullptr) {
		return result;
	}

	auto result_left = extract_leaf_labels_from_supertree(tree->left);
	result.insert(result_left.begin(), result_left.end());
	auto result_right = extract_leaf_labels_from_supertree(tree->right);
	result.insert(result_right.begin(), result_right.end());

	if (tree->is_leaf()) {
		result.insert(tree->label);
	}

	return result;
}

std::vector<constraint> extract_constraints_from_tree(
		const std::shared_ptr<Tree> supertree) {
	std::vector<constraint> constraints;

	extract_constraints_from_tree_rec(supertree, constraints);

	return constraints;
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
