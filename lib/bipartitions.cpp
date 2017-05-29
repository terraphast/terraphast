#include <terraces/bipartitions.hpp>

#include <cassert>

namespace terraces {

bipartition_iterator::bipartition_iterator(const std::vector<std::vector<index>>& sets)
        : bip{1}, end{(1u << (sets.size() - 1))}, sets{sets} {
	assert(sets.size() < 64);
}

bipartition bipartition_iterator::get_bipartition() {
	std::vector<index> s1 = sets.at(0);
	std::vector<index> s2;
	for (size_t i = sets.size() - 1; i > 0; i--) {
		if (bip & (1 << (i - 1))) {
			s2.insert(s2.end(), sets.at(i).begin(), sets.at(i).end());
		} else {
			s1.insert(s1.end(), sets.at(i).begin(), sets.at(i).end());
		}
	}
	return bipartition{s1, s2};
}

void bipartition_iterator::increase() { bip++; }

bool bipartition_iterator::is_valid() { return bip < end; }

} // namespace terraces
