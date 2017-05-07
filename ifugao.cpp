#include "ifugao.h"
#include "assert.h"

std::vector<binary_tree> merge_subtrees(std::vector<binary_tree> &left, std::vector<binary_tree> &right) {

	std::vector<binary_tree> merged_trees;

	for(binary_tree l: left) {
		for(binary_tree r: right) {
			binary_tree new_tree;
			new_tree.is_leaf = false;
			new_tree.label = NULL;
			new_tree.left_subtree = &l;
			new_tree.right_subtree = &r;
			merged_trees.push_back(new_tree);
		}
	}

	assert(merged_trees.size() == left.size() * right.size());
	return merged_trees;
}

std::vector<std::tr1::shared_ptr<std::set<leaf> > > apply_constraints(const std::set<leaf> &leaves, const std::vector<constraint> &constraints) {
	
	std::vector<std::tr1::shared_ptr<std::set<leaf> > > sets;

	for(leaf l: leaves) {
		// create an empty set for each leave
		std::tr1::shared_ptr<std::set<leaf>> set (new std::set<leaf>);
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
