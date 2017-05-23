#ifndef TERRACES_BIPARTITIONS_HPP
#define TERRACES_BIPARTITIONS_HPP

#include <cmath>
#include <gmpxx.h>

#include "trees.hpp"

namespace terraces {

using bipartition = std::tuple<std::vector<std::size_t>, std::vector<std::size_t>>;

class bipartition_iterator {
public:
	bipartition_iterator(const std::vector<std::vector<index>>&);
	bipartition get_bipartition();
	void increase();

private:
	mpz_class bip;
	const std::vector<std::vector<index>>& sets;
};

} // namespace terraces

#endif
