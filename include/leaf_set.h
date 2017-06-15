#include <boost/dynamic_bitset.hpp>
#include <tuple>
#include <vector>
#include <ifugao.h>
#include <UnionFind.h>

class LeafSet {
public:
    virtual ~LeafSet() = default;
    virtual std::vector<LeafSet> apply_constraints(const std::vector<constraint> &constraints);
    virtual bool contains(size_t leaf);
    virtual std::tuple<LeafSet, LeafSet> get_nth_partition_tuple(
            std::vector<LeafSet> &partitions, size_t n);
};

class BitLeafSet : LeafSet {
public:
    std::vector<LeafSet> apply_constraints(const std::vector<constraint> &constraints);
    bool contains(size_t leaf);
    std::tuple<LeafSet, LeafSet> get_nth_partition_tuple(
            std::vector<LeafSet> &partitions, size_t n);

private:
    boost::dynamic_bitset<> set;
};

class UnionFindLeafSet : LeafSet {
public:

    UnionFindLeafSet(std::shared_ptr<UnionFind> data) : data_structure(data) {}

    std::vector<LeafSet> apply_constraints(const std::vector<constraint> &constraints);
    bool contains(size_t leaf);
    std::tuple<LeafSet, LeafSet> get_nth_partition_tuple(
            std::vector<LeafSet> &partitions, size_t n);

private:
    std::shared_ptr<UnionFind> data_structure;
    size_t set; //this is the number of the representative of this set. its an index to the array of the union find data structure
};
