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

} // namespace test
} // namespace terraces
