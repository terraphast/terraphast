#include <catch.hpp>

#include <terraces/clamped_uint.hpp>

#include "../lib/supertree_enumerator.hpp"
#include "../lib/supertree_variants_multitree.hpp"

#include "../validation_utils.hpp"
#include "datasets.hpp"

using terraces::variants::multitree_callback;
using terraces::variants::count_callback;

namespace terraces {
namespace tests {

TEST_CASE("app_bigint", "[app]") {
	for (auto set : data_sets) {
		auto data = load(std::get<0>(set), std::get<1>(set));

		tree_enumerator<count_callback<mpz_class>> enumerator{
		        {}, data.num_species, data.constraints.size()};
		auto count = enumerator.run(data.num_species, data.constraints, data.root_species);
		SECTION(std::get<0>(set)) { CHECK(count == std::get<2>(set)); }
	}
}

TEST_CASE("app_clamped", "[app]") {
	for (auto set : data_sets) {
		auto data = load(std::get<0>(set), std::get<1>(set));

		tree_enumerator<count_callback<clamped_uint>> enumerator{
		        {}, data.num_species, data.constraints.size()};
		auto count = enumerator.run(data.num_species, data.constraints, data.root_species)
		                     .value();
		SECTION(std::get<0>(set)) {
			if (count == std::numeric_limits<std::uint64_t>::max()) {
				CHECK(count <= std::get<2>(set));
			} else {
				CHECK(count == std::get<2>(set));
			}
		}
	}
}

TEST_CASE("app_validation", "[app]") {
	for (auto set : small_data_sets) {
		auto data = load(std::get<0>(set), std::get<1>(set));

		tree_enumerator<multitree_callback> enumerator{
		        {}, data.num_species, data.constraints.size()};
		auto root = enumerator.run(data.num_species, data.constraints, data.root_species);
		SECTION(std::get<0>(set)) {
			CHECK(root->num_trees == std::get<2>(set));
			check_unique_trees(root, std::get<2>(set));
		}
	}
}

} // namespace tests
} // namespace terraces
