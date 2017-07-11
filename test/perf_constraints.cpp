
#include <catch.hpp>

#include "../lib/supertree_helpers.hpp"
#include <terraces/constraints.hpp>
#include <terraces/stack_allocator.hpp>

#include <algorithm>
#include <vector>

#include "performance.hpp"

namespace terraces {
namespace tests {

TEST_CASE("performance of constraints", "[.][performance][performance_constraints]") {

	auto vec_01 = std::vector<std::uint32_t>{};
	auto vec_02 = std::vector<std::uint32_t>{};
	auto vec_03 = std::vector<std::uint32_t>{};
	for (auto i = 0u; i < 10u; ++i) {
		{
			auto fl = utils::free_list{};
			auto alloc = utils::stack_allocator<index>{fl, 3};
			auto leaves = full_ranked_set(3, alloc);
			constraints c = {constraint{7, 8, 9}};
			auto t = timer{};
			t.start();
			terraces::filter_constraints(leaves, full_set(c.size(), alloc), c);
			t.stop();
			vec_01.push_back(t.nanoseconds());
		}
		{
			auto fl = utils::free_list{};
			auto alloc = utils::stack_allocator<index>{fl, 11};
			auto leaves = full_ranked_set(11, alloc);
			constraints c = {constraint{7, 8, 9}};
			auto t = timer{};
			t.start();
			terraces::filter_constraints(leaves, full_set(c.size(), alloc), c);
			t.stop();
			vec_02.push_back(t.nanoseconds());
		}
		{
			auto fl = utils::free_list{};
			auto alloc = utils::stack_allocator<index>{fl, 13};
			auto leaves = full_ranked_set(13, alloc);
			leaves.clr(9);
			leaves.update_ranks();
			constraints c = {constraint{7, 8, 9}};
			auto t = timer{};
			t.start();
			terraces::filter_constraints(leaves, full_set(c.size(), alloc), c);
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
