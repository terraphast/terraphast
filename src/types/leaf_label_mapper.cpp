#include "types/leaf_label_mapper.h"

std::map<const char*, size_t> LeafLabelMapper::label_to_leaf_id_internal;
char **LeafLabelMapper::species_names_internal = nullptr;

void LeafLabelMapper::init_leaf_label_mapper(
        std::map<const char*, size_t> label_to_leaf_id,
        char **species_names) {
    static bool already_init = false;
    
    if(!already_init) {
        species_names_internal = species_names;
        label_to_leaf_id_internal = label_to_leaf_id;
    } else {
        // only init once ever
        assert(false);
    }
}

char* LeafLabelMapper::get_label_from_leaf_id(const size_t leaf) {
    assert(species_names_internal != nullptr);
    
    return species_names_internal[leaf];
};

size_t LeafLabelMapper::get_leaf_id_from_label(const char* label) {
    assert(label_to_leaf_id_internal.count(label) == 1);
    
    return label_to_leaf_id_internal[label];
};

size_t LeafLabelMapper::size() {
    return label_to_leaf_id_internal.size();
}
