#ifndef CLAMPED_INT_H
#define CLAMPED_INT_H

#include <cstdint>
#include <limits>
#include <safeint.h>

namespace terraces {

class clamped_uint {
private:
	using num_t = std::uint64_t;
	num_t m_value;

	static constexpr num_t m_max = std::numeric_limits<num_t>::max();

public:
	clamped_uint(uint64_t value = 0) : m_value{value} {}

	clamped_uint& operator+=(clamped_uint other) {
		if (m_max - other.m_value < m_value) {
			m_value = m_max;
		} else {
			m_value += other.m_value;
		}
		return *this;
	}

	clamped_uint& operator*=(clamped_uint other) {
		if (m_max / other.m_value < m_value) {
			m_value = m_max;
		} else {
			m_value *= other.m_value;
		}
		return *this;
	}

	bool is_clamped() const { return m_value == m_max; }

	num_t value() const { return m_value; }
};

inline clamped_uint operator+(clamped_uint a, clamped_uint b) { return a += b; }

inline clamped_uint operator*(clamped_uint a, clamped_uint b) { return a *= b; }

} // namespace terraces

#endif // CLAMPED_INT_H
