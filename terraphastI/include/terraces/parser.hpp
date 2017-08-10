
#ifndef TERRACES_PARSER_HPP
#define TERRACES_PARSER_HPP

#include <istream>
#include <stdexcept>
#include <string>
#include <utility>

#include "bitmatrix.hpp"
#include "trees.hpp"

namespace terraces {

/**
 * Stores a tree and its node name <-> index mapping
 */
struct tree_set {
	terraces::tree tree;
	name_map names;
	index_map indices;
};

/**
 * Parses a string in Newick-format and returns an
 * unrooted tree and name-table
 *
 * Will throw bad_input_error if the format is not obeyed.
 */
tree_set parse_nwk(const std::string& input);

/**
 * Parses a data-file and returns the associated bit-matrix as well
 * as a suitable index for rooting (or \ref none if none exists).
 */
std::pair<bitmatrix, index> parse_bitmatrix(std::istream& input, const index_map& indices,
                                            index tree_size);

} // namespace terraces

#endif
