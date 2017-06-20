
#ifndef TERRACES_UTILS_HPP
#define TERRACES_UTILS_HPP

#include <cctype>
#include <iterator>
#include <utility>

namespace terraces {
namespace utils {

template <typename Iterator>
Iterator skip_ws(Iterator it, Iterator last) {
	while (it != last and std::isspace(*it)) {
		++it;
	}
	return it;
}

template <typename Iterator>
Iterator reverse_skip_ws(Iterator first, Iterator last) {
	while (first != last) {
		if (std::isspace(*std::prev(last))) {
			--last;
		} else {
			break;
		}
	}
	return last;
}

template <typename Exception, typename... Args>
void ensure(bool b, Args&&... args) {
	if (not b) {
		throw Exception{std::forward<Args>(args)...};
	}
}

template <typename T>
struct comma_separated_output {
	const T& data;
};

template <typename T>
std::ostream& operator<<(std::ostream& stream, comma_separated_output<T> output) {
	bool first = true;
	for (auto el : output.data) {
		if (not first) {
			stream << ",";
		}
		stream << el;
		first = false;
	}
	return stream;
}

template <typename T1, typename T2>
struct comma_separated_mapped_output {
	const T1& data;
	const T2& names;
};

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& stream, comma_separated_mapped_output<T1, T2> output) {
	bool first = true;
	for (auto el : output.data) {
		if (not first) {
			stream << ",";
		}
		stream << output.names[el];
		first = false;
	}
	return stream;
}

template <typename T1, typename T2, typename T3>
struct comma_separated_mapped_subset_output {
	const T1& subset;
	const T2& data;
	const T3& names;
};

template <typename T2, typename T3>
struct named_output {
	const typename T2::value_type entry;
	const T3& names;
};

template <typename T1, typename T2, typename T3>
std::ostream& operator<<(std::ostream& stream,
                         comma_separated_mapped_subset_output<T1, T2, T3> output) {
	bool first = true;
	for (auto el : output.subset) {
		if (not first) {
			stream << ",";
		}
		stream << named_output<T2, T3>{output.data[el], output.names};
		first = false;
	}
	return stream;
}

// helpers because c++14 can't deduce class template types
template <typename T>
comma_separated_output<T> as_comma_separated_output(const T& data) {
	return {data};
}

template <typename T1, typename T2>
comma_separated_mapped_output<T1, T2> as_comma_separated_output(const T1& data, const T2& names) {
	return {data, names};
}

template <typename T1, typename T2, typename T3>
comma_separated_mapped_subset_output<T1, T2, T3>
as_comma_separated_output(const T1& subset, const T2& data, const T3& names) {
	return {subset, data, names};
}

} // namespace utils
} // namespace terraces

#endif
