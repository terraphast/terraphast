#include <terraces/supertree.hpp>

#include <iostream>
#include <unordered_map>

std::string prefix;

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
	prefix = "";
	return count_supertree(leaves, c);
}

size_t count_supertree(const std::vector<index>& leaves, const constraints& c) {
	size_t number = 0;
	std::cout << prefix << "leaves {";
	for (auto l : leaves) {
		std::cout << l << ",";
	}
	std::cout << "}\n";
	std::cout << prefix << "constraints {";
	for (auto cc : c) {
		std::cout << cc << ",";
	}
	std::cout << "}\n";

	if (leaves.size() == 2) {
		std::cout << prefix << "result: 1\n";
		return 1;
	}

	if (c.size() == 0) {
		size_t res = 1;
		for (size_t i = 3; i <= leaves.size() + 1; i++) {
			res *= (2 * i - 5);
		}
		std::cout << prefix << "result: " << res << "\n";
		return res;
	}

	constraints new_c = map_constraints(leaves, c);
	std::vector<std::vector<index>> sets = apply_constraints(leaves.size(), new_c);
	sets = map_sets(leaves, sets);

	std::cout << prefix << "sets ";
	for (auto& set : sets) {
		std::cout << prefix << "{";
		for (auto el : set) {
			std::cout << el << ",";
		}
		std::cout << "}, ";
	}
	std::cout << "\n";

	for (bipartition_iterator bip_it(sets); bip_it.is_valid(); bip_it.increase()) {
		std::vector<index> left_set = std::get<0>(bip_it.get_bipartition());
		std::vector<index> right_set = std::get<1>(bip_it.get_bipartition());

		constraints left_bips = filter_constraints(left_set, c);
		constraints right_bips = filter_constraints(right_set, c);

		std::cout << prefix << "subcall for bipartition " << bip_it << "{";
		for (auto el : left_set) {
			std::cout << el << ",";
		}
		std::cout << "}, {";
		for (auto el : right_set) {
			std::cout << el << ",";
		}
		std::cout << "}\n";

		prefix += "  ";

		auto cl = count_supertree(left_set, left_bips);
		auto cr = count_supertree(right_set, right_bips);

		prefix = std::string{prefix.begin() + 2, prefix.end()};

		std::cout << prefix << "subcalls returned " << cl << " and " << cr << "\n";

		number += cl * cr;
	}

	std::cout << prefix << "result: " << number << "\n";
	return number;
}

} // namespace terraces
