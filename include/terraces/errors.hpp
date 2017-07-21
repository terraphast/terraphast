
#ifndef TERRACES_ERRORS_HPP
#define TERRACES_ERRORS_HPP

#include <stdexcept>

namespace terraces {

class bad_input_error : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

class no_usable_root_error : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

class file_open_error : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

class tree_count_overflow_error : public std::overflow_error {
	using std::overflow_error::overflow_error;
};

} // namespace terraces

#endif
