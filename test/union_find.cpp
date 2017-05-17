#include <catch.hpp>

#include <iostream>

#include <terraces/union_find.hpp>

namespace terraces {
namespace tests {

TEST_CASE("union_find", "[union_find]") {
        union_find leaves = make_set(3);
        merge(leaves, 0, 1);
	CHECK(find(leaves, 0) == find(leaves, 1));
	CHECK(find(leaves, 2) == 2);
}

} // namespace tests
} // namespace terraces
