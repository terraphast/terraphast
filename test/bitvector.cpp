
#include <catch.hpp>

#include <terraces/bitvector.hpp>

namespace terraces {
namespace tests {

TEST_CASE("popcount tests", "[bitvector]") {
	CHECK(efficient::popcount(
	              0b1010111010101101010001010100000101000011000010111100000101001000) == 26);
	CHECK(efficient::popcount(
	              0b0000000000000000000000000000000000000000000000000000000000000000) == 0);
	CHECK(efficient::popcount(
	              0b0000001000000100000000000010000000001000000000001000000010001100) == 8);
}

TEST_CASE("prefix mask test", "[bitvector]") {
	CHECK(efficient::prefix_mask(63) ==
	      0b0111111111111111111111111111111111111111111111111111111111111111);
	CHECK(efficient::prefix_mask(1) ==
	      0b0000000000000000000000000000000000000000000000000000000000000001);
	CHECK(efficient::prefix_mask(10) ==
	      0b0000000000000000000000000000000000000000000000000000001111111111);
}

TEST_CASE("partial popcount tests", "[bitvector]") {
	//              6666555555555544444444443333333333222222222211111111110000000000
	//              3210987654321098765432109876543210987654321098765432109876543210
	CHECK(efficient::partial_popcount(
	              0b1010111010101101010001010100000101000011000010111100000101001000, 63) ==
	      25);
	CHECK(efficient::partial_popcount(
	              0b1010111010101101010001010100000101000011000010111100000101001000, 62) ==
	      25);
	CHECK(efficient::partial_popcount(
	              0b1010111010101101010001010100000101000011000010111100000101001000, 60) ==
	      24);
	CHECK(efficient::partial_popcount(
	              0b1010111010101101010001010100000101000011000010111100000101001000, 49) ==
	      17);
}

TEST_CASE("naive bitvector", "[bitvector]") {
	// 0 1 2 3 4 5 6 7 8 9
	// 0 1 1 0 1 0 1 0 1 1
	naive::bitvector b(10);
	b.set(1);
	b.set(2);
	b.set(4);
	b.set(6);
	b.set(8);
	b.set(9);
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
}

TEST_CASE("efficient bitvector", "[bitvector]") {
	// 0 1 2 3 4 5 6 7 8 9
	// 0 1 1 0 1 0 1 0 1 1
	efficient::bitvector b(10);
	b.set(1);
	b.set(2);
	b.set(4);
	b.set(6);
	b.set(8);
	b.set(9);
	b.update_ranks();
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
}

TEST_CASE("efficient bitvector large", "[bitvector]") {
	efficient::bitvector b(255);
	b.set(1);
	b.set(63);
	b.set(128);
	b.set(191);
	b.set(200);
	b.set(204);
	b.update_ranks();
	CHECK(b.rank(10) == 1);
	CHECK(b.rank(63) == 1);
	CHECK(b.rank(64) == 2);
	CHECK(b.rank(128) == 2);
	CHECK(b.rank(129) == 3);
	CHECK(b.rank(191) == 3);
	CHECK(b.rank(192) == 4);
	CHECK(b.rank(200) == 4);
	CHECK(b.rank(201) == 5);
	CHECK(b.rank(204) == 5);
	CHECK(b.rank(205) == 6);
	CHECK(b.rank(255) == 6);
}

} // namespace tests
} // namespace terraces
