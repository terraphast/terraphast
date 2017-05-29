#include <terraces/supertree.hpp>

#include <unordered_map>

namespace terraces {

constraints map_constraints(const std::vector<index>& leaves, constraints c) {
	std::unordered_map<index, index> m;
	for (size_t i = 0; i < leaves.size(); i++) {
		m[leaves.at(i)] = i;
	}
	for (size_t i = 0; i < c.size(); i++) {
		auto& constraint = c.at(i);
		constraint.left = m[constraint.left];
		constraint.right = m[constraint.right];
		constraint.shared = m[constraint.shared];
	}
	return c;
}

std::vector<std::vector<index>> map_sets(std::vector<index> leaves,
                                         std::vector<std::vector<index>> sets) {
	std::vector<std::vector<index>> res;
	for (size_t i = 0; i < sets.size(); i++) {
		std::vector<index> set;
		for (size_t j = 0; j < sets.at(i).size(); j++) {
			set.push_back(leaves.at(sets.at(i).at(j)));
		}
		res.push_back(set);
	}
	return res;
}

bool check_supertree(index number, const constraints& c) {
	std::vector<index> leaves(number);
	for (size_t i = 0; i < number; i++) {
		leaves.at(i) = i;
	}
	return check_supertree(leaves, c);
}

bool check_supertree(const std::vector<index>& leaves, const constraints& c) {
	// only one tree possible for two leaves
	if (leaves.size() <= 2) {
		return false;
	}

	// on a terrace if more than two leaves and no contraints
	if (c.size() == 0) {
		return true;
	}

	constraints new_c = map_constraints(leaves, c);
	std::vector<std::vector<index>> sets = apply_constraints(leaves.size(), new_c);
	sets = map_sets(leaves, sets);

	// on a terrace if more than one bipartition
	if (sets.size() > 2) {
		return true;
	}

	std::vector<index> left_set = sets.at(0);
	std::vector<index> right_set = sets.at(1);

	constraints left_bips = filter_constraints(left_set, c);
	constraints right_bips = filter_constraints(right_set, c);

	if (check_supertree(left_set, left_bips)) {
		return true;
	} else {
		return check_supertree(right_set, right_bips);
	}
}

size_t count_supertree(index number, const constraints& c) {
	std::vector<index> leaves(number);
	for (size_t i = 0; i < number; i++) {
		leaves.at(i) = i;
	}
	return count_supertree(leaves, c);
}

size_t count_supertree(const std::vector<index>& leaves, const constraints& c) {
	size_t number = 0;

	if (leaves.size() == 2) {
		return 1;
	}

	if (c.size() == 0) {
		size_t res = 1;
		for (size_t i = 3; i <= leaves.size() + 1; i++) {
			res *= (2 * i - 5);
		}
		return res;
	}

	constraints new_c = map_constraints(leaves, c);
	std::vector<std::vector<index>> sets = apply_constraints(leaves.size(), new_c);
	sets = map_sets(leaves, sets);

	for (bipartition_iterator bip_it(sets); bip_it.is_valid(); bip_it.increase()) {
		std::vector<index> left_set = std::get<0>(bip_it.get_bipartition());
		std::vector<index> right_set = std::get<1>(bip_it.get_bipartition());

		constraints left_bips = filter_constraints(left_set, c);
		constraints right_bips = filter_constraints(right_set, c);

		number += count_supertree(left_set, left_bips) *
		          count_supertree(right_set, right_bips);
	}

	return number;
}

} // namespace terraces
