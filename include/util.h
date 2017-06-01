#ifndef UTIL_H
#define UTIL_H

//this file contains useful function that don't belong anywhere else

#include "newick-tools/newick-tools.h"
#include "input_parser.h"
#include "terraces.h"

#include <cstring>
#include <map>
#include <memory>
#include <string>
#include <iostream>
#include <vector>

/**
 * This funktion generates the trees T|G_i, which is exactly the subtree of T induced by partition G_i
 * @param tree the rooted binary supertree
 * @param missing_data the data matrix representing the missing partition data.
 * @param partition the index i used to extract the subtree T|G_i. In other words, it is the i-th
 * column of the missing data matrix
 * @return the subtree T|G_i.
 */
std::shared_ptr<Tree> generate_induced_tree(const std::shared_ptr<Tree> tree,
		const missingData *missing_data,
		std::map<std::string, unsigned char>& first, size_t partition);

/**
 * This function roots the tree at an appropriate position according to the missing data array
 *
 * @param tree the tree to be rooted
 * @param missing_data the data for the missing sequences on each partition
 * @return the new root of the tree, or NULL if the tree cannot be rooted (e.g. if there is no species that has data for every partition)
 */
std::shared_ptr<Tree> root_tree(ntree_t *tree, const missingData *missing_data, std::string &root_species_name);

/**
 * Returns a pointer to the leaf that has the label <label>
 *
 * @param tree a pointer to the root of the tree
 * @return a pointer to the leaf
 */
ntree_t* get_leaf_by_name(ntree_t *tree, const char *label);

/**
 * private function that roots the tree at a given leaf-edge
 * @param root the leaf-edge
 * @return pointer to the new root
 */
std::shared_ptr<Tree> root_at(ntree_t *root);

/**
 * private function
 * @param current instance of the Rtree class that needs to get its child pointers
 * @param current_ntree the ntree_t coresponding to the current parameter. Here we get the children from
 * @param parent the ntree_t represention of the parent. this is needed to avoid calling the recursion on the parent
 */
void recursive_root(std::shared_ptr<Tree> current, ntree_t *current_ntree,
		ntree_t *parent);

/**
 * @brief check_tree do a dfs on the tree and check, if the childs parent is the current node.
 * @param tree the tree to check
 * @return true if the tree is correct
 */
bool check_tree(ntree_t *tree);

/**
 * @brief fix_tree makes sure, that tree->child[i]->parent == tree
 * @param tree the tree to fix
 */
void fix_tree(ntree_t *tree);

/**
 * @brief get_neighbours returns the neighbours of a given node in the tree. this can be used for easier usage of rooted trees as unrooted trees
 * @param node the node of which we want to get the adjacent nodes
 * @return a vector containing the adjacent nodes.
 */
std::vector<std::shared_ptr<Tree>> get_neighbours(std::shared_ptr<Tree> node);


/**
 * @brief node_compare compares two instances of the Tree class by their label
 * TODO We can use a lambda function in std::sort in tree_to_string instead
 * @param node_1 this node is compared to the other paramater
 * @param node_2 this node is compared to the other paramater
 * @return true, iff the label of node_1 is smaller than the label of node_2
 */
bool node_compare(std::shared_ptr<Tree> node_1, std::shared_ptr<Tree> node_2);


/**
 * @brief get_leafs return a list the list of all leafs of the tree
 * @param leaf_list the list where the leafs will be inserted
 * @param current_node the node, where the dfs is at the moment
 * @param parent the node where we come from. to avoid endles recursion
 */
void get_leafs (std::vector<std::shared_ptr<Tree>> &leaf_list, std::shared_ptr<Tree> current_node, std::shared_ptr<Tree> parent);

#endif //include guard
