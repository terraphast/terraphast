#pragma once

#ifndef IFUGAO_LEAF_SET_H
#define IFUGAO_LEAF_SET_H

//#include <boost/dynamic_bitset.hpp>
#include <tuple>
#include <vector>
#include <map>

#include "constraints.h"
#include "UnionFind.h"

class LeafSet {
public:
    LeafSet() = default;
    LeafSet(const LeafSet&) = default;
    virtual ~LeafSet() = default;
    virtual void apply_constraints(const std::vector<constraint> &constraints) = 0;
    virtual bool contains(size_t leaf) = 0;
    virtual std::tuple<std::shared_ptr<LeafSet>, std::shared_ptr<LeafSet> > get_nth_partition_tuple(size_t n) = 0;
    //virtual size_t size() const = 0;

    //virtual bool compression_necessary();
    //virtual LeafSet compress();
};

class StandardFindLeafSet : public LeafSet {
public:
    void apply_constraints(const std::vector<constraint> &constraints);
};

class UnionFindLeafSet : public LeafSet {
public:
    UnionFindLeafSet(const UnionFindLeafSet& obj) {
        data_structure = obj.data_structure;
        repr = obj.repr;
        set_list = obj.set_list;
    }
    UnionFindLeafSet(size_t num_elems) {
        data_structure = std::make_shared<UnionFind>(num_elems);
        set_list = std::make_shared<std::vector<std::shared_ptr<UnionFindLeafSet> > >();
    }

    void apply_constraints(const std::vector<constraint> &constraints);
    bool contains(size_t leaf);
    std::tuple<std::shared_ptr<LeafSet>, std::shared_ptr<LeafSet>> get_nth_partition_tuple(size_t n);
private:
    std::shared_ptr<UnionFind> data_structure;
    size_t repr; //this is the number of the representative of this set. its an index to the array of the union find data structure
    std::shared_ptr<std::vector<std::shared_ptr<UnionFindLeafSet> > > set_list;
    inline std::shared_ptr<UnionFind> get_data_structure() {
        return data_structure;
    }
    inline void set_repr(size_t p_repr) {
        this->repr = p_repr;
    }
    inline size_t get_repr() {
        return repr;
    }
};

inline std::shared_ptr<LeafSet> create_leafSet(size_t num_elems) {
    return std::make_shared<UnionFindLeafSet>(num_elems);
}

#endif // IFUGAO_LEAF_SET_H
