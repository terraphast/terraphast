#include <catch.hpp>

#include <iostream>

#include <terraces/bipartitions.hpp>

namespace terraces {
namespace tests {

TEST_CASE("bipartitions1", "[bipartitions]") {
	std::vector<std::vector<index>> set = {{0}, {1}};
	std::vector<bipartition> res = {{{0}, {1}}};
	CHECK(sets_to_bipartitions(set) == res);
}

TEST_CASE("bipartitions2", "[bipartitions]") {
	std::vector<std::vector<index>> set = {{3}, {2}};
	std::vector<bipartition> res = {{{3}, {2}}};
	CHECK(sets_to_bipartitions(set) == res);
}

TEST_CASE("bipartitions3", "[bipartitions]") {
	std::vector<std::vector<index>> set = {{0}, {1}, {2}};
	std::vector<bipartition> res = {{{0, 1}, {2}}, {{0, 2}, {1}}, {{0}, {2, 1}}};
	CHECK(sets_to_bipartitions(set) == res);
}

} // namespace tests
} // namespace terraces
