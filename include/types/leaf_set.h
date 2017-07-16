#pragma once

#include <boost/dynamic_bitset.hpp>

#include "types.h"
#include "debug.h"

//Black Magic
class BitLeafSet;
typedef BitLeafSet LeafSet;

//class UnionFindLeafSet;
//typedef UnionFindLeafSet LeafSet;

/**
 * Checks whenever the n-th bit is set in the given number
 * @param num the number to check
 * @param n the n-th bit to check whenever it is set or not
 * @return true of the n-th bit is set
 */
inline bool is_bit_set(size_t num, size_t n) {
    return 1 == ((num >> n) & 1);
}

template <class LSClass>
class AbstractLeafSet {
public:
    AbstractLeafSet() = default;
    AbstractLeafSet(const AbstractLeafSet&) = default;
    virtual ~AbstractLeafSet() = default;
    virtual bool contains(const size_t leaf) const = 0;
    virtual size_t size() const = 0;
    virtual leaf_number pop() = 0;
    virtual void merge(const LSClass &other) = 0;
};

//TODO SimpleLeafSet aufs neue Interface anpassen
class SimpleLeafSet : public AbstractLeafSet<SimpleLeafSet>, std::set<leaf_number> {
    typedef std::vector<std::shared_ptr<SimpleLeafSet>> partition_list;
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
    partition_list create_partition_list() const {
        partition_list sets;
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
    void merge(const SimpleLeafSet  &other) {
        std::set<leaf_number>::insert(other.begin(), other.end());
    }
};

class BitLeafSet : public AbstractLeafSet<BitLeafSet>, boost::dynamic_bitset<> {
    typedef std::vector<std::shared_ptr<BitLeafSet>> partition_list;
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
    BitLeafSet (size_t size) {
        this->resize(size, 1);
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
    std::set<leaf_number> to_set() const {
        std::set<leaf_number> set;
        leaf_number pos = this->find_first();
        while (pos != npos) {
            set.insert(pos);
            pos = this->find_next(pos);
        }
        return set;
    }

    inline
    leaf_number pop() {
        leaf_number pos = this->find_first();
        (*this)[pos] = false;
        return pos;
    }

    std::tuple<std::shared_ptr<BitLeafSet>,
            std::shared_ptr<BitLeafSet> > get_nth_partition_tuple(const size_t n) {

        assert(list_of_partitions.size() > 0);

        auto part_one = this->create_empty();
        auto part_two = this->create_empty();

        assert(n > 0 && n <= number_partition_tuples());

        for (size_t i = 0; i < list_of_partitions.size(); i++) {
            if (is_bit_set(n, i)) {
                part_one->merge(*list_of_partitions[i]);
            } else {
                part_two->merge(*list_of_partitions[i]);
            }
        }

        return std::make_tuple(part_one, part_two);
    }

    //TODO this method should be in the super class
    /**
     * Returns the number of partition tuples that can be formed by combining the
     * given list of partitions. The formular is 2^(n-1) - 1, where n is the size of
     * the list.
     * @return the number of partition tuples that can be formed from the given list
     */
    inline size_t number_partition_tuples() {
        assert(list_of_partitions.size() > 1);
        return (1 << (list_of_partitions.size() - 1)) - 1;
    }

    void apply_constraints(const std::vector<constraint> &constraints) {
        list_of_partitions = create_partition_list();

        bool found_left_constraint = false;
        bool found_right_constraint = false;
        size_t index_containing_left_constraint = 0;
        size_t index_containing_right_constraint = 0;

        for (constraint cons : constraints) {
             for (size_t i = 0; i < list_of_partitions.size(); i++) {
                if (list_of_partitions[i]->contains(cons.smaller_left)) {
                    // set contains the left constraint
                    found_left_constraint = true;
                    index_containing_left_constraint = i;
                }
                if (list_of_partitions[i]->contains(cons.smaller_right)) {
                    // set contains the right constraint
                    found_right_constraint = true;
                    index_containing_right_constraint = i;
                }
            }
            assert(found_left_constraint
                    && index_containing_left_constraint < list_of_partitions.size());
            assert(
                    found_right_constraint
                    && index_containing_right_constraint < list_of_partitions.size());
            if (index_containing_left_constraint != index_containing_right_constraint) {
                // sets need to be merged
                list_of_partitions[index_containing_left_constraint]->merge(*list_of_partitions[index_containing_right_constraint]);
                list_of_partitions.erase(list_of_partitions.begin() + static_cast<partition_list::difference_type>(
                                                  index_containing_right_constraint));
            }
        }
    }

    const partition_list& get_list_of_partitions() const {
        return list_of_partitions;
    }

private:
    partition_list list_of_partitions;

    inline
    partition_list create_partition_list() const {
        partition_list sets;
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
    std::shared_ptr<BitLeafSet> create_empty() {
        return std::make_shared<BitLeafSet>(boost::dynamic_bitset<>::size(), 0);
    }

    inline
    void merge(const BitLeafSet  &other) {
        boost::dynamic_bitset<>::operator|=(other);
    }
};


class UnionFindLeafSet  {
    typedef std::vector<std::shared_ptr<UnionFindLeafSet>> partition_list;
public:
    UnionFindLeafSet(const UnionFindLeafSet& obj) {
        data_structure = obj.data_structure;
        repr = obj.repr;
        repr_valid = obj.repr_valid;
        //list_of_partitions = obj.list_of_partitions; I dont think we need to copy this
    }

    UnionFindLeafSet(std::shared_ptr<UnionFind> data_structure, size_t repr)
        : data_structure(data_structure), repr(repr), repr_valid(true) {

    }

    UnionFindLeafSet(std::shared_ptr<UnionFind> data_structure) : data_structure(data_structure) {}

    UnionFindLeafSet(size_t num_elems) {
        data_structure = std::make_shared<UnionFind>(num_elems);
        //list_of_partitions = std::vector<std::shared_ptr<UnionFindLeafSet> >();
    }

    UnionFindLeafSet (std::initializer_list<leaf_number> l)  {
        auto max_val = *std::max_element(l.begin(), l.end());
        data_structure = std::make_shared<UnionFind>(max_val + 1);

        repr = *(l.begin());
        for(size_t i = 1; i < l.size(); i++) {
            data_structure->get_parent().at(i) = repr;
        }
        repr_valid = true;
    }

    bool contains(size_t leaf) const {
        return data_structure->find(leaf) == repr;
    }

    inline size_t size() const {
        if (!repr_valid) {  //in this case we dont really represent a set
            return 0;
        } else {
            size_t count = 0;
            for (size_t i = 0; i < data_structure->size(); i++) {
                if (data_structure->find(i) == repr) {
                    count++;
                }
            }
            return count;
        }
    }

    inline void insert(const leaf_number l) {
        assert(repr_valid);
        data_structure->get_parent().at(l) = repr;
    }

    inline std::set<leaf_number> to_set() const {
        std::set<leaf_number> set;
        if (!repr_valid) {
            return set;
        }
        for (size_t i = 0; i < data_structure->size(); i++) {
            if (data_structure->find(i) == repr) {
                set.insert(i);
            }
        }
        return set;
    }

    inline leaf_number pop() {
        assert(repr_valid);

        //TODO this is inefficient. Pop can be efficiently implemented with backpointers
        if (this->size() == 1) {
            repr_valid = false;
            return repr;
        }

        for (size_t i = 0; i < data_structure->size(); i++) {
            if (i == repr) {
                continue;
            }
            else if (data_structure->find(i) == repr) {
                data_structure->get_parent().at(i) = i;
                return i;
            }
        }
        assert(0);  //this means that we have a valid set without any element
        return 0;
    }

    //TODO this method should be in the super class
    /**
     * Returns the number of partition tuples that can be formed by combining the
     * given list of partitions. The formular is 2^(n-1) - 1, where n is the size of
     * the list.
     * @return the number of partition tuples that can be formed from the given list
     */
    inline size_t number_partition_tuples() const {
        assert(list_of_partitions.size() > 1);
        return (1 << (list_of_partitions.size() - 1)) - 1;
    }

    void apply_constraints(const std::vector<constraint> &constraints) {

        //if the datastrucute is not copied, we must apply "allToSingletons" only to the elements in the current set
        data_structure->allToSingletons();
        repr_valid = false;

        for (constraint cons : constraints) {
            assert(cons.smaller_left < data_structure->size() && cons.smaller_right < data_structure->size());
            data_structure->merge(cons.smaller_left, cons.smaller_right);
        }

        //TODO how slow are std::sets? we could use a bool array as well
        //create a list of all distinct sets
        std::set<leaf_number> sets;
        for (size_t i = 0; i < data_structure->size(); i++) {
            leaf_number set_repr = data_structure->find(i);
            sets.insert(set_repr);
        }
        for (std::set<leaf_number>::iterator it=sets.begin(); it != sets.end(); ++it) {
            list_of_partitions.push_back(*it);
        }

    }

    std::tuple<std::shared_ptr<UnionFindLeafSet>, std::shared_ptr<UnionFindLeafSet>>
    get_nth_partition_tuple(size_t n) const {
        //idea: it may be possible to copy the datastructure only once, since the two sets are disjoint
        assert(n > 0 && n <= number_partition_tuples());
        assert(list_of_partitions.size() > 1);  //we need at least 2 sets, otherwise creating partitions does not make sense

        std::shared_ptr<UnionFind> uf_1 = std::make_shared<UnionFind>(data_structure->get_parent(), data_structure->get_rank());
        std::shared_ptr<UnionFind> uf_2 = std::make_shared<UnionFind>(data_structure->get_parent(), data_structure->get_rank());

        std::shared_ptr<UnionFindLeafSet> part_one = std::make_shared<UnionFindLeafSet>(uf_1);
        std::shared_ptr<UnionFindLeafSet> part_two = std::make_shared<UnionFindLeafSet>(uf_2);

        bool part_one_first_set = false;
        bool part_two_first_set = false;

        for (size_t i = 0; i < list_of_partitions.size(); i++) {
          if (is_bit_set(n, i)) { //put the set into part_one
              if (!part_one_first_set) {
                  part_one->repr = list_of_partitions.at(i);
                  part_one->repr_valid = true;
                  part_one_first_set = true;
              } else {
                  part_one->repr = part_one->data_structure->merge(part_one->repr, list_of_partitions.at(i));
              }
          } else {    //put the set into part_two
              if (!part_two_first_set) {
                  part_two->repr = list_of_partitions.at(i);
                  part_two->repr_valid = true;
                  part_two_first_set = true;
              } else {
                  part_two->repr = part_two->data_structure->merge(part_two->repr, list_of_partitions.at(i));
              }
          }
        }

        return std::tuple<std::shared_ptr<UnionFindLeafSet>, std::shared_ptr<UnionFindLeafSet>>(part_one, part_two);
    }

    partition_list get_list_of_partitions() const {
        partition_list ret;

        for (size_t elem : list_of_partitions) {
            std::shared_ptr<UnionFindLeafSet> temp = std::make_shared<UnionFindLeafSet>(this->data_structure, elem);
            ret.push_back(temp);
        }

        return ret;
    }


private:
    std::shared_ptr<UnionFind> data_structure;
    size_t repr; //this is the number of the representative of this set. its an index to the array of the union find data structure
    bool repr_valid = false; //true iff the representative is valid
    std::vector<leaf_number> list_of_partitions;  //contains the id's of the representatives of the sets

//    void merge(const UnionFindLeafSet &other) {
//        assert(repr_valid && other.repr_valid);
//        repr = data_structure->merge(repr, other.repr);
//    }
};
