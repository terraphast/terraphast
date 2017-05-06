#include "util.h"


bool root_tree(ntree_t *tree, const char* data_file) {
    missingData *missing_data;
    input_data *in_data = parse_input_data(data_file);
    initializeMissingData(in_data->number_of_species, in_data->number_of_partitions, in_data->names);
    copyDataMatrix(in_data->matrix, missing_data);

    int root_species_number = -1;
    for (int i = 0; i < in_data->number_of_species; i++) {
        bool contains_all_data = true;
        for (int k = 0; k < in_data->number_of_partitions; k++) {
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
    }
}
