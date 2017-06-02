#ifndef UTIL_TEST_H
#define UTIL_TEST_H


#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"

#include "../util.h"

//#include <limits.h>
//#include <iostream>
#include <memory>

// Test a simple tree file
TEST(get_neighbours, simple) {
    std::shared_ptr<Tree> t1 = std::make_shared<Tree>("t1");
    std::shared_ptr<Tree> t2 = std::make_shared<Tree>("t2");
    std::shared_ptr<Tree> t3 = std::make_shared<Tree>("t3");
    std::shared_ptr<Tree> t4 = std::make_shared<Tree>("t4");

    t1->left = t2;
    std::vector<std::shared_ptr<Tree>> result = get_neighbours(t1);
    ASSERT_EQ(result.size(), 1);
    ASSERT_EQ(result[0]->label.compare("t2"), 0);

    t1->right = t3;
    result = get_neighbours(t1);
    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result[0]->label.compare("t2"), 0);
    ASSERT_EQ(result[1]->label.compare("t3"), 0);

    t1->parent = t4;
    result = get_neighbours(t1);
    ASSERT_EQ(result.size(), 3);
    ASSERT_EQ(result[0]->label.compare("t2"), 0);
    ASSERT_EQ(result[1]->label.compare("t3"), 0);
    ASSERT_EQ(result[2]->label.compare("t4"), 0);

}

#endif // UTIL_TEST_H
