#pragma once

#include "types.h"

/**
 * Static variables and methods to map leaf_id <->label
 */
class LeafLabelMapper {
public:
    static void init_leaf_label_mapper(
            std::map<const char*, size_t> label_to_leaf_id,
            char **species_names);
    
    static char* get_label_from_leaf_id(const size_t leaf);

    static size_t get_leaf_id_from_label(const char* label);
    
    static size_t size();
    
private:
    static std::map<const char*, size_t> label_to_leaf_id_internal;
    static char **species_names_internal;
};
