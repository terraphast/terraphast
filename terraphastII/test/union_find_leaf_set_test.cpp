#include "gtest/gtest.h"

#include "types.h"

#include <vector>
#include <iostream>

TEST(DISABLED_get_nth_split_tuple, no_constraints) {
    UnionFindLeafSet leafset(10);
    std::vector<constraint> constraints;
    leafset.apply_constraints(constraints);

    std::shared_ptr<UnionFindLeafSet> part_left;
    std::shared_ptr<UnionFindLeafSet> part_right;
    std::tie(part_left, part_right) = leafset.get_nth_split_tuple(1);

    ASSERT_EQ(part_left->contains(0), true);
    ASSERT_EQ(part_right->contains(0), false);

    for (size_t i = 1; i < 10; i++) {
        ASSERT_EQ(part_left->contains(i), false);
    }
    for (size_t i = 1; i < 10; i++) {
        ASSERT_EQ(part_right->contains(i), true);
    }
}

TEST(DISABLED_get_nth_split_tuple, only_one_set) {
    UnionFindLeafSet leafset(10);
    std::vector<constraint> constraints =
            { constraint(1,2,4), constraint(2,3,4), constraint(3,4,3),
              constraint(4,5,4), constraint(5,6,4), constraint(6,7,4),
              constraint(7,8,3), constraint(8,9,3) };

    leafset.apply_constraints(constraints);

    std::shared_ptr<UnionFindLeafSet> part_left;
    std::shared_ptr<UnionFindLeafSet> part_right;
    std::tie(part_left, part_right) = leafset.get_nth_split_tuple(1);

    for (size_t i = 0; i < 10; i++) {
        ASSERT_EQ(part_left->contains(i), true);
    }
    for (size_t i = 0; i < 10; i++) {
        ASSERT_EQ(part_right->contains(i), false);
    }
}

TEST(DISABLED_get_nth_split_tuple, only_one_leaf) {
    UnionFindLeafSet leafset(1);
    std::vector<constraint> constraints;

    leafset.apply_constraints(constraints);

    std::shared_ptr<UnionFindLeafSet> part_left;
    std::shared_ptr<UnionFindLeafSet> part_right;
    std::tie(part_left, part_right) = leafset.get_nth_split_tuple(1);

    ASSERT_EQ(part_left->contains(1), true);
    ASSERT_EQ(part_right->contains(1), false);
}

//TODO more testcases, especially with a loop over all the split tuples
