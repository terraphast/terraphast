#include "leaf_set.h"


//std::vector<LeafSet> UnionFindLeafSet::apply_constraints(const std::vector<constraint> &constraints) {

//}

bool UnionFindLeafSet::contains(size_t leaf) {
    return data_structure->find(leaf) == set;
}

//std::tuple<LeafSet, LeafSet> UnionFindLeafSet::get_nth_partition_tuple(std::vector<LeafSet> &partitions, size_t n) {

//}

