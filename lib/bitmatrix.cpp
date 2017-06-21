
#include <terraces/bitmatrix.hpp>

namespace terraces {

bitmatrix::bitmatrix(index rows, index cols) : m_rows{rows}, m_cols{cols}, m_vec(rows * cols) {}

index bitmatrix::rows() const { return m_rows; }
index bitmatrix::cols() const { return m_cols; }

bool bitmatrix::get(index row, index col) const {
	assert(row < m_rows && col < m_cols);
	return m_vec.at(row * m_cols + col);
}

void bitmatrix::set(index row, index col, bool val) {
	assert(row < m_rows && col < m_cols);
	m_vec.at(row * m_cols + col) = val;
}

void bitmatrix::row_or(index in1, index in2, index out) {
	for (index i = 0; i < cols(); ++i) {
		set(out, i, get(in1, i) || get(in2, i));
	}
}

} // namespace terraces
