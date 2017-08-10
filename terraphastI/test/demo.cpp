#include <catch.hpp>

#include <iostream>

#include <gmpxx.h>

namespace terraces {
namespace tests {

TEST_CASE("demo-testcase", "[testing-demo]") {
	const auto one = mpz_class{1u};
	const auto two = mpz_class{2u};
	CHECK(one + one == two);
	CHECK(one != two);
}

} // namespace tests
} // namespace terraces
