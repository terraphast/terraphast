#include "gtest/gtest.h"

#include "input_parser_test.h"
#include "algorithm_test.h"
#include "terraces_test.h"
#include "util_test.h"
#include "union_find_leaf_set_test.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

