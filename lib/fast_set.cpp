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

auto fast_index_set::begin() const -> iterator { return iterator{this, m_vector.begin()}; }

auto fast_index_set::end() const -> iterator { return iterator{this, m_vector.end()}; }

void fast_index_set::insert(index i) { m_vector.set(i); }

void fast_index_set::remove(index i) { m_vector.clr(i); }

void fast_index_set::clear() { m_vector.blank(); }

void fast_index_set::finalize_edit() { m_vector.update_ranks(); }

fast_index_set_iterator::fast_index_set_iterator(const fast_index_set* set, index i)
        : m_set{set}, m_index{i} {}

fast_index_set_iterator& fast_index_set_iterator::operator++() {
	m_index = m_set->m_vector.next(m_index);
	return *this;
}

bool fast_index_set_iterator::operator==(const fast_index_set_iterator& other) const {
	return m_index == other.m_index;
}

bool fast_index_set_iterator::operator!=(const fast_index_set_iterator& other) const {
	return !(*this == other);
}

const index& fast_index_set_iterator::operator*() const { return m_index; }

} // namespace terraces
