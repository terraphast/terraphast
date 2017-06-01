#include <terraces/fast_set.hpp>

#include <algorithm>
#include <cassert>

namespace terraces {

bitvector::bitvector(index size) : m_vector(size) {}

bool bitvector::get(index i) const {
	assert(i < m_vector.size());
	return m_vector[i];
}

void bitvector::set(index i, bool b) {
	assert(i < m_vector.size());
	m_vector[i] = b;
}

index bitvector::rank(index i) const {
	assert(i <= m_vector.size());
	return (index)std::count(m_vector.begin(),
	                         m_vector.begin() + (std::vector<bool>::difference_type)i, true);
}

index bitvector::select(index i) const {
	index n = m_vector.size();
	assert(i <= rank(n));
	assert(i > 0);
	index rank = 0;
	index j;
	for (j = 0; j < n; ++j) {
		rank += get(j);
		if (rank == i) {
			break;
		}
	}
	return j;
}

index bitvector::next_set_bit(index i) const {
	index j;
	for (j = i + 1; j < m_vector.size(); ++j) {
		if (m_vector[j]) {
			break;
		}
	}
	return j;
}

fast_index_set::fast_index_set(index size) : m_dirty{false}, m_set{}, m_occ(size) {}

bool fast_index_set::contains(index i) const {
	assert(i < m_occ.size());
	return m_occ[i];
}

index fast_index_set::size() const { return m_set.size(); }

void fast_index_set::fill(bool val) {
	if (val) {
		for (auto&& v : m_occ) {
			v = true;
		}
		for (index i = 0; i < m_occ.size(); ++i) {
			m_set.insert(i);
		}
	} else {
		for (auto&& v : m_occ) {
			v = false;
		}
		m_set.clear();
	}
	m_dirty = true;
	finalize_edit();
}

fast_index_set::const_iterator fast_index_set::begin() const {
	assert(!m_dirty);
	return m_set.cbegin();
}

fast_index_set::const_iterator fast_index_set::find(index i) const {
	assert(!m_dirty);
	assert(contains(i));
	return m_set.find(i);
}

fast_index_set::const_iterator fast_index_set::end() const {
	assert(!m_dirty);
	return m_set.cend();
}

void fast_index_set::reset_and_resize(index size) {
	m_set.clear();
	m_occ.resize(size);
	fill(false);
}

void fast_index_set::insert_element(index i) {
	assert(i < m_occ.size());
	m_set.emplace(i);
	m_occ[i] = true;
	m_dirty = true;
}

void fast_index_set::delete_element(index i) {
	assert(i < m_occ.size());
	assert(contains(i));
	assert(m_set.erase(i));
	m_occ[i] = false;
	m_dirty = true;
}

void fast_index_set::finalize_edit() {
	// so far, we don't need this
	m_dirty = false;
}
} // namespace terraces