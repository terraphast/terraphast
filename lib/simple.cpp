
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
#include <terraces/supertree.hpp>

namespace terraces {
namespace {
std::ifstream open_ifstream(const std::string& filename) {
	auto stream = std::ifstream{filename};
	utils::ensure<file_open_error>(stream.is_open(), "failed to open " + filename);
	return stream;
}

std::pair<constraints, index> get_constraints(const std::string& nwk_string,
                                              std::istream& matrix_stream) {
	auto t = parse_nwk(nwk_string);
	auto site_pair = parse_bitmatrix(matrix_stream, t.indices, t.tree.size());
	utils::ensure<no_usable_root_error>(site_pair.second != none, "no usable root found");
	reroot_inplace(t.tree, site_pair.second);
	const auto sts = subtrees(t.tree, site_pair.first);
	auto constraints = compute_constraints(sts);
	deduplicate_constraints(constraints);
	const auto num_species = remap_to_leaves(t.tree, constraints, t.names, site_pair.second);
	return {constraints, num_species};
}

std::string read_all(std::istream& stream) {
	using it = std::istreambuf_iterator<char>;
	return {it{stream}, it{}};
}

std::string read_file(const std::string& filename) {
	auto file = open_ifstream(filename);
	return read_all(file);
}

} // anonymous namespace

bool is_on_terrace(const std::string& nwk_string, std::istream& matrix_stream) {
	auto constraints = get_constraints(nwk_string, matrix_stream);
	return check_supertree(constraints.second, constraints.first);
}

bool is_on_terrace(std::istream& nwk_stream, std::istream& matrix_stream) {
	return is_on_terrace(read_all(nwk_stream), matrix_stream);
}

bool is_on_terrace(std::istream& nwk_stream, const std::string& matrix_string) {
	auto matrix_stream = std::istringstream{matrix_string};
	return is_on_terrace(read_all(nwk_stream), matrix_stream);
}

bool is_on_terrace(const std::string& nwk_string, const std::string& matrix_string) {
	auto matrix_stream = std::istringstream{matrix_string};
	return is_on_terrace(nwk_string, matrix_stream);
}

bool is_on_terrace_from_file(const std::string& nwk_filename, const std::string& matrix_filename) {
	auto nwk_string = read_file(nwk_filename);
	auto matrix_stream = open_ifstream(matrix_filename);
	return is_on_terrace(nwk_string, matrix_stream);
}

// TODO:
std::uint64_t get_terrace_size(std::istream& nwk_stream, std::istream& matrix_stream);
std::uint64_t get_terrace_size(std::istream& nwk_stream, const std::string& matrix_string);
std::uint64_t get_terrace_size(const std::string& nwk_string, std::istream& matrix_stream);
std::uint64_t get_terrace_size(const std::string& nwk_string, const std::string& matrix_string);
std::uint64_t get_terrace_size_from_file(const std::string& nwk_filename,
                                         const std::string& matrix_filename);

mpz_class get_terrace_size_as_bigint(const std::string& nwk_string, std::istream& matrix_stream) {
	auto constraints = get_constraints(nwk_string, matrix_stream);
	return count_supertree(constraints.second, constraints.first);
}

mpz_class get_terrace_size_as_bigint(std::istream& nwk_stream, std::istream& matrix_stream) {
	return get_terrace_size_as_bigint(read_all(nwk_stream), matrix_stream);
}

mpz_class get_terrace_size_as_bigint(std::istream& nwk_stream, const std::string& matrix_string) {
	auto matrix_stream = std::istringstream{matrix_string};
	return get_terrace_size_as_bigint(read_all(nwk_stream), matrix_stream);
}

mpz_class get_terrace_size_as_bigint(const std::string& nwk_string,
                                     const std::string& matrix_string) {
	auto matrix_stream = std::istringstream{matrix_string};
	return get_terrace_size_as_bigint(nwk_string, matrix_stream);
}
mpz_class get_terrace_size_as_bigint_from_file(const std::string& nwk_filename,
                                               const std::string& matrix_filename) {
	auto nwk_string = read_file(nwk_filename);
	auto matrix_stream = open_ifstream(matrix_filename);
	return get_terrace_size_as_bigint(nwk_string, matrix_stream);
}

mpz_class print_terrace(const std::string& nwk_string, std::istream& matrix_stream,
                        std::ostream& output) {
	auto t = parse_nwk(nwk_string);
	auto site_pair = parse_bitmatrix(matrix_stream, t.indices, t.tree.size());
	utils::ensure<no_usable_root_error>(site_pair.second != none, "no usable root found");
	reroot_inplace(t.tree, site_pair.second);
	const auto sts = subtrees(t.tree, site_pair.first);
	auto constraints = compute_constraints(sts);
	deduplicate_constraints(constraints);
	const auto num_species = remap_to_leaves(t.tree, constraints, t.names, site_pair.second);
	tree_enumerator<variants::multitree_callback> enumerator{
	        {}, num_species, constraints.size()};
	const auto enumeration = enumerator.run(num_species, constraints, site_pair.second);
	output << as_newick(enumeration, t.names);
	return enumeration->num_trees;
}

mpz_class print_terrace(std::istream& nwk_stream, const std::string& matrix_string,
                        std::ostream& out) {
	auto matrix_stream = std::istringstream{matrix_string};
	return print_terrace(read_all(nwk_stream), matrix_stream, out);
}

mpz_class print_terrace(std::istream& nwk_stream, std::istream& matrix_stream, std::ostream& out) {
	return print_terrace(read_all(nwk_stream), matrix_stream, out);
}

mpz_class print_terrace(const std::string& nwk_string, const std::string& matrix_string,
                        std::ostream& output) {
	auto matrix_stream = std::istringstream{matrix_string};
	return print_terrace(nwk_string, matrix_stream, output);
}

mpz_class print_terrace_from_file(const std::string& nwk_filename,
                                  const std::string& matrix_filename, std::ostream& output) {
	auto nwk_string = read_file(nwk_filename);
	auto matrix_stream = open_ifstream(matrix_filename);
	return print_terrace(nwk_string, matrix_stream, output);
}

} // namespace terraces
