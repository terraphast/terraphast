#include <terraces/bipartitions.hpp>

#include <cassert>
#include <ostream>

namespace terraces {

bipartition_iterator::bipartition_iterator(index size) : bip{1}, end{(1u << (size - 1))} {
	assert(size < 64);
}

bool bipartition_iterator::get(index i) const { return bip & (1 << (i - 1)); }

void bipartition_iterator::increase() { bip++; }

bool bipartition_iterator::is_valid() const { return bip < end; }

std::ostream& bipartition_iterator::write_binary(std::ostream& stream) const {
	for (index b = end, c = bip; b; b >>= 1, c >>= 1) {
		stream << (index)(c & 1);
	}
	stream << '/';
	for (index b = end; b; b >>= 1) {
		stream << (index)(b & 1);
	}
	return stream;
}

std::ostream& operator<<(std::ostream& os, const bipartition_iterator& it) {
	return it.write_binary(os);
}

} // namespace terraces
