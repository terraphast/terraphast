#include "util.h"


bool root_tree(ntree_t *tree, const char* data_file) {
    /*missingData *missing_data = NULL;
    input_data *in_data = parse_input_data(data_file);
    initializeMissingData(in_data->number_of_species, in_data->number_of_partitions, (const char**) in_data->names);
    copyDataMatrix(in_data->matrix, missing_data);

    long root_species_number = -1;
    for (size_t i = 0; i < in_data->number_of_species; i++) {
        bool contains_all_data = true;
        for (size_t k = 0; k < in_data->number_of_partitions; k++) {
            if (getDataMatrix(missing_data, i, k) == '0') {
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
        //TODO root tree
        return true;
    } else {    //tree cannot be rooted consistently
        return false;
    }*/
}

ntree_t* get_leaf_by_name(ntree_t *tree, char *label) {
    /*if (strcmp(tree->label, label) == 0) {
        return tree;
    } else {
        for (int i = 0; i < tree->children_count; i++) {
            ntree_t *temp = get_leaf_by_name(tree->children[i], label);
            if (temp != NULL) {
                return temp;
            }
        }
    }
    return NULL;*/
}
