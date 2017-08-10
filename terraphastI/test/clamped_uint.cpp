#include <catch.hpp>

#include <terraces/clamped_uint.hpp>

#include <limits>

namespace terraces {
namespace tests {

TEST_CASE("clamped_uint", "[clamped_uint]") {
	auto max = std::numeric_limits<uint64_t>::max();
	CHECK((clamped_uint{10} + clamped_uint{417}).value() == 10 + 417);
	CHECK((clamped_uint{10} * clamped_uint{417}).value() == 10 * 417);
	CHECK((clamped_uint{max} + 1).is_clamped());
	CHECK((clamped_uint{max} + 1).value() == max);
	CHECK((clamped_uint{max / 2} * 3).is_clamped());
	CHECK((clamped_uint{max / 2} * 3).value() == max);
}

} // namespace tests
} // namespace terraces
