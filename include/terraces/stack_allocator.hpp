#ifndef TERRACES_STACK_ALLOCATOR_HPP
#define TERRACES_STACK_ALLOCATOR_HPP

#include <cstdint>
#include <iostream>
#include <memory>
#include <new>
#include <utility>
#include <vector>

namespace terraces {
namespace utils {

class array_deleter {
public:
	void operator()(char* ptr) { ::operator delete[](static_cast<void*>(ptr)); }
};

using char_buffer = std::unique_ptr<char[], array_deleter>;

class free_list {
public:
	free_list(std::size_t initial_capacity = 0) { m_list.reserve(initial_capacity); }

	void push(char_buffer ptr) { m_list.push_back(std::move(ptr)); }

	char_buffer pop() {
		if (not m_list.empty()) {
			auto ret = std::move(m_list.back());
			m_list.pop_back();
			return ret;
		}
		return nullptr;
	}

private:
	std::vector<char_buffer> m_list;
};

template <typename T>
class stack_allocator {
	template <typename U>
	friend class stack_allocator;

public:
	stack_allocator(free_list& fl, std::size_t n) : m_fl{&fl}, m_expected_size{n * sizeof(T)} {}
	template <typename U>
	stack_allocator(const stack_allocator<U>& other)
	        : m_fl{other.m_fl}, m_expected_size{other.m_expected_size} {}

	using value_type = T;

	T* allocate(std::size_t n) {
		if (n * sizeof(T) > m_expected_size) {
			throw std::logic_error{"Oversized allocation from stack-allocator"};
		}
		auto ret = m_fl->pop();
		if (ret == nullptr) {
			return system_allocate();
		}
		return reinterpret_cast<T*>(ret.release());
	}

	void deallocate(T* ptr, std::size_t) {
		auto p = char_buffer{reinterpret_cast<char*>(ptr)};
		m_fl->push(std::move(p));
	}

	friend bool operator==(const stack_allocator& lhs, const stack_allocator& rhs) {
		return lhs.m_expected_size == rhs.m_expected_size;
	}

private:
	T* system_allocate() {
		auto ret = ::operator new[](m_expected_size);
		return reinterpret_cast<T*>(ret);
	}

	free_list* m_fl;
	std::size_t m_expected_size;
};

template <typename T>
bool operator==(const stack_allocator<T>& lhs, const stack_allocator<T>& rhs) {
	return !(lhs == rhs);
}

} // namespace utils
} // namespace terraces

#endif