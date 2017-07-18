
#include <terraces/constraints.hpp>
#include <terraces/errors.hpp>
#include <terraces/parser.hpp>
#include <terraces/rooting.hpp>
#include <terraces/simple.hpp>
#include <terraces/subtree_extraction.hpp>
#include <terraces/supertree.hpp>
#include <terraces/trees.hpp>

#include <fstream>
#include <sstream>

#include "supertree_enumerator.hpp"
#include "supertree_variants_multitree.hpp"
#include "utils.hpp"

namespace terraces {

std::pair<constraints, index> get_constraints(std::istream& nwk_stream,
                                              std::istream& matrix_stream) {
	auto t = parse_nwk(nwk_stream);
	auto site_pair = parse_bitmatrix(matrix_stream, t.indices, t.indices.size());
	utils::ensure<no_usable_root_error>(site_pair.second != none, "no usable root found");
	reroot_inplace(t.tree, site_pair.second);
	const auto sts = subtrees(t.tree, site_pair.first);
	return {terraces::compute_constraints(sts), t.indices.size()};
}

bool is_on_terrace(std::istream& nwk_stream, std::istream& matrix_stream) {
	auto constraints = get_constraints(nwk_stream, matrix_stream);
	return check_supertree(constraints.second, constraints.first);
}

bool is_on_terrace(const std::string& nwk_string, const std::string& matrix_string) {
	auto nwk_stream = std::istringstream{nwk_string};
	auto matrix_stream = std::istringstream{matrix_string};
	return is_on_terrace(nwk_stream, matrix_stream);
}

bool is_on_terrace_from_file(const std::string& nwk_filename, const std::string& matrix_filename) {
	auto nwk_stream = std::ifstream{nwk_filename};
	utils::ensure<file_open_error>(nwk_stream.is_open(), "failed to open " + nwk_filename);
	auto matrix_stream = std::ifstream{matrix_filename};
	utils::ensure<file_open_error>(nwk_stream.is_open(), "failed to open " + matrix_filename);
	return is_on_terrace(nwk_stream, matrix_stream);
}

// TODO:
std::uint64_t get_terrace_size(std::istream& nwk_stream, std::istream& matrix_stream);
std::uint64_t get_terrace_size(const std::string& nwk_string, const std::string& matrix_string);
std::uint64_t get_terrace_size_from_file(const std::string& nwk_filename,
                                         const std::string& matrix_filename);

mpz_class get_terrace_size_as_bigint(std::istream& nwk_stream, std::istream& matrix_stream) {
	auto constraints = get_constraints(nwk_stream, matrix_stream);
	return count_supertree(constraints.second, constraints.first);
}

mpz_class get_terrace_size_as_bigint(const std::string& nwk_string,
                                     const std::string& matrix_string) {
	auto nwk_stream = std::istringstream{nwk_string};
	auto matrix_stream = std::istringstream{matrix_string};
	return get_terrace_size_as_bigint(nwk_stream, matrix_stream);
}
mpz_class get_terrace_size_as_bigint_from_file(const std::string& nwk_filename,
                                               const std::string& matrix_filename) {
	auto nwk_stream = std::ifstream{nwk_filename};
	utils::ensure<file_open_error>(nwk_stream.is_open(), "failed to open " + nwk_filename);
	auto matrix_stream = std::ifstream{matrix_filename};
	utils::ensure<file_open_error>(nwk_stream.is_open(), "failed to open " + matrix_filename);
	return get_terrace_size_as_bigint(nwk_stream, matrix_stream);
}

void print_terrace(std::istream& nwk_stream, std::istream& matrix_stream, std::ostream& output) {
	auto t = parse_nwk(nwk_stream);
	auto site_pair = parse_bitmatrix(matrix_stream, t.indices, t.indices.size());
	utils::ensure<no_usable_root_error>(site_pair.second != none, "no usable root found");
	reroot_inplace(t.tree, site_pair.second);
	const auto num_species = t.indices.size();
	const auto root_species = site_pair.second;
	const auto sts = subtrees(t.tree, site_pair.first);
	auto constraints = compute_constraints(sts);
	tree_enumerator<variants::multitree_callback> enumerator{
	        {}, num_species, constraints.size()};
	output << as_newick(enumerator.run(num_species, constraints, root_species), t.names);
}
void print_terrace(const std::string& nwk_string, const std::string& matrix_string,
                   std::ostream& output) {
	auto nwk_stream = std::istringstream{nwk_string};
	auto matrix_stream = std::istringstream{matrix_string};
	return print_terrace(nwk_stream, matrix_stream, output);
}
void print_terrace_from_file(const std::string& nwk_filename, const std::string& matrix_filename,
                             std::ostream& output) {
	auto nwk_stream = std::ifstream{nwk_filename};
	utils::ensure<file_open_error>(nwk_stream.is_open(), "failed to open " + nwk_filename);
	auto matrix_stream = std::ifstream{matrix_filename};
	utils::ensure<file_open_error>(nwk_stream.is_open(), "failed to open " + matrix_filename);
	return print_terrace(nwk_stream, matrix_stream, output);
}

} // namespace terraces
