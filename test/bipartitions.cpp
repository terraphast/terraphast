#include <catch.hpp>

#include <iostream>

#include <terraces/bipartitions.hpp>

namespace terraces {
namespace tests {

TEST_CASE("bipartition1", "[bipartition]") {
	bipartition_iterator bip_it(3);
	CHECK(!bip_it.get(0));
	CHECK(!bip_it.get(2));
	CHECK(bip_it.get(1));
	bip_it.increase();
	CHECK(!bip_it.get(0));
	CHECK(!bip_it.get(1));
	CHECK(bip_it.get(2));
	bip_it.increase();
	CHECK(!bip_it.get(0));
	CHECK(bip_it.get(1));
	CHECK(bip_it.get(2));
	CHECK(bip_it.is_valid());
	bip_it.increase();
	CHECK(!bip_it.is_valid());
}

} // namespace tests
} // namespace terraces
