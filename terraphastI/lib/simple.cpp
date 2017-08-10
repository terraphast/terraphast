#include <terraces/advanced.hpp>
#include <terraces/errors.hpp>
#include <terraces/parser.hpp>
#include <terraces/rooting.hpp>
#include <terraces/simple.hpp>

#include <fstream>
#include <sstream>

#include "utils.hpp"

namespace terraces {
namespace simple {

namespace {

std::ifstream open_ifstream(const std::string& filename) {
	auto stream = std::ifstream{filename};
	utils::ensure<file_open_error>(stream.is_open(), "failed to open " + filename);
	return stream;
}

std::pair<supertree_data, name_map> parse_data(const std::string& nwk_string,
                                               std::istream& matrix_stream) {
	auto t = parse_nwk(nwk_string);
	auto site_pair = parse_bitmatrix(matrix_stream, t.indices, t.tree.size());
	utils::ensure<no_usable_root_error>(site_pair.second != none, "no usable root found");
	reroot_inplace(t.tree, site_pair.second);
	auto data = prepare_constraints(t.tree, site_pair.first, t.names, site_pair.second);
	return {data, t.names};
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
	return check_terrace(parse_data(nwk_string, matrix_stream).first);
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

std::uint64_t get_terrace_size(const std::string& nwk_string, std::istream& matrix_stream) {
	return count_terrace(parse_data(nwk_string, matrix_stream).first);
}

std::uint64_t get_terrace_size(std::istream& nwk_stream, std::istream& matrix_stream) {
	return get_terrace_size(read_all(nwk_stream), matrix_stream);
}

std::uint64_t get_terrace_size(std::istream& nwk_stream, const std::string& matrix_string) {
	auto matrix_stream = std::istringstream{matrix_string};
	return get_terrace_size(read_all(nwk_stream), matrix_stream);
}

std::uint64_t get_terrace_size(const std::string& nwk_string, const std::string& matrix_string) {
	auto matrix_stream = std::istringstream{matrix_string};
	return get_terrace_size(nwk_string, matrix_stream);
}
std::uint64_t get_terrace_size_from_file(const std::string& nwk_filename,
                                         const std::string& matrix_filename) {
	auto nwk_string = read_file(nwk_filename);
	auto matrix_stream = open_ifstream(matrix_filename);
	return get_terrace_size(nwk_string, matrix_stream);
}

mpz_class get_terrace_size_bigint(const std::string& nwk_string, std::istream& matrix_stream) {
	return count_terrace_bigint(parse_data(nwk_string, matrix_stream).first);
}

mpz_class get_terrace_size_bigint(std::istream& nwk_stream, std::istream& matrix_stream) {
	return get_terrace_size_bigint(read_all(nwk_stream), matrix_stream);
}

mpz_class get_terrace_size_bigint(std::istream& nwk_stream, const std::string& matrix_string) {
	auto matrix_stream = std::istringstream{matrix_string};
	return get_terrace_size_bigint(read_all(nwk_stream), matrix_stream);
}

mpz_class get_terrace_size_bigint(const std::string& nwk_string, const std::string& matrix_string) {
	auto matrix_stream = std::istringstream{matrix_string};
	return get_terrace_size_bigint(nwk_string, matrix_stream);
}
mpz_class get_terrace_size_bigint_from_file(const std::string& nwk_filename,
                                            const std::string& matrix_filename) {
	auto nwk_string = read_file(nwk_filename);
	auto matrix_stream = open_ifstream(matrix_filename);
	return get_terrace_size_bigint(nwk_string, matrix_stream);
}

mpz_class print_terrace(const std::string& nwk_string, std::istream& matrix_stream,
                        std::ostream& output) {

	auto data = parse_data(nwk_string, matrix_stream);
	return print_terrace(data.first, data.second, output);
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

} // namespace simple
} // namespace terraces
