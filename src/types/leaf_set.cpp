#include "types/leaf_set.h"
#include "debug.h"

//void UnionFindLeafSet::apply_constraints(
//        const std::vector<constraint> &constraints) {

//    //if the datastrucute is not copied, we must apply "allToSingletons" only to the elements in the current set
//    data_structure->allToSingletons();

//    for (constraint cons : constraints) {
//        data_structure->merge(data_structure->find(cons.smaller_left),
//                                                   data_structure->find(cons.smaller_right));
//    }

//    //create a list of all distinct sets
//    std::map<leaf_number, leaf_number> sets;
//    for (size_t i = 0; i < data_structure->size(); i++) {
//        leaf_number set_repr = data_structure->find(i);
//        sets[set_repr] = set_repr;
//    }
//    for (std::map<leaf_number, leaf_number>::iterator it=sets.begin(); it != sets.end(); ++it) {
//        //TODO
//        list_of_partitions.push_back(std::make_shared<UnionFindLeafSet>(it->first));
//    }

//}



//TODO speed can be improved, if the caller allocates the memory for part_one and part_two and hands the adresses to the callee as argument.
//so we dont have to allocate new memory each time this method is called.
//std::tuple<std::shared_ptr<UnionFindLeafSet>, std::shared_ptr<UnionFindLeafSet>> UnionFindLeafSet::get_nth_partition_tuple(size_t n) {
//    //idea: it may be possible to copy the datastructure only once, since the two sets are disjoint

//    //TODO: Only heare because code is commented out!
//    dout("N:" << n << "\n");

//    /*std::shared_ptr<UnionFind> uf_1 = std::make_shared<UnionFind>(data_structure->get_parent(), data_structure->get_rank());
//    std::shared_ptr<UnionFind> uf_2 = std::make_shared<UnionFind>(data_structure->get_parent(), data_structure->get_rank());



//    auto part_one = std::make_shared<std::set<UnionFind>>(uf_1);
//    auto part_two = std::make_shared<std::set<UnionFind>>(uf_2);


//    bool part_one_first_set = false;
//    bool part_two_first_set = false;

//    assert(n > 0 && n <= number_partition_tuples(set_list->size()));

//    for (size_t i = 0; i < set_list->size(); i++) {
//        if (is_bit_set(n, i)) { //put the set into part_one
//            if (!part_one_first_set) {
//                part_one->set_repr(set_list.at(i).get_repr());
//            } else {
//                part_one->get_data_structure()->merge((part_one)->get_repr(), set_list->at(i).get_repr()); //compiler error could be fixed by calling a local apply-constraints function
//            }
//        } else {    //put the set into part_two
//            if (!part_two_first_set) {
//                part_two->set_repr(set_list.at(i).get_repr());
//            } else {
//                part_two->get_data_structure()->merge(part_two->get_repr(), set_list->at(i).get_repr());
//            }
//        }
//    }

//    //set the representative to the correct element
//    part_one->set_repr(part_one->get_data_structure()->find(part_one->get_repr()));
//    part_two->set_repr(part_two->get_data_structure()->find(part_two->get_repr()));*/

//    auto p1 = std::make_shared<UnionFindLeafSet>(1);
//    auto p2 = std::make_shared<UnionFindLeafSet>(1);

//    return std::tuple<std::shared_ptr<UnionFindLeafSet>, std::shared_ptr<UnionFindLeafSet>>(p1, p2);
//}

