#include <terraces/fast_set.hpp>

#include <algorithm>
#include <cassert>

namespace terraces {

fast_index_set::fast_index_set(index max_size) : m_vector{max_size} {}

bool fast_index_set::contains(index i) const { return m_vector.get(i); }

index fast_index_set::rank(index i) const {
	assert(contains(i));
	return m_vector.rank(i);
}

index fast_index_set::max_size() const { return m_vector.size(); }

index fast_index_set::size() const { return m_vector.count(); }

void fast_index_set::insert(index i) { m_vector.set(i); }

void fast_index_set::remove(index i) { m_vector.clr(i); }

void fast_index_set::toggle(index i) { m_vector.flip(i); }

void fast_index_set::symm_difference(const fast_index_set& other) {
	m_vector.bitwise_xor(other.m_vector);
}

void fast_index_set::clear() { m_vector.blank(); }

void fast_index_set::finalize_edit() { m_vector.update_ranks(); }

} // namespace terraces
