
#ifndef TERRACES_ERRORS_HPP
#define TERRACES_ERRORS_HPP

#include <stdexcept>

namespace terraces {

/** This error is thrown if the input to a function is malformed. */
class bad_input_error : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

/** This error is thrown if there is no comprehensive taxon in a dataset. */
class no_usable_root_error : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

/** This error is thrown if a file couldn't be opened. */
class file_open_error : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

/**
 * This error is thrown if the terrace search ran into a huge terrace.
 * More specifically, this means that on a recursive subcalls,
 * more than 2^64 possible bipartitions would have to be enumerated.
 * This would take quite a lot of time.
 */
class tree_count_overflow_error : public std::overflow_error {
	using std::overflow_error::overflow_error;
};

} // namespace terraces

#endif
