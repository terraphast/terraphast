#pragma once

#ifndef IFUGAO_LEAF_SET_H
#define IFUGAO_LEAF_SET_H

#include <boost/dynamic_bitset.hpp>
#include <tuple>
#include <set>

#include "constraints.h"
#include "UnionFind.h"

template <class LSClass>
class AbstractLeafSet {
public:
    AbstractLeafSet() = default;
    AbstractLeafSet(const AbstractLeafSet&) = default;
    virtual ~AbstractLeafSet() = default;
    virtual bool contains(const size_t leaf) const = 0;
    virtual size_t size() const = 0;
    virtual leaf_number pop() = 0;
    virtual LSClass& operator&=(const LSClass &other) = 0;
};

class SimpleLeafSet : public AbstractLeafSet<SimpleLeafSet >, std::set<leaf_number> {
public:
    typedef std::set<leaf_number>::iterator iterator;
    typedef std::set<leaf_number>::iterator const_iterator;
    typedef leaf_number value_type;

    SimpleLeafSet () = default;
    SimpleLeafSet (std::initializer_list<leaf_number> l) : std::set<leaf_number>(l) {
    }

    inline
    static SimpleLeafSet  create(size_t size) {
        SimpleLeafSet  leaves;
        for (size_t k = 0; k < size; k++) {
            leaves.insert(leaf_number(k));
        }
        return leaves;
    }

    inline
    bool contains(size_t leaf) const {
        return this->count(leaf) > 0;
    }
    inline size_t size() const {
        return std::set<leaf_number>::size();
    }
    inline
    iterator begin() const {
        return std::set<leaf_number>::begin();
    }
    inline
    iterator end() const {
        return std::set<leaf_number>::end();
    }
    inline
    std::pair<iterator, bool> insert(const leaf_number l) {
        return std::set<leaf_number>::insert(l);
    }
    inline leaf_number pop() {
        auto itr = this->begin();
        auto first_elem = *itr;
        this->erase(itr);
        return first_elem;
    }
    inline
    SimpleLeafSet & operator&=(const SimpleLeafSet  &other) {
        std::set<leaf_number>::insert(other.begin(), other.end());
        return *this;
    }
};

class UnionFindLeafSet  {
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

    bool contains(size_t leaf);

    void apply_constraints(const std::vector<constraint> &constraints);
    std::tuple<std::shared_ptr<UnionFindLeafSet>, std::shared_ptr<UnionFindLeafSet>> get_nth_partition_tuple(size_t n);
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

//Black Magick
typedef SimpleLeafSet LeafSet;

#endif // IFUGAO_LEAF_SET_H
