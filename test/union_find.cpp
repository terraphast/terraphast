#include <catch.hpp>

#include <iostream>

#include <terraces/union_find.hpp>

namespace terraces {
namespace tests {

TEST_CASE("union_find1", "[union_find]") {
	union_find leaves(3);
	leaves.merge(0, 1);
	CHECK(leaves.find(0) == leaves.find(1));
	CHECK(leaves.find(2) == 2);
}

TEST_CASE("union_find2", "[union_find]") {
	union_find leaves(5);
	leaves.merge(0, 1);
	leaves.merge(1, 4);
	CHECK(leaves.find(0) == leaves.find(1));
	CHECK(leaves.find(1) == leaves.find(4));
	CHECK(leaves.find(2) == 2);
	CHECK(leaves.find(3) == 3);
}

} // namespace tests
} // namespace terraces
