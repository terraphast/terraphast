#include "input_parser_test.h"
#include "algorithm_test.h"

#include <limits.h>
#include "terraces.h"
#include "gtest/gtest.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"

// Tests positive input.
/*TEST(TerracesTest, Simple) {
  EXPECT_FALSE(IsPrime(4));
  EXPECT_TRUE(IsPrime(5));
  EXPECT_FALSE(IsPrime(6));
  EXPECT_TRUE(IsPrime(23));
}*/

#pragma clang diagnostic pop

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
