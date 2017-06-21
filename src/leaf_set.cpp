#include "leaf_set.h"



std::vector<LeafSet> UnionFindLeafSet::apply_constraints(
        const std::vector<constraint> &constraints) {

    std::vector<std::shared_ptr<LeafSet>> setList;

    //if the datastrucute is not copied, we must apply "allToSingletons" only to the elements in the current set
    data_structure->allToSingletons();

    for (constraint cons : constraints) {
        data_structure->merge(data_structure->find(cons.smaller_left,
                                                   data_structure->find(cons.smaller_right)));
    }

    //create a list of all distinct sets
    std::map<leaf_number, leaf_number> sets;
    for (size_t i = 0; i < data_structure->size(); i++) {
        leaf_number set_repr = data_structure->find(i);
        sets.insert(set_repr, set_repr);
    }
    for (std::map<leaf_number, leaf_number>::iterator it=sets.begin(); it != sets.end(); ++it) {
        setList.push_back(std::make_shared<LeafSet>(it->first));
    }

    return setList;
}

bool UnionFindLeafSet::contains(size_t leaf) {
    return data_structure->find(leaf) == set;
}

//TODO speed can be improved, if the caller allocates the memory for part_one and part_two and hands the adresses to the callee as argument.
//so we dont have to allocate new memory each time this method is called.

//TODO really copy the datastructure. At the moment I reuse the pointer to it, which is wrong.
std::tuple<LeafSet, LeafSet> UnionFindLeafSet::get_nth_partition_tuple(std::vector<LeafSet> &partitions, size_t n) {
    LeafSet part_one = std::make_shared<std::set<leaf_number>>(data_structure);
    LeafSet part_two = std::make_shared<std::set<leaf_number>>(data_structure);

    bool part_one_first_set = false;
    bool part_two_first_set = false;

    assert(n > 0 && n <= number_partition_tuples(partitions));

    for (size_t i = 0; i < partitions.size(); i++) {
        if (is_bit_set(n, i)) {
            if (!part_one_first_set) {
                part_one->set_repr(partitions.at(i).get_repr());
            } else {
                part_one->get_data_structure()->merge(part_one->get_repr(), partitions.at(i).get_repr()); //compiler error could be fixed by calling a local apply-constraints function
            }
        } else {
            if (!part_two_first_set) {
                part_two->set_repr(partitions.at(i).get_repr());
            } else {
                part_two->get_data_structure()->merge(part_two->get_repr(), partitions.at(i).get_repr());
            }

            part_two->insert(partitions[i]->begin(), partitions[i]->end());
        }
    }

    return std::make_tuple(part_one, part_two);
}


std::shared_ptr<UnionFind> UnionFindLeafSet::get_data_structure() {
    return data_structure;
}

void UnionFindLeafSet::set_repr(size_t repr) {
    set = repr;
}

size_t UnionFindLeafSet::get_repr() {
    return set;
}

