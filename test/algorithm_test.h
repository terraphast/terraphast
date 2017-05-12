
#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"

#include "../ifugao.cpp"

#include <limits.h>
#include <iostream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"

TEST(ApplyConstraintsTest, simple_tree) {

  std::set<leaf_number> leaves = {1, 2, 3, 4, 5};

  std::vector<constraint> constraints;

  constraint cons1 = {1, 3, 2, 2};
  constraint cons2 = {4, 4, 5, 2};

  constraints.push_back(cons1);
  constraints.push_back(cons2);

  std::vector<std::shared_ptr<std::set<leaf_number> > > result = apply_constraints(leaves, constraints);
 
  ASSERT_EQ(result.size(), 3);

  int sum = 0;
  for(int i = 0; i < result.size(); i++){
	for(leaf_number n: *result[i]) {
	   sum += n;
	}
  }
  // all elements still contained in the sets?
  ASSERT_EQ(sum, 15);
}

#pragma clang diagnostic pop
