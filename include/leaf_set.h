#pragma once

//#include <boost/dynamic_bitset.hpp>
#include <tuple>
#include <vector>
#include <ifugao.h>
#include <UnionFind.h>
#include <map>
#include "types.h"

struct constraint;
class UnionFindLeafSet;

class LeafSet {
public:
    virtual ~LeafSet() = default;
    virtual void apply_constraints(const std::vector<constraint> &constraints);
    virtual bool contains(size_t leaf);
    virtual std::tuple<LeafSet, LeafSet> get_nth_partition_tuple(size_t n);
    virtual size_t number_partition_tuples() const;
    virtual size_t size() const;

    virtual bool compression_necessary();
    virtual LeafSet compress();
};

//class BitLeafSet : LeafSet {
//public:
//    //TODO in bitvector branch den rückgabetyp anpassen
//    void apply_constraints(const std::vector<constraint> &constraints);
//    bool contains(size_t leaf);
//    std::tuple<LeafSet, LeafSet> get_nth_partition_tuple(size_t n);

//private:
//    boost::dynamic_bitset<> set;
//};

class UnionFindLeafSet : public LeafSet {
public:
    UnionFindLeafSet(size_t num_elems) {
        data_structure = std::make_shared<UnionFind>(num_elems);
    }

    UnionFindLeafSet(std::shared_ptr<UnionFind> data) : data_structure(data) {}
    UnionFindLeafSet(std::shared_ptr<UnionFind> data, size_t repr) : data_structure(data), repr(repr) {}

    void apply_constraints(const std::vector<constraint> &constraints);
    bool contains(size_t leaf);
    std::tuple<LeafSet, LeafSet> get_nth_partition_tuple(size_t n);
    std::shared_ptr<UnionFind> get_data_structure();
    void set_repr(size_t repr);
    size_t get_repr();

private:
    std::shared_ptr<UnionFind> data_structure;
    size_t repr; //this is the number of the representative of this set. its an index to the array of the union find data structure
    std::shared_ptr<std::vector<UnionFindLeafSet>> set_list;

};

inline LeafSet create_leafSet(size_t num_elems) {
    return UnionFindLeafSet(num_elems);
}
