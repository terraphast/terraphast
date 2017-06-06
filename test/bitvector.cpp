
#include <catch.hpp>

#include <terraces/bitvector.hpp>

namespace terraces {
namespace tests {

TEST_CASE("byte select tests", "[bitvector]") {
	//                             76543210
	CHECK(efficient::byte_select(0b00001000, 0) == 3);
	CHECK(efficient::byte_select(0b00100100, 0) == 2);
	CHECK(efficient::byte_select(0b00100100, 1) == 5);
	CHECK(efficient::byte_select(0b00000000, 0) == 7);
	CHECK(efficient::byte_select(0b11111101, 5) == 6);
}

TEST_CASE("block select tests", "[bitvector]") {
	//              6666555555555544444444443333333333222222222211111111110000000000
	//              3210987654321098765432109876543210987654321098765432109876543210
	CHECK(efficient::block_select(
	              0b1010111010101101010001010100000101000011000010111100000101001000, 25) ==
	      63);
	CHECK(efficient::block_select(
	              0b1010111010101101010001010100000101000011000010111100000101001000, 10) ==
	      30);
	CHECK(efficient::block_select(
	              0b1010111010101101010001010100000101000011000010111100000101001000, 0) == 3);
	CHECK(efficient::block_select(
	              0b1010111010101101010001010100000101000011000010111100000101001000, 6) == 17);
}

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
	CHECK(b.select(0) == 1);
	CHECK(b.select(1) == 2);
	CHECK(b.select(2) == 4);
	CHECK(b.select(3) == 6);
	CHECK(b.select(4) == 8);
	CHECK(b.select(5) == 9);
	CHECK(b.next(0) == 1);
	CHECK(b.next(1) == 2);
	CHECK(b.next(2) == 4);
	CHECK(b.next(8) == 9);
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
	CHECK(b.select(0) == 1);
	CHECK(b.select(1) == 2);
	CHECK(b.select(2) == 4);
	CHECK(b.select(3) == 6);
	CHECK(b.select(4) == 8);
	CHECK(b.select(5) == 9);
	CHECK(b.next(0) == 1);
	CHECK(b.next(1) == 2);
	CHECK(b.next(2) == 4);
	CHECK(b.next(8) == 9);
}

} // namespace tests
} // namespace terraces
