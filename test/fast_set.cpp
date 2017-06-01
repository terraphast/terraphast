#include <catch.hpp>

#include <terraces/fast_set.hpp>

namespace terraces {
namespace test {

TEST_CASE("fast_set1", "[fast_set]") {
	fast_index_set set{10};
	for (auto&& el : set) {
		FAIL("element in empty set!");
	}
	set.insert_element(4);
	set.finalize_edit();
	CHECK(*set.begin() == 4);
	CHECK((++set.begin()) == set.end());
	CHECK(set.contains(4));
	CHECK(!set.contains(5));
	set.delete_element(4);
	set.insert_element(5);
	set.finalize_edit();
	CHECK(!set.contains(4));
	CHECK(set.contains(5));
	CHECK(*set.begin() == 5);
	CHECK((++set.begin()) == set.end());

	set.fill(false);
	CHECK(set.size() == 0);
	for (index i = 0; i < 10; ++i) {
		CHECK(!set.contains(i));
	}
	set.fill(true);
	CHECK(set.size() == 10);
	for (index i = 0; i < 10; ++i) {
		CHECK(set.contains(i));
	}

	set.reset_and_resize(5);
	CHECK(set.size() == 0);
	set.fill(true);
	CHECK(set.size() == 5);
	CHECK(set.contains(4));
}

TEST_CASE("bitvector1", "[bitvector]") {
	// 0 1 2 3 4 5 6 7 8 9
	// 0 1 1 0 1 0 1 0 1 1
	bitvector b(10);
	b.set(1, true);
	b.set(2, true);
	b.set(4, true);
	b.set(6, true);
	b.set(8, true);
	b.set(9, true);
	CHECK(!b.get(0));
	CHECK(!b.get(3));
	CHECK(!b.get(5));
	CHECK(!b.get(7));
	CHECK(b.get(1));
	CHECK(b.get(2));
	CHECK(b.get(4));
	CHECK(b.get(6));
	CHECK(b.get(8));
	CHECK(b.get(9));
	CHECK(b.rank(4) == 2);
	CHECK(b.rank(7) == 4);
	CHECK(b.rank(10) == 6);
	CHECK(b.select(1) == 1);
	CHECK(b.select(2) == 2);
	CHECK(b.select(3) == 4);
	CHECK(b.select(4) == 6);
	CHECK(b.select(5) == 8);
	CHECK(b.select(6) == 9);
	CHECK(b.next_set_bit(0) == 1);
	CHECK(b.next_set_bit(1) == 2);
	CHECK(b.next_set_bit(2) == 4);
	CHECK(b.next_set_bit(8) == 9);
}

} // namespace test
} // namespace terraces
