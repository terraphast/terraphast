#include <terraces/constraints.hpp>
#include <terraces/union_find.hpp>

namespace terraces {

constraints filter_constraints(std::vector<index> leaves, constraints c) {
	constraints new_c;
	for (size_t i = 0; i < c.size(); i++) {
		if ((std::find(leaves.begin(), leaves.end(), c.at(i).shared) != leaves.end()) &&
		    (std::find(leaves.begin(), leaves.end(), c.at(i).left) != leaves.end()) &&
		    (std::find(leaves.begin(), leaves.end(), c.at(i).right) != leaves.end())) {
			new_c.push_back(c.at(i));
		}
	}
	return new_c;
}

std::vector<std::vector<index>> apply_constraints(std::vector<index> species, constraints c) {
	union_find leaves = make_set(species.size());
	for (size_t i = 0; i < c.size(); i++) {
		merge(leaves, c.at(i).shared, c.at(i).left);
	}
	return to_set_of_sets(leaves);
}

} // namespace terraces
