#ifndef TERRACES_BIPARTITIONS_HPP
#define TERRACES_BIPARTITIONS_HPP

#include <cmath>

#include "trees.hpp"

namespace terraces {

class bipartition_iterator {
public:
	bipartition_iterator(index);
	void increase();
	bool get(index i) const;
	bool is_valid() const;

private:
	index bip;
	index end;
};

} // namespace terraces

#endif
