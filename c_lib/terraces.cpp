
#include <terraces/terraces.h>

// The implementation will of the C-wrapper will of course be done in C++:

#include <fstream>

#include <terraces/errors.hpp>
#include <terraces/simple.hpp>
#include <terraces/bitmatrix.hpp>

namespace {

template <typename Function>
terraces_errors exec_and_catch(Function f) {
	// TODO: deal with all exceptions:
	try {
		f();
		return terraces_success;
	} catch (std::bad_alloc& e) {
		return terraces_out_of_memory_error;
	} catch (terraces::bad_input_error) {
		return terraces_invalid_input_error;
	} catch (...) {
		return terraces_unknown_error;
	}
}

std::ofstream open_output_file(const char* filename) {
	auto ret = std::ofstream{filename};
	if (not ret.is_open()) {
		throw terraces::file_open_error{""};
	}
	return ret;
}

std::pair<terraces::bitmatrix, terraces::index> to_bitmatrix(const terraces_missing_data* missing_data_ptr) {
	if (not missing_data_ptr) {
		throw terraces::bad_input_error{"nullpointer"};
	}
	const auto& missing_data = *missing_data_ptr;
	auto bm = terraces::bitmatrix{missing_data.num_species, missing_data.num_partitions};
	auto root = terraces::none;
	for (auto row = terraces::index{}; row < missing_data.num_species; ++row) {
		auto all_known = true;
		auto any_known = false;
		for (auto col = terraces::index{}; col < missing_data.num_partitions; ++col) {
			const auto known = missing_data.matrix[row * missing_data.num_partitions + col];
			all_known &= known;
			any_known |= known;
			bm.set(row, col, known);
		}
		if (all_known and root == terraces::none) {
			root = row;
		}
		else if (not any_known) {
			throw terraces::bad_input_error{""};
		}
	}
	return {bm, root};
}

} // anonymous namespace

extern "C" {

terraces_errors terraces_check_tree(const terraces_missing_data* missing_data,
                                    const char* nwk_string, bool* out) noexcept {
	// TODO: implement this using missing_data
	auto sites = to_bitmatrix(missing_data);
	(void)sites;
	(void)nwk_string;
	(void)out;
	return terraces_success;
}

terraces_errors terraces_count_tree(const terraces_missing_data* missing_data,
                                    const char* nwk_string, mpz_t out) noexcept {
	// TODO: implement this using missing_data
	auto sites = to_bitmatrix(missing_data);
	(void)sites;
	(void)nwk_string;
	(void)out;
	return terraces_success;
}

terraces_errors terraces_print_tree(const terraces_missing_data* missing_data,
                                    const char* nwk_string, mpz_t out,
                                    const char* output_filename) noexcept {
	// TODO: implement this using missing_data
	auto sites = to_bitmatrix(missing_data);
	(void)sites;
	(void)nwk_string;
	(void)out;
	(void)output_filename;
	return terraces_success;
}

terraces_errors terraces_check_tree_str(const char* missing_data, const char* nwk_string,
                                        bool* out) noexcept {
	return exec_and_catch([&] { *out = terraces::is_on_terrace(nwk_string, missing_data); });
}

terraces_errors terraces_count_tree_str(const char* missing_data, const char* nwk_string,
                                        mpz_t out) noexcept {
	return exec_and_catch([&] {
		mpz_set(out,
		        terraces::get_terrace_size_bigint(nwk_string, missing_data).get_mpz_t());
	});
}

terraces_errors terraces_print_tree_str(const char* missing_data, const char* nwk_string, mpz_t out,
                                        const char* output_filename) noexcept {
	return exec_and_catch([&] {
		auto output = open_output_file(output_filename);
		mpz_set(out, terraces::print_terrace(nwk_string, missing_data, output).get_mpz_t());
	});
}

} // extern "C"
