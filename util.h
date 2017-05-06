#ifndef UTIL_H
#define UTIL_H

//this file contains useful function that don't belong anywhere else

#include "newick-tools/newick-tools.h"
#include "input_parser.h"
#include "terraces.h"

/**
    * This function roots the tree at an appropriate position according to the missing data array
    *
    * @param the tree to be rooted
    * @param data_file the name of the file containing the missing-data data
    * @return true if rooting was succesfully, false otherwise (e.g. if there is no species that has data for every partition)
    */
bool root_tree(ntree_t *tree, const char* data_file);

#endif //include guard
