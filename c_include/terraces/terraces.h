
#ifndef TERRACES_TERRACES_H
#define TERRACES_TERRACES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <gmp.h>

#ifdef __cplusplus
#define TERRACES_NOEXCEPT noexcept
extern "C" {
#else
#define TERRACES_NOEXCEPT
#endif

// data type containing data to be passed to the algorithm we want to implement

typedef struct {
	size_t num_species;
	size_t num_partitions;
	unsigned char* matrix;
	char** names;
	bool allocatedNameArray;
} terraces_missing_data;

enum terraces_errors {
	terraces_success = 0,
	terraces_out_of_memory_error = 1,
	terraces_invalid_input_error = 2,
	terraces_no_usable_root_error = 3,
	terraces_file_open_error = 4,
	terraces_unknown_error = 5,
	terraces_tree_count_overflow_error = 6
};

terraces_errors terraces_check_tree(const terraces_missing_data* missing_data,
                                    const char* nwk_string, bool* out) TERRACES_NOEXCEPT;

terraces_errors terraces_count_tree(const terraces_missing_data* missing_data,
                                    const char* nwk_string, mpz_t out) TERRACES_NOEXCEPT;

terraces_errors terraces_print_tree(const terraces_missing_data* missing_data,
                                    const char* nwk_string, mpz_t out,
                                    const char* output_filename) TERRACES_NOEXCEPT;

terraces_errors terraces_check_tree_str(const char* missing_data, const char* nwk_string,
                                        bool* out) TERRACES_NOEXCEPT;

terraces_errors terraces_count_tree_str(const char* missing_data, const char* nwk_string,
                                        mpz_t out) TERRACES_NOEXCEPT;

terraces_errors terraces_print_tree_str(const char* missing_data, const char* nwk_string, mpz_t out,
                                        const char* output_filename) TERRACES_NOEXCEPT;

#ifdef __cplusplus
} // extern "C"
#endif

#endif
