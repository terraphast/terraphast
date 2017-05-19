#include "ifugao.h"

#include <assert.h>
#include <terraces.h>
#include <map>

long list_trees(const std::vector<constraint> &constraints,
		const std::set<leaf_number> &leaves, bool count_only, FILE &file) {

	std::vector<Tree> all_trees = combine_sets(
			apply_constraints(leaves, constraints), constraints);

	if (!count_only) {
		// TODO: print all trees to file
	}
	return all_trees.size();
}

std::vector<Tree> combine_sets(
		const std::vector<std::shared_ptr<std::set<leaf_number> > > &set_array,
		const std::vector<constraint> &constraints) {
	std::shared_ptr<std::set<leaf_number> > left_set(new std::set<leaf_number>);
	std::shared_ptr<std::set<leaf_number> > right_set(new std::set<leaf_number>);
	std::vector<Tree> found_trees;
	//initialize right set

	//TODO
//	for(std::shared_ptr<std::set<Leaf> > set: set_array) {
//		right_set->insert(set);
//	}
//	auto right_iterator = right_set.begin();
//	auto right_end = right_set.end();
//	std::advance(right_end, -1);
//
//	while(right_iterator!=right_end) {
//		left_set.insert(*right_iterator);
//		right_set.erase(right_iterator);
//		combine_sets_subcall(set_array, constraints, &found_trees);
//	}

	/*
	 std::set<Leaf> set_to_move = set_array[current];
	 left_set.insert(set_to_move);
	 right_set.er
	 }
	 */

// index of last element
	long last = set_array.size() - 1;
	//TODO
//	auto left_set.end();
	// first loop, fill array completely
	/* Second loop, iterate until end is maximum value, then increase previous
	 * value. 1 always stays in the set. This results in all combinations
	 *       v-[max!]       v-[increase!]       v-[max!]
	 * 1 2 3 5       -> 1 2 4            -> 1 2 5
	 *   v-[increase!]     v-[increase!]     v-[max!]
	 * 1 3            -> 1 4            -> 1 5       -> STOP
	 */
	bool stop = false;

	while (!stop) {

		//TODO uncomment
		//combine_sets_subcall(set_array, constraints, found_trees, left_set, right_set);
	}

	return found_trees;
}

//TODO
/*combine_sets_subcall(const std::vector<std::shared_ptr<std::set<Leaf> > > &set_array,
 const std::vector<constraint> &constraints, std::vector<Tree> &found_trees,
 std::shared_ptr<std::set<Leaf> > left_set, std::shared_ptr<std::set<Leaf> > right_set) {
 find_constraints()


 left_subtrees = combine_sets_subcall(set_array, constraints, left_set);
 right_subtrees = combine_sets_subcall(set_array, constraints, right_set);

 found_trees.push_back(merge_subtrees(left_subtrees, right_subtrees));
 }*/

std::vector<std::shared_ptr<Tree> > merge_subtrees(std::vector<std::shared_ptr<Tree> > &left,
		std::vector<std::shared_ptr<Tree> > &right) {

	std::vector<std::shared_ptr<Tree> > merged_trees;

	for (std::shared_ptr<Tree> &l : left) {
		for (std::shared_ptr<Tree> &r : right) {
			auto new_tree = std::make_shared<Tree>(l, r);
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
