#include <catch.hpp>

#include <iostream>

#include <terraces/bipartitions.hpp>

namespace terraces {
namespace tests {

TEST_CASE("bipartition1", "[bipartition]") {
	std::vector<std::vector<index>> set = {{0}, {1}, {2}};
	std::vector<bipartition> res = {bipartition{{0, 2}, {1}}, bipartition{{0, 1}, {2}},
	                                bipartition{{0}, {2, 1}}};
	bipartition_iterator bip_it(set);
	CHECK(bip_it.get_bipartition() == res.at(0));
	bip_it.increase();
	CHECK(bip_it.get_bipartition() == res.at(1));
	bip_it.increase();
	CHECK(bip_it.get_bipartition() == res.at(2));
	CHECK(bip_it.is_valid());
	bip_it.increase();
	CHECK(!bip_it.is_valid());
}

} // namespace tests
} // namespace terraces
