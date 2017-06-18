#include <terraces/union_find.hpp>

#include <algorithm>
#include <numeric>

namespace terraces {

union_find::union_find(index n) : m_id(n), m_size(n, 1) {
	std::iota(m_id.begin(), m_id.end(), index{});
}

index union_find::find(index x) const {
	index root = x;
	while (root != m_id[root]) {
		root = m_id[root];
	}
	while (x != root) {
		x = std::exchange(m_id[x], root);
	}
	return root;
}

index union_find::size() const { return m_id.size(); }

void union_find::merge(index x, index y) {
	index i = find(x);
	index j = find(y);
	if (i == j) {
		return;
	} else if (m_size[i] >= m_size[j]) {
		std::swap(i, j);
	}
	m_id[i] = j;
	m_size[j] += m_size[i];
}

bool union_find::is_representative(index x) const { return x == find(x); }

} // namespace terraces
