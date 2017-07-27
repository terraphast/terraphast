
#include <terraces/terraces.h>

// The implementation will of the C-wrapper will of course be done in C++:

#include <cstdlib>
#include <fstream>

#include <terraces/bitmatrix.hpp>
#include <terraces/errors.hpp>
#include <terraces/simple.hpp>
#include <terraces/advanced.hpp>

namespace {

template <typename Function>
terraces_errors exec_and_catch(Function f) {
	// TODO: deal with all exceptions:
	try {
		f();
		return terraces_success;
	} catch (std::bad_alloc& e) {
		return terraces_out_of_memory_error;
	} catch (terraces::bad_input_error& e) {
		return terraces_invalid_input_error;
	} catch (terraces::no_usable_root_error& e) {
		return terraces_no_usable_root_error;
	} catch (terraces::file_open_error& e) {
		return terraces_file_open_error;
	} catch (terraces::tree_count_overflow_error& e) {
		return terraces_tree_count_overflow_error;
	} catch (std::exception& e) {
		return terraces_unknown_error;
	} catch (...) {
		// Something would have to go terribly wrong to end here, so let's just abort.
		std::abort();
	}
}

std::ofstream open_output_file(const char* filename) {
	auto ret = std::ofstream{filename};
	if (not ret.is_open()) {
		throw terraces::file_open_error{""};
	}
	return ret;
}

std::pair<terraces::bitmatrix, terraces::index>
to_bitmatrix(const terraces_missing_data* missing_data_ptr) {
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
			const auto known =
			        missing_data.matrix[row * missing_data.num_partitions + col];
			all_known &= known;
			any_known |= known;
			bm.set(row, col, known);
		}
		if (all_known and root == terraces::none) {
			root = row;
		} else if (not any_known) {
			throw terraces::bad_input_error{""};
		}
	}
	return {bm, root};
}

} // anonymous namespace

extern "C" {

terraces_errors terraces_check_tree(const terraces_missing_data* missing_data,
                                    const char* nwk_string, bool* out) noexcept {
	return exec_and_catch([&]{
		auto sites = to_bitmatrix(missing_data);
		auto tree = terraces::parse_nwk(nwk_string);
		terraces::reroot_inplace(tree.tree, sites.second);
		*out = check_terrace(prepare_constraints(tree.tree, sites.first, tree.names, sites.second));
	});
}

terraces_errors terraces_count_tree(const terraces_missing_data* missing_data,
                                    const char* nwk_string, mpz_t out) noexcept {
	return exec_and_catch([&]{
		auto sites = to_bitmatrix(missing_data);
		auto tree = terraces::parse_nwk(nwk_string);
		terraces::reroot_inplace(tree.tree, sites.second);
		mpz_set(out, terraces::count_terrace_bigint(prepare_constraints(tree.tree, sites.first, tree.names, sites.second)).get_mpz_t());
	});
}

terraces_errors terraces_print_tree(const terraces_missing_data* missing_data,
                                    const char* nwk_string, mpz_t out,
                                    const char* output_filename) noexcept {
	return exec_and_catch([&]{
		auto sites = to_bitmatrix(missing_data);
		auto tree = terraces::parse_nwk(nwk_string);
		auto output = open_output_file(output_filename);
		terraces::reroot_inplace(tree.tree, sites.second);
		mpz_set(out, terraces::print_terrace(prepare_constraints(tree.tree, sites.first, tree.names, sites.second), tree.names, output).get_mpz_t());
	});
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
