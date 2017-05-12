#include "ifugao.h"
#include "assert.h"


long list_trees(const std::vector<constraint> &constraints, const std::set<leaf_number> &leaves, bool count_only, FILE &file) {

	std::vector<binary_tree> all_trees = combine_sets(apply_constraints(leaves, constraints), constraints);

	if(!count_only) {
    		// TODO: print all trees to file
	}
	return all_trees.size();
}

std::vector<binary_tree> combine_sets(const std::vector<std::shared_ptr<std::set<leaf_number> > > &set_array, const std::vector<constraint> &constraints) {
	//TODO
	return std::vector<binary_tree> ();
}

std::vector<binary_tree> merge_subtrees(std::vector<binary_tree> &left, std::vector<binary_tree> &right) {

	std::vector<binary_tree> merged_trees;

	for(binary_tree &l: left) {
		for(binary_tree &r: right) {
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

std::vector<std::shared_ptr<std::set<leaf_number> > > apply_constraints(const std::set<leaf_number> &leaves, const std::vector<constraint> &constraints) {
	
	std::vector<std::shared_ptr<std::set<leaf_number> > > sets;

	for(leaf_number l: leaves) {
		// create an empty set for each leave
		std::shared_ptr<std::set<leaf_number>> set (new std::set<leaf_number>);
		set->insert(l);
		sets.push_back(set);
	}


	int index_containing_left_constraint;
	int index_containing_right_constraint;

	for(constraint cons: constraints) {
		for(int i = 0; i < sets.size(); i++) {
			if(sets[i]->find(cons.smaller_left) != sets[i]->end()) {
				// set contains the left constraint
				index_containing_left_constraint = i;
			}
			if(sets[i]->find(cons.smaller_right) != sets[i]->end()) {
				// set contains the right constraint
				index_containing_right_constraint = i;
			}
		}
		assert(index_containing_left_constraint >= 0 && index_containing_left_constraint < sets.size());
		assert(index_containing_right_constraint >= 0 && index_containing_right_constraint < sets.size());
		if(index_containing_left_constraint != index_containing_right_constraint) {
			// sets need to be merged
			sets[index_containing_left_constraint]->insert(sets[index_containing_right_constraint]->begin(), sets[index_containing_right_constraint]->end());
			sets.erase(sets.begin() + index_containing_right_constraint);
		}
	}
	return sets;
}

std::vector<constraint> find_constraints(const std::set<leaf_number> &leaves, const std::vector<constraint> &constraints) {
	
	std::vector<constraint> valid_constraints;

	for(constraint cons: constraints) {
		if(cons.smaller_left == cons.bigger_left) {
			if(leaves.find(cons.smaller_left) != leaves.end()
				&& leaves.find(cons.smaller_right) != leaves.end()
				&& leaves.find(cons.bigger_right) != leaves.end()) {
				// constraint is valid on leaf set
				valid_constraints.push_back(cons);
			}
		} else {
			// smaller_right == bigger_right
			if(leaves.find(cons.smaller_left) != leaves.end()
				&& leaves.find(cons.smaller_right) != leaves.end()
				&& leaves.find(cons.bigger_left) != leaves.end()) {
				// constraint is valid on leaf set
				valid_constraints.push_back(cons);
			}
		}
	}
	return valid_constraints;
}
