#ifndef UNION_FIND_LEAF_SET_TEST_H
#define UNION_FIND_LEAF_SET_TEST_H


#include "gtest/gtest.h"

#include "types.h"

#include <vector>
#include <iostream>


TEST(DISABLED_get_nth_partition_tuple, no_constraints) {
    UnionFindLeafSet leafset(10);
    std::vector<constraint> constraints;
    leafset.apply_constraints(constraints);

    std::shared_ptr<UnionFindLeafSet> part_left;
    std::shared_ptr<UnionFindLeafSet> part_right;
    std::tie(part_left, part_right) = leafset.get_nth_partition_tuple(1);

    ASSERT_EQ(part_left->contains(0), true);
    ASSERT_EQ(part_right->contains(0), false);

    for (size_t i = 1; i < 10; i++) {
        ASSERT_EQ(part_left->contains(i), false);
    }
    for (size_t i = 1; i < 10; i++) {
        ASSERT_EQ(part_right->contains(i), true);
    }
}

TEST(DISABLED_get_nth_partition_tuple, only_one_set) {
    UnionFindLeafSet leafset(10);
    std::vector<constraint> constraints;
    constraints.emplace_back(1,2,3,4);
    constraints.emplace_back(2,3,3,4);
    constraints.emplace_back(3,4,3,4);
    constraints.emplace_back(4,5,3,4);
    constraints.emplace_back(5,6,3,4);
    constraints.emplace_back(6,7,3,4);
    constraints.emplace_back(7,8,3,4);
    constraints.emplace_back(8,9,3,4);

    leafset.apply_constraints(constraints);

    std::shared_ptr<UnionFindLeafSet> part_left;
    std::shared_ptr<UnionFindLeafSet> part_right;
    std::tie(part_left, part_right) = leafset.get_nth_partition_tuple(1);

    for (size_t i = 0; i < 10; i++) {
        ASSERT_EQ(part_left->contains(i), true);
    }
    for (size_t i = 0; i < 10; i++) {
        ASSERT_EQ(part_right->contains(i), false);
    }
}

TEST(DISABLED_get_nth_partition_tuple, only_one_leaf) {
    UnionFindLeafSet leafset(1);
    std::vector<constraint> constraints;

    leafset.apply_constraints(constraints);

    std::shared_ptr<UnionFindLeafSet> part_left;
    std::shared_ptr<UnionFindLeafSet> part_right;
    std::tie(part_left, part_right) = leafset.get_nth_partition_tuple(1);

    ASSERT_EQ(part_left->contains(1), true);
    ASSERT_EQ(part_right->contains(1), false);
}

//TODO more testcases, especially with a loop over all the partition tuples


#endif // UNION_FIND_LEAF_SET_TEST_H
