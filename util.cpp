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

std::shared_ptr<Tree> root_tree(ntree_t *tree, const missingData *missing_data) {
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
        //rtree_t *temp_root = ntree_to_rtree(future_root);
        ntree_t *future_root = get_leaf_by_name(tree,
				missing_data->speciesNames[root_species_number]);

        return root_at(future_root);
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
            if (temp != nullptr) {
                return temp;
            }
        }
    }
    return nullptr;
}

std::shared_ptr<Tree> root_at(ntree_t *leaf) {
    //if leaf->parent is null, leaf is the root => the tree is not binary, or the node is no leaf
    assert(leaf->parent != nullptr);
    ntree_t *neighbour = leaf->parent;
    std::shared_ptr<Tree> root = std::make_shared<Tree>();
    std::shared_ptr<Tree> new_leaf = std::make_shared<Tree>();
    std::shared_ptr<Tree> new_neighbour = std::make_shared<Tree>();

    //initialize root
    root->label = "";
    root->left = new_leaf;
    root->right = new_neighbour;
    root->label = "";

    //initialize new_leaf and new_neighbour
    new_leaf->parent = root;
    new_neighbour->parent = root;

    recursive_root(new_leaf, leaf, neighbour);
    recursive_root(new_neighbour, neighbour, leaf);
    return root;
}

void recursive_root(std::shared_ptr<Tree> current,  ntree_t *current_ntree, ntree_t *parent) {
    //when the children_count is 3, we are at the pseudoroot of the unrooted binary tree
    assert(current_ntree->children_count == 2 || current_ntree->children_count == 0 ||
           (current_ntree->children_count == 3 && current_ntree->parent == nullptr));

    if (current_ntree->label == nullptr) {
        current->label = "";
    } else {
        current->label = current_ntree->label;
    }

    if (current_ntree->children_count == 0) {   //is leaf
        return;
    }

    current->left = std::make_shared<Tree>();
    current->right = std::make_shared<Tree>();

    ntree_t *left_ntree;
    ntree_t *right_ntree;

    if (current_ntree->children_count == 3) {   //we are at the pseudoroot
        //set the children, leave out the parent
        if (current_ntree->children[0] == parent) {
            left_ntree = current_ntree->children[1];
            right_ntree = current_ntree->children[2];
        } else if (current_ntree->children[1] == parent) {
            left_ntree = current_ntree->children[0];
            right_ntree = current_ntree->children[2];
        } else if (current_ntree->children[2] == parent) {
            left_ntree = current_ntree->children[0];
            right_ntree = current_ntree->children[1];
        } else {
            assert(false);
        }
    } else {
        assert(current_ntree->children_count == 2);
        //set the children, leave out the parent
        if(current_ntree->children[0] == parent) {
            left_ntree = current_ntree->children[1];
            right_ntree = current_ntree->parent;
        } else if (current_ntree->children[1] == parent) {
            left_ntree = current_ntree->children[0];
            right_ntree = current_ntree->parent;
        } else if (current_ntree->parent == parent) {
            left_ntree = current_ntree->children[0];
            right_ntree = current_ntree->children[1];
        } else {
            assert(false);
        }

    }

    recursive_root(current->left, left_ntree, current_ntree);
    recursive_root(current->right, right_ntree, current_ntree);
}
