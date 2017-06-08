#ifndef TERRACES_BIPARTITIONS_HPP
#define TERRACES_BIPARTITIONS_HPP

#include <cmath>
#include <iosfwd>

#include "trees.hpp"

namespace terraces {

class bipartition_iterator {
public:
	bipartition_iterator(index);
	void increase();
	bool get(index i) const;
	bool is_valid() const;
	std::ostream& write_binary(std::ostream& stream) const;

private:
	index bip;
	index end;
};

std::ostream& operator<<(std::ostream& stream, const bipartition_iterator& it);

} // namespace terraces

#endif
