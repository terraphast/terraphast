#include <terraces/bipartitions.hpp>

namespace terraces {

std::vector<bipartition> sets_to_bipartitions(std::vector<std::vector<std::size_t>>& sets) {
	std::vector<bipartition> bips;
	std::size_t n = sets.size();
	// iterate over possible bipartitions
	for (std::size_t i = 1; i < pow(2, n - 1); i++) {
		std::size_t shifter = i;
		std::vector<index> s1 = sets.at(0);
		std::vector<index> s2;
		// i represents bitmask for bipartition
		for (std::size_t j = sets.size() - 1; j > 0; j--) {
			if (shifter & 0x01) {
				s2.insert(s2.end(), sets.at(j).begin(), sets.at(j).end());
			} else {
				s1.insert(s1.end(), sets.at(j).begin(), sets.at(j).end());
			}
			shifter >>= 1;
		}
		bips.push_back({s1, s2});
	}
	return bips;
}

} // namespace terraces
