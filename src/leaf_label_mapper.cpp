#include "leaf_label_mapper.h"

//std::map<const char*, size_t, label_compare> LeafLabelMapper::label_to_leaf_id;
char **LeafLabelMapper::species_names = nullptr;

void LeafLabelMapper::init_leaf_label_mapper(const missingData *m) {
    static bool already_init = false;
    
    if(!already_init) {
        for (size_t i = 0; i < m->numberOfSpecies; i++) {
            label_to_leaf_id[m->speciesNames[i]] = i;
        }
        species_names = m->speciesNames;
    } else {
        // only init once ever
        assert(0);
    }
}

char* LeafLabelMapper::get_label_from_leaf_id(const size_t leaf) {
    assert(species_names != nullptr);
    assert(leaf < label_to_leaf_id.size());
    
    return species_names[leaf];
}

size_t LeafLabelMapper::get_leaf_id_from_label(const char* label) {
    assert(label_to_leaf_id.count(label) == 1);
    return label_to_leaf_id[label];
}

size_t LeafLabelMapper::size() {
    return label_to_leaf_id.size();
}
