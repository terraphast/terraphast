#pragma once

#include <boost/dynamic_bitset.hpp>

#include "types.h"
#include "debug.h"

template <typename T>
using list_of_leaf_sets = std::vector<std::shared_ptr<T>>;

//Black Magic
class BitLeafSet;
typedef BitLeafSet LeafSet;
typedef list_of_leaf_sets<LeafSet> partition_list;


template <class LSClass>
class AbstractLeafSet {
public:
    AbstractLeafSet() = default;
    AbstractLeafSet(const AbstractLeafSet&) = default;
    virtual ~AbstractLeafSet() = default;
    virtual bool contains(const size_t leaf) const = 0;
    virtual size_t size() const = 0;
    virtual leaf_number pop() = 0;
    virtual LSClass& operator|=(const LSClass &other) = 0;
};

class SimpleLeafSet : public AbstractLeafSet<SimpleLeafSet>, std::set<leaf_number> {
public:
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
    bool contains(leaf_number leaf) const {
        return this->count(leaf) > 0;
    }
    inline size_t size() const {
        return std::set<leaf_number>::size();
    }
    inline
    void insert(const leaf_number l) {
        std::set<leaf_number>::insert(l);
    }
    inline
    list_of_leaf_sets<SimpleLeafSet> create_partition_list() const {
        list_of_leaf_sets<SimpleLeafSet> sets;
        sets.reserve(this->size());

        for (leaf_number l : *this) {
            // create an empty set for each leave
            auto set = std::make_shared<SimpleLeafSet>();
            set->insert(l);
            sets.push_back(set);
        }

        return sets;
    }
    inline
    std::set<leaf_number> to_set() {
        return *this;
    }
    inline
    std::shared_ptr<SimpleLeafSet> create_empty() {
        return std::make_shared<SimpleLeafSet>();
    }
    inline
    leaf_number pop() {
        auto itr = this->begin();
        auto first_elem = *itr;
        this->erase(itr);
        return first_elem;
    }
    inline
    SimpleLeafSet & operator|=(const SimpleLeafSet  &other) {
        std::set<leaf_number>::insert(other.begin(), other.end());
        return *this;
    }
};

class BitLeafSet : public AbstractLeafSet<BitLeafSet>, boost::dynamic_bitset<> {
public:
    BitLeafSet() = default;
    BitLeafSet (size_type num_bits, unsigned long value) : boost::dynamic_bitset<>(num_bits, value) {
    }
    BitLeafSet (std::initializer_list<leaf_number> l)  {
        auto max_val = *std::max_element(l.begin(), l.end());
        this->resize(max_val + 1, 0);
        for(auto elem : l) {
            (*this)[elem] = 1;
        }
    }
    inline
    static BitLeafSet create(size_t size) {
        BitLeafSet leaves;
        leaves.resize(size, 1);
        return leaves;
    }
    inline
    bool contains(leaf_number leaf) const {
        return leaf < boost::dynamic_bitset<>::size() && (*this)[leaf];
    }
    inline size_t size() const {
        return this->count();
    }
    inline
    void insert(const leaf_number l) {
        (*this)[l] = 1;
    }
    inline
    list_of_leaf_sets<BitLeafSet> create_partition_list() const {
        list_of_leaf_sets<BitLeafSet> sets;
        sets.reserve(this->size());

        leaf_number pos = this->find_first();
        while (pos != npos) {
            // create an empty set for each leave
            auto set = std::make_shared<BitLeafSet>(boost::dynamic_bitset<>::size(), 0);
            set->insert(pos);
            sets.push_back(set);
            pos = this->find_next(pos);
        }

        return sets;
    }
    inline
    std::set<leaf_number> to_set() {
        std::set<leaf_number> set;
        leaf_number pos = this->find_first();
        while (pos != npos) {
            set.insert(pos);
            pos = this->find_next(pos);
        }
        return set;
    }
    inline
    std::shared_ptr<BitLeafSet> create_empty() {
        return std::make_shared<BitLeafSet>(boost::dynamic_bitset<>::size(), 0);
    }
    inline
    leaf_number pop() {
        leaf_number pos = this->find_first();
        (*this)[pos] = false;
        return pos;
    }
    inline
    BitLeafSet & operator|=(const BitLeafSet  &other) {
        boost::dynamic_bitset<>::operator|=(other);
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
