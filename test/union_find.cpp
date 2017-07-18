#include <catch.hpp>

#include <iostream>

#include "../lib/union_find.hpp"

namespace terraces {
namespace tests {

TEST_CASE("union_find1", "[union_find]") {
	auto fl = utils::free_list{};
	auto alloc = utils::stack_allocator<index>{fl, 3};
	union_find leaves(3, alloc);
	leaves.merge(0, 1);
	CHECK(leaves.find(0) == leaves.find(1));
	CHECK(leaves.find(2) == 2);
}

TEST_CASE("union_find2", "[union_find]") {
	auto fl = utils::free_list{};
	auto alloc = utils::stack_allocator<index>{fl, 5};
	union_find leaves(5, alloc);
	leaves.merge(0, 1);
	leaves.merge(1, 4);
	CHECK(leaves.find(0) == leaves.find(1));
	CHECK(leaves.find(1) == leaves.find(4));
	CHECK(leaves.find(2) == 2);
	CHECK(leaves.find(3) == 3);
}

} // namespace tests
} // namespace terraces
