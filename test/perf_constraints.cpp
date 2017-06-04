
#include <catch.hpp>

#include <terraces/constraints.hpp>
#include <terraces/subtree_extraction.hpp>

#include <algorithm>

#include "performance.hpp"

namespace terraces {
namespace tests {

TEST_CASE("performance of constraints", "[.][performance][performance_constraints]") {

	auto vec_01 = std::vector<std::uint32_t>{};
	auto vec_02 = std::vector<std::uint32_t>{};
	auto vec_03 = std::vector<std::uint32_t>{};
	for (auto i = 0u; i < 10u; ++i) {
		{
			std::vector<index> leaves = {0, 1, 2};
			constraints c = {constraint{7, 8, 9}};
			auto t = timer{};
			t.start();
			terraces::filter_constraints(leaves, c);
			t.stop();
			vec_01.push_back(t.nanoseconds());
		}
		{
			std::vector<index> leaves = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
			constraints c = {constraint{7, 8, 9}};
			auto t = timer{};
			t.start();
			terraces::filter_constraints(leaves, c);
			t.stop();
			vec_02.push_back(t.nanoseconds());
		}
		{
			std::vector<index> leaves = {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11};
			constraints c = {constraint{7, 8, 9}};
			auto t = timer{};
			t.start();
			terraces::filter_constraints(leaves, c);
			t.stop();
			vec_03.push_back(t.nanoseconds());
		}
	}
	report_performance_results("three leaves, one constraint, empty result: ", vec_01);
	report_performance_results("eleven leaves, one constraint, one result:  ", vec_02);
	report_performance_results("eleven leaves, one constraint, empty result:", vec_03);
}

} // namespace tests
} // namespace terraces
