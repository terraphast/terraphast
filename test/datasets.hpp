#ifndef DATASETS_HPP
#define DATASETS_HPP

#include <terraces/constraints.hpp>
#include <terraces/parser.hpp>
#include <terraces/trees.hpp>

#include <gmpxx.h>

#include <fstream>
#include <tuple>

namespace terraces {
namespace tests {

extern const std::vector<std::tuple<std::string, std::string, mpz_class>> data_sets;
extern const std::vector<std::tuple<std::string, std::string, index>> small_data_sets;
extern const std::vector<std::tuple<std::string, std::string, index>> medium_data_sets;

struct loaded_dataset {
	terraces::tree_set tree_set;
	bitmatrix site_occ;
	index num_species;
	index root_species;
	std::vector<tree> subtrees;
	terraces::constraints constraints;
};

loaded_dataset load(std::string tree_file_path, std::string data_file_path);

} // namespace tests
} // namespace terraces

#endif // DATASETS_HPP
