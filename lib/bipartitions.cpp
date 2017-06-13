#include <terraces/bipartitions.hpp>

#include <cassert>

namespace terraces {

bipartition_iterator::bipartition_iterator(const fast_index_set& leaves, const union_find& sets)
        : m_leaves{leaves}, m_sets{sets}, m_set_rep{find_set_reps()},
          m_subleaves{leaves.max_size()}, m_bip{0}, m_end{(1u << (m_set_rep.size() - 1))} {
	assert(leaves.max_size() < 64);
	increase();
}

bool bipartition_iterator::in_left_partition(index i) const { return m_bip & (1 << (i - 1)); }

fast_index_set bipartition_iterator::find_set_reps() const {
	// TODO ugly style :)
	fast_index_set set_rep(m_leaves.size());
	for (index i = 0; i < m_leaves.size(); ++i) {
		set_rep.insert(m_sets.find(i));
	}
	set_rep.finalize_edit();
	return set_rep;
}

bool bipartition_iterator::is_valid() const { return m_bip < m_end; }

void bipartition_iterator::increase() {
	m_bip++;
	// TODO can we do better?
	if (is_valid()) {
		m_subleaves.clear();
		index ii = 0;
		for (auto i : m_leaves) {
			if (in_left_partition(m_set_rep.rank(m_sets.find(ii)))) {
				m_subleaves.insert(i);
			}
			++ii;
		}
		m_subleaves.finalize_edit();
	}
}

const fast_index_set& bipartition_iterator::get_current_set() const { return m_subleaves; }

void bipartition_iterator::flip_sets() {
	m_subleaves.symm_difference(m_leaves);
	m_subleaves.finalize_edit();
}

} // namespace terraces
