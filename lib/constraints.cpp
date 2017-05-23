#include <terraces/constraints.hpp>

#include <ostream>

#include <terraces/union_find.hpp>

namespace terraces {

bool operator==(const constraint& c1, const constraint& c2) {
	return std::tie(c1.shared, c1.left, c1.right) == std::tie(c2.shared, c2.left, c2.right);
}

std::ostream& operator<<(std::ostream& s, const constraint& c) {
	return s << "((" << c.left << ", " << c.shared << ") < (" << c.right << ", " << c.shared
	         << "))";
}

constraints filter_constraints(const std::vector<index>& leaves, const constraints& c) {
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

std::vector<std::vector<index>> apply_constraints(index number, const constraints& c) {
	union_find leaves = make_set(number);
	for (size_t i = 0; i < c.size(); i++) {
		merge(leaves, c.at(i).shared, c.at(i).left);
	}
	return to_set_of_sets(leaves);
}

} // namespace terraces
