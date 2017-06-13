#include <catch.hpp>

#include <iostream>

#include <terraces/bipartitions.hpp>
#include <terraces/fast_set.hpp>
#include <terraces/union_find.hpp>

namespace terraces {
namespace tests {

TEST_CASE("bipartition1", "[bipartition]") {
	union_find u = make_set(4);
	fast_index_set s{4};
	s.insert(0);
	s.insert(1);
	s.insert(2);
	s.insert(3);
	s.finalize_edit();
	merge(u, 0, 1);
	bipartition_iterator bip_it(s, u);
	CHECK(!bip_it.get_current_set().contains(0));
	CHECK(!bip_it.get_current_set().contains(1));
	CHECK(bip_it.get_current_set().contains(2));
	CHECK(!bip_it.get_current_set().contains(3));
	bip_it.increase();
	CHECK(!bip_it.get_current_set().contains(0));
	CHECK(!bip_it.get_current_set().contains(1));
	CHECK(!bip_it.get_current_set().contains(2));
	CHECK(bip_it.get_current_set().contains(3));
	bip_it.increase();
	CHECK(!bip_it.get_current_set().contains(0));
	CHECK(!bip_it.get_current_set().contains(1));
	CHECK(bip_it.get_current_set().contains(2));
	CHECK(bip_it.get_current_set().contains(3));
	CHECK(bip_it.is_valid());
	bip_it.increase();
	CHECK(!bip_it.is_valid());
}

} // namespace tests
} // namespace terraces
