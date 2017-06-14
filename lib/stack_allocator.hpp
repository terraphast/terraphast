#ifndef TERRACES_UTILS_HPP
#define TERRACES_UTILS_HPP

#include <cstdint>
#include <memory>
#include <new>
#include <utility>
#include <vector>

namespace terraces {
namespace utils {


class free_list {
public:
	free_list(std::size_t initial_capacity = 0) {
		m_list.reserve(initial_capacity);
	}

	void push(std::unique_ptr<char[]> ptr) { m_list.push_back(std::move(ptr)); }

	std::unique_ptr<char[]> pop() {
		if (not m_list.empty()) {
			auto ret = std::move(m_list.back());
			m_list.pop_back();
			return ret;
		} else {
			return nullptr;
		}
	}

private:
	std::vector<std::unique_ptr<char[]>> m_list;
};

template <typename T>
class free_list_allocator {
public:
	free_list_allocator(free_list& fl, std::size_t expected_size)
	        : m_fl{&fl}, m_expected_size{expected_size} {}

	using value_type = T;

	T* allocate(std::size_t n) {
		if (n != m_expected_size) {
			return system_allocate(n);
		}
		auto ret = m_fl->pop();
		if (ret == nullptr) {
			return system_allocate(n);
		}
		return reinterpret_cast<T*>(ret.release());
	}

	void deallocate(T* ptr, std::size_t n) {
		if (n != m_expected_size) {
			return ::operator delete(ptr);
		}
		auto p = std::unique_ptr<char[]>{reinterpret_cast<char*>(ptr)};
		m_fl->push(std::move(p));
	}

private:
	T* system_allocate(std::size_t n) {
		auto size = n * sizeof(T);
		auto ptr = ::operator new(size);
		return reinterpret_cast<T*>(ptr);
	}

	free_list* m_fl;
	std::size_t m_expected_size;
};


} // namespace utils
} // namespace terraces

#endif
