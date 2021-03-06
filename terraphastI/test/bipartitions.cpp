#include <catch.hpp>

#include <iostream>

#include "../lib/bipartitions.hpp"
#include "../lib/ranked_bitvector.hpp"
#include "../lib/union_find.hpp"

namespace terraces {
namespace tests {

TEST_CASE("bipartition1", "[bipartition]") {
	auto fl = utils::free_list{};
	auto alloc = utils::stack_allocator<index>{fl, 4};
	union_find u(4, alloc);
	ranked_bitvector s{4, alloc};
	s.set(0);
	s.set(1);
	s.set(2);
	s.set(3);
	s.update_ranks();
	u.merge(0, 1);
	u.compress();
	bipartition_iterator bip_it(s, u, alloc);
	CHECK(bip_it.end_bip() == 4);
	CHECK(bip_it.num_bip() == 3);
	CHECK(bip_it.cur_bip() == 1);
	CHECK(!bip_it.get_current_set().get(0));
	CHECK(!bip_it.get_current_set().get(1));
	CHECK(bip_it.get_current_set().get(2));
	CHECK(!bip_it.get_current_set().get(3));
	bip_it.increase();
	CHECK(bip_it.cur_bip() == 2);
	CHECK(!bip_it.get_current_set().get(0));
	CHECK(!bip_it.get_current_set().get(1));
	CHECK(!bip_it.get_current_set().get(2));
	CHECK(bip_it.get_current_set().get(3));
	bip_it.increase();
	CHECK(bip_it.cur_bip() == 3);
	CHECK(!bip_it.get_current_set().get(0));
	CHECK(!bip_it.get_current_set().get(1));
	CHECK(bip_it.get_current_set().get(2));
	CHECK(bip_it.get_current_set().get(3));
	CHECK(bip_it.is_valid());
	bip_it.increase();
	CHECK(!bip_it.is_valid());
	CHECK(bip_it.end_bip() == 4);
	CHECK(bip_it.num_bip() == 3);
}

} // namespace tests
} // namespace terraces
