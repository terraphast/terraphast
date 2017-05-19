#include <catch.hpp>

#include <iostream>

#include <terraces/bipartitions.hpp>

namespace terraces {
namespace tests {

TEST_CASE("bipartitions", "[bipartitions]") {
	std::vector<std::vector<index>> set = {{0}, {1}, {2}};
	std::vector<bipartition> res = {{{0, 2}, {1}}, {{0, 1}, {2}}, {{0}, {1, 2}}};
	CHECK(sets_to_bipartitions(set) == res);
}

} // namespace tests
} // namespace terraces
