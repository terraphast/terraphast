#include <catch.hpp>


namespace terraces {
namespace tests {

TEST_CASE("demo-testcase", "[testing-demo]") {
	const auto one = 1u;
	const auto two = 2u;
	CHECK(one + one == two);
	CHECK(one != two);
}

} // namespace tests
} // namespace terraces


