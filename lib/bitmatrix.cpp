
#include <terraces/bitmatrix.hpp>

namespace terraces {

bitmatrix::bitmatrix(index rows, index cols) : m_rows{rows}, m_cols{cols}, m_vec(rows*cols) {}

index bitmatrix::rows() const { return m_rows; }
index bitmatrix::cols() const { return m_cols; }

bool bitmatrix::get(index row, index col) const {
	return m_vec.at(row*m_cols + col);
}

void bitmatrix::set(index row, index col, bool val) {
	m_vec.at(row*m_cols + col) = val;
}

} // namespace terraces
