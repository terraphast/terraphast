#pragma once

#include "terraces.h" // for missingData

/**
 * Static variables and methods to map leaf_id <->label
 */
class LeafLabelMapper {
public:
    static void init_leaf_label_mapper(const missingData *m);
    
    static char* get_label_from_leaf_id(const size_t leaf);

    static size_t get_leaf_id_from_label(const char* label);
    
    static size_t size();
    
private:
    /**
     * Mapping of leaf labels to their IDs
     */
    static std::map<const char*, size_t> label_to_leaf_id;
    
    /**
     * Mapping of leaf IDs to their labels
     */
    static char **species_names;
};
