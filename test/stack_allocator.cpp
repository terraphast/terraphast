
#include <catch.hpp>


#include "../lib/stack_allocator.hpp"

namespace terraces {
namespace tests {

TEST_CASE("free_list_allocator basic", "[utils][utils::free_list_allocator]") {
	utils::free_list fl{sizeof(int) * 10};
	utils::free_list_allocator<int> alloc{fl, 10};
	std::vector<int, utils::free_list_allocator<int>> vec(alloc);
	CHECK_NOTHROW(vec.reserve(10));

}


} // namespace tests
} // namespace terraces
