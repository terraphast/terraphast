#ifndef STACK_ALLOCATOR_HPP
#define STACK_ALLOCATOR_HPP

#include <cassert>
#include <memory>
#include <vector>

namespace terraces {

class memory_pool {
private:
	std::vector<char*> m_blocks;

public:
	memory_pool() : m_blocks{} {}
	~memory_pool() {
		for (auto block : m_blocks) {
			delete[] block;
		}
	}

	bool empty() { return m_blocks.empty(); }

	char* pop() {
		assert(!empty());
		char* ptr = m_blocks.back();
		m_blocks.pop_back();
		return ptr;
	}

	void push(char* block) { m_blocks.emplace_back(block); }
};

template <typename T>
class stack_allocator {
public:
	using value_type = T;
	using size_type = std::size_t;
	using propagate_on_container_copy_assignment = std::true_type;
	using propagate_on_container_move_assignment = std::true_type;
	using propagate_on_container_swap = std::true_type;

private:
	std::shared_ptr<memory_pool> m_pool;
	size_type m_block_size;

public:
	stack_allocator(size_type block_size, size_type prealloc)
	        : m_pool{std::make_shared<memory_pool>()}, m_block_size{block_size} {
		for (size_type i = 0; i < prealloc; ++i) {
			m_pool->push((char*)(new T[m_block_size]));
		}
	}

	T* allocate(size_type size) {
		(void)size;
		assert(size <= m_block_size);
		if (m_pool->empty()) {
			return new T[m_block_size];
		}
		return (T*)m_pool->pop();
	}

	void deallocate(T* ptr, size_type size) {
		(void)size;
		assert(size <= m_block_size);
		m_pool->push((char*)ptr);
	}

	size_type max_size() const { return m_block_size; }
};

} // namespace terraces

#endif // STACK_ALLOCATOR_HPP
