#ifndef UTIL_TEST_H
#define UTIL_TEST_H

#include "util.h"

#include <memory>

// Test a simple tree file
TEST(get_neighbours, simple) {
    std::shared_ptr<Tree> t1 = std::make_shared<Tree>(1);
    std::shared_ptr<Tree> t2 = std::make_shared<Tree>(2);
    std::shared_ptr<Tree> t3 = std::make_shared<Tree>(3);
    std::shared_ptr<Tree> t4 = std::make_shared<Tree>(4);

    t1->left = t2;
    std::vector<std::shared_ptr<Tree>> result = get_neighbours(t1);
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0]->id, 2);

    t1->right = t3;
    result = get_neighbours(t1);
    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result[0]->id, 2);
    ASSERT_EQ(result[1]->id, 3);

    t1->parent = t4;
    result = get_neighbours(t1);
    ASSERT_EQ(result.size(), 3);
    ASSERT_EQ(result[0]->id, 2);
    ASSERT_EQ(result[1]->id, 3);
    ASSERT_EQ(result[2]->id, 4);

}

#endif // UTIL_TEST_H
