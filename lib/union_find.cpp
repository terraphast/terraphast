#include <terraces/union_find.hpp>

#include <algorithm>
#include <numeric>

namespace terraces {

union_find::union_find(index n, utils::stack_allocator<index> a) : m_parent(n, n, a) {
#ifndef NDEBUG
	m_compressed = true;
#endif
}

index union_find::find(index x) {
	index root = x;
	while (!is_representative(root)) {
		root = m_parent[root];
	}
	while (x != root) {
		x = std::exchange(m_parent[x], root);
	}
	assert(is_representative(root) && root < m_parent.size());
	return root;
}

index union_find::size() const { return m_parent.size(); }

void union_find::compress() {
	for (index i = 0; i < m_parent.size(); ++i) {
		find(i);
	}
#ifndef NDEBUG
	m_compressed = true;
#endif
}

void union_find::merge(index x, index y) {
#ifndef NDEBUG
	m_compressed = false;
#endif
	index i = find(x);
	index j = find(y);
	if (i == j) {
		return;
	}
	if (m_parent[i] < m_parent[j]) {
		// link the smaller group to the larger one
		m_parent[i] = j;
	} else if (m_parent[i] > m_parent[j]) {
		// link the smaller group to the larger one
		m_parent[j] = i;
	} else {
		// equal rank: link arbitrarily and increase rank
		m_parent[j] = i;
		++m_parent[i];
	}
}

bool union_find::is_representative(index x) const { return m_parent[x] >= m_parent.size(); }

} // namespace terraces
