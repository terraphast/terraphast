#ifndef DATASETS_HPP
#define DATASETS_HPP

#include <terraces/constraints.hpp>
#include <terraces/parser.hpp>
#include <terraces/rooting.hpp>
#include <terraces/subtree_extraction.hpp>
#include <terraces/trees.hpp>

#include "../lib/multitree_iterator.hpp"
#include "../lib/supertree_enumerator.hpp"
#include "../lib/supertree_variants.hpp"
#include "../lib/supertree_variants_multitree.hpp"
#include "../lib/validation.hpp"

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
void check_unique_trees(multitree_node* root, index num_trees);
void check_subtrees(multitree_node* root, const loaded_dataset& dataset);

} // namespace tests
} // namespace terraces

#endif // DATASETS_HPP
