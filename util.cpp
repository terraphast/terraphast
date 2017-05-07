#include "util.h"

#include <assert.h>

rtree_t * ntree_to_rtree(ntree_t * root);

rtree_t* root_tree(ntree_t *tree, const missingData *missing_data) {
	long root_species_number = -1;
	for (size_t i = 0; i < missing_data->numberOfSpecies; i++) {
		bool contains_all_data = true;
		for (size_t k = 0; k < missing_data->numberOfPartitions; k++) {
			if (getDataMatrix(missing_data, i, k) == 0) {
				contains_all_data = false;
				break;
			}
		}
		if (contains_all_data) {
			root_species_number = i;
			break;
		}
	}
	if (root_species_number != -1) {
		ntree_t *temp = get_leaf_by_name(tree,
				missing_data->speciesNames[root_species_number]);
		return ntree_to_rtree(temp);
	} else {
		//tree cannot be rooted consistently
		return NULL;
	}
}

ntree_t* get_leaf_by_name(ntree_t *tree, char *label) {
	assert(tree != nullptr);
	assert(label != nullptr);
	if (tree->label != nullptr && strcmp(tree->label, label) == 0) {
		return tree;
	} else {
		for (int i = 0; i < tree->children_count; i++) {
			ntree_t *temp = get_leaf_by_name(tree->children[i], label);
			if (temp != NULL) {
				return temp;
			}
		}
	}
	return NULL;
}
