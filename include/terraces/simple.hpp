#ifndef TERRACES_SIMPLE_HPP
#define TERRACES_SIMPLE_HPP

#include <cstdint>
#include <iosfwd>
#include <string>

#include <gmpxx.h>

namespace terraces {
namespace simple {

/**
 * Check whether the given tree is on a terrace.
 * \returns true if there is at least one other tree on the terrace.
 */
bool is_on_terrace(std::istream& nwk_stream, std::istream& matrix_stream);
bool is_on_terrace(std::istream& nwk_stream, const std::string& matrix_string);
bool is_on_terrace(const std::string& nwk_string, std::istream& matrix_stream);
bool is_on_terrace(const std::string& nwk_string, const std::string& matrix_string);
bool is_on_terrace_from_file(const std::string& nwk_filename, const std::string& matrix_filename);

/**
 * Count the number of trees on the terrace.
 * \returns the number of trees on the terrace. If the number of trees is not representable in 64
 * bits,
 * this method returns the maximum value of uint64_t instead.
 */
std::uint64_t get_terrace_size(std::istream& nwk_stream, std::istream& matrix_stream);
std::uint64_t get_terrace_size(std::istream& nwk_stream, const std::string& matrix_string);
std::uint64_t get_terrace_size(const std::string& nwk_string, std::istream& matrix_stream);
std::uint64_t get_terrace_size(const std::string& nwk_string, const std::string& matrix_string);
std::uint64_t get_terrace_size_from_file(const std::string& nwk_filename,
                                         const std::string& matrix_filename);

/**
 * Count the number of trees on the terrace.
 * \returns the number of trees on the terrace. If the number of trees is not representable in 64
 * bits,
 * this method returns the maximum value of uint64_t instead.
 * \throws tree_count_overflow_error if the method will not terminate in any usable timeframe.
 */
mpz_class get_terrace_size_bigint(std::istream& nwk_stream, std::istream& matrix_stream);
mpz_class get_terrace_size_bigint(std::istream& nwk_stream, const std::string& matrix_string);
mpz_class get_terrace_size_bigint(const std::string& nwk_string, std::istream& matrix_stream);
mpz_class get_terrace_size_bigint(const std::string& nwk_string, const std::string& matrix_string);
mpz_class get_terrace_size_bigint_from_file(const std::string& nwk_filename,
                                            const std::string& matrix_filename);

/**
 * Print the multitree representation of all trees to the provided output.
 *
 * Note that the output may be quite large!
 * \throws tree_count_overflow_error if the method will not terminate in any usable timeframe.
 */
mpz_class print_terrace(std::istream& nwk_stream, std::istream& matrix_stream, std::ostream& out);
mpz_class print_terrace(std::istream& nwk_stream, const std::string& matrix_string,
                        std::ostream& out);
mpz_class print_terrace(const std::string& nwk_string, std::istream& matrix_stream,
                        std::ostream& out);
mpz_class print_terrace(const std::string& nwk_string, const std::string& matrix_string,
                        std::ostream& out);
mpz_class print_terrace_from_file(const std::string& nwk_filename,
                                  const std::string& matrix_filename, std::ostream& output);

} // namespace simple
} // namespace terraces

#endif
