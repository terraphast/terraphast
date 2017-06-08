#include <terraces/bipartitions.hpp>

#include <cassert>

namespace terraces {

bipartition_iterator::bipartition_iterator(index size) : bip{1}, end{(1u << (size - 1))} {
	assert(size < 64);
}

bool bipartition_iterator::get(index i) const { return bip & (1 << (i - 1)); }

void bipartition_iterator::increase() { bip++; }

bool bipartition_iterator::is_valid() const { return bip < end; }

} // namespace terraces
