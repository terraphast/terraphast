#ifndef TERRACES_TEST_PERFORMANCE
#define TERRACES_TEST_PERFORMANCE

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

namespace terraces {
namespace tests {

class timer {
public:
	void start() { m_start = std::chrono::steady_clock::now(); }
	void stop() { m_end = std::chrono::steady_clock::now(); }
	std::chrono::steady_clock::duration time() const { return m_end - m_start; }
	std::uint32_t nanoseconds() const {
		return static_cast<std::uint32_t>(
		        std::chrono::duration_cast<std::chrono::nanoseconds>(time()).count());
	}

private:
	std::chrono::steady_clock::time_point m_start;
	std::chrono::steady_clock::time_point m_end;
};

class auto_timer {
public:
	auto_timer(std::string str) : m_str{std::move(str)} { m_timer.start(); }
	~auto_timer() {
		m_timer.stop();
		std::cout << m_str << m_timer.nanoseconds() << "ns\n";
	}

private:
	std::string m_str;
	timer m_timer;
};

inline void report_performance_results(const std::string& desc,
                                       std::vector<std::uint32_t> messures) {
	std::sort(messures.begin(), messures.end());
	assert(messures.size() > 1u);
	const auto sum = std::accumulate(messures.begin(), messures.end(), std::uint64_t{});
	const auto avg = sum / static_cast<double>(messures.size());
	const auto median =
	        (messures[messures.size() / 2u] + messures[(messures.size() - 1u) / 2u]) / 2.0;
	std::cout << desc << " min = " << messures.front() << "ns, max = " << messures.back()
	          << "ns, avg = " << avg << "ns, med = " << median << "ns,   sorted: ";
	std::copy(messures.begin(), messures.end(),
	          std::ostream_iterator<std::uint32_t>{std::cout, "ns, "});
	std::cout << '\n';
}

} // namespace tests
} // namespace terraces

#endif
