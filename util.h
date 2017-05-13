#ifndef UTIL_H
#define UTIL_H

//this file contains useful function that don't belong anywhere else

#include "newick-tools/newick-tools.h"
#include "input_parser.h"
#include "terraces.h"

#include <string.h>
#include <map>
#include <memory>
#include <string>

/**
 * This funktion generates the trees T|G_i, which is exactly the subtree of T induced by partition G_i
 * @param tree the rooted binary supertree
 * @param missing_data the data matrix representing the missing partition data.
 * @param partition the index i used to extract the subtree T|G_i. In other words, it is the i-th
 * column of the missing data matrix
 * @return the subtree T|G_i.
 */
rtree_t* generate_induced_tree(rtree_t *tree, const missingData *missing_data,
		std::map<char*, unsigned char, cmp_str>& first, size_t partition);

/**
 * This function roots the tree at an appropriate position according to the missing data array
 *
 * @param tree the tree to be rooted
 * @param missing_data the data for the missing sequences on each partition
 * @return the new root of the tree, or NULL if the tree cannot be rooted (e.g. if there is no species that has data for every partition)
 */
std::shared_ptr<Rtree> root_tree(ntree_t *tree,
		const missingData *missing_data);

/**
 * Returns a pointer to the leaf that has the label <label>
 *
 * @param tree a pointer to the root of the tree
 * @return a pointer to the leaf
 */
ntree_t* get_leaf_by_name(ntree_t *tree, char *label);

/**
 * private function that roots the tree at a given leaf-edge
 * @param root the leaf-edge
 * @return pointer to the new root
 */
std::shared_ptr<Rtree> root_at(ntree_t *root);

/**
 * private function
 * @param current instance of the Rtree class that needs to get its child pointers
 * @param current_ntree the ntree_t coresponding to the current parameter. Here we get the children from
 * @param parent the ntree_t represention of the parent. this is needed to avoid calling the recursion on the parent
 */
void recursive_root(std::shared_ptr<Rtree> current, ntree_t *current_ntree,
		ntree_t *parent);

#endif //include guard
