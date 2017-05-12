#include "util.h"

#include <assert.h>

/*
 * Create a shallow copy of the given node
 */
static inline rtree_t* copy_node(rtree_t *node) {
	rtree_t* leave = new rtree_t();
	if (node->label == nullptr) {
		node->label = nullptr;
	} else {
		leave->label = new char[strlen(node->label) + 1];
		strcpy(leave->label, node->label);
	}
	leave->parent = node->parent;
	leave->left = node->left;
	leave->right = node->right;

	leave->color = node->color;
	leave->data = node->data;
	leave->leaves = node->leaves;
	leave->length = node->length;
	leave->mark = node->mark;
	return leave;
}

rtree_t* generate_induced_tree(rtree_t *tree, const missingData *missing_data,
		std::map<char*, unsigned char, cmp_str>& species_map,
		size_t partition) {
	if (tree == nullptr) {
		return nullptr;
	}

	if (tree->right == nullptr && tree->left == nullptr) {
		// Leave case
		if (species_map.count(tree->label) == 1
				&& getDataMatrix(missing_data, species_map[tree->label],
						partition)) {
			rtree_t *leave = copy_node(tree);
			leave->leaves = 1;
			leave->left = nullptr;
			leave->right = nullptr;
			leave->parent = nullptr;
			return tree;
		}
	} else {
		// Inner node case
		assert(tree->left != nullptr);
		rtree_t *left = generate_induced_tree(tree->left, missing_data,
				species_map, partition);
		assert(tree->right != nullptr);
		rtree_t *right = generate_induced_tree(tree->right, missing_data,
				species_map, partition);
		//Left and rigt subtrees are included -> common ancestor is included
		if (left != nullptr && right != nullptr) {
			rtree_t *inner_node = copy_node(tree);
			left->parent = inner_node;
			right->parent = inner_node;
			inner_node->leaves = left->leaves + right->leaves;
			inner_node->left = left;
			inner_node->right = right;
			return inner_node;
		} else if (left != nullptr) {
			return left;
		} else if (right != nullptr) {
			return right;
		}
	}

	return nullptr;
}

rtree_t* ntree_to_rtree(ntree_t * root);

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
