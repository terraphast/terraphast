#ifndef TERRACES_SIMPLE_HPP
#define TERRACES_SIMPLE_HPP

#include <cstdint>
#include <iosfwd>
#include <string>

#include <gmpxx.h>

namespace terraces {

/*
 * All these functions may throw bad_input_error if the input is invalid.
 *
 * The non-GMP count methods may throw a tree_count_overflow_error if
 * the returntype is to small to represent the number of trees.
 */

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
 * \returns the number of trees on the terrace
 */
std::uint64_t get_terrace_size(std::istream& nwk_stream, std::istream& matrix_stream);
std::uint64_t get_terrace_size(std::istream& nwk_stream, const std::string& matrix_string);
std::uint64_t get_terrace_size(const std::string& nwk_string, std::istream& matrix_stream);
std::uint64_t get_terrace_size(const std::string& nwk_string, const std::string& matrix_string);
std::uint64_t get_terrace_size_from_file(const std::string& nwk_filename,
                                         const std::string& matrix_filename);

mpz_class get_terrace_size_bigint(std::istream& nwk_stream, std::istream& matrix_stream);
mpz_class get_terrace_size_bigint(std::istream& nwk_stream, const std::string& matrix_string);
mpz_class get_terrace_size_bigint(const std::string& nwk_string, std::istream& matrix_stream);
mpz_class get_terrace_size_bigint(const std::string& nwk_string, const std::string& matrix_string);
mpz_class get_terrace_size_bigint_from_file(const std::string& nwk_filename,
                                            const std::string& matrix_filename);

/**
 * Print the extended-nwk-representation of all trees to the provided output.
 *
 * This function may run longer if the number of such trees is very big!
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

} // namespace terraces

#endif
