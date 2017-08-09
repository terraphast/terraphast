#include <catch.hpp>

#include <terraces/simple.hpp>

#include "datasets.hpp"

namespace terraces {
namespace tests {

TEST_CASE("c_api_bigint", "[c-api]") {
	for (auto set : data_sets) {
		auto count = simple::get_terrace_size_bigint_from_file(
		        "../../data/data/" + std::get<0>(set),
		        "../../data/data/" + std::get<1>(set));
		SECTION(std::get<0>(set)) { CHECK(count == std::get<2>(set)); }
	}
}

TEST_CASE("c_api", "[c-api]") {
	for (auto set : data_sets) {
		auto count =
		        simple::get_terrace_size_from_file("../../data/data/" + std::get<0>(set),
		                                           "../../data/data/" + std::get<1>(set));
		SECTION(std::get<0>(set)) {
			if (count == std::numeric_limits<std::uint64_t>::max()) {
				CHECK(count <= std::get<2>(set));
			} else {
				CHECK(count == std::get<2>(set));
			}
		}
	}
}

} // namespace tests
} // namespace terraces
