#include <catch.hpp>

#include <terraces/fast_set.hpp>

namespace terraces {
namespace test {

TEST_CASE("fast_set1", "[fast_set]") {
	fast_index_set set{10};
	for (auto&& el : set) {
		(void)el;
		FAIL("element in empty set!");
	}
	set.insert(4);
	set.finalize_edit();
	CHECK(*set.begin() == 4);
	CHECK((++set.begin()) == set.end());
	CHECK(set.contains(4));
	CHECK(!set.contains(5));
	set.remove(4);
	set.insert(5);
	set.finalize_edit();
	CHECK(!set.contains(4));
	CHECK(set.contains(5));
	CHECK(*set.begin() == 5);
	CHECK((++set.begin()) == set.end());
	set.insert(2);
	set.finalize_edit();
	index count = 0;
	for (auto&& el : set) {
		(void)el;
		++count;
	}
	CHECK(count == set.size());
}

} // namespace test
} // namespace terraces
