
#ifndef TERRACES_PARSER_HPP
#define TERRACES_PARSER_HPP

#include <istream>
#include <stdexcept>
#include <string>
#include <tuple>

#include "bitmatrix.hpp"
#include "trees.hpp"

namespace terraces {

struct tree_set {
	tree tree;
	name_map names;
	index_map indices;
};

/**
 * Parses a string in Newick-format and returns an
 * unrooted tree and name-table
 *
 * Warning/TODO: The exact type of the input will likely be
 * changed to something along the lines of std::string_view,
 * which should however not impact normal users in ways other than
 * performance improvements.
 */
tree_set parse_nwk(const std::string& input);

class bad_input_error : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

bitmatrix parse_bitmatrix(std::istream& input, const index_map& indices, index tree_size);

} // namespace terraces

#endif
