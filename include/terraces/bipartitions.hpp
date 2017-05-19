#ifndef TERRACES_BIPARTITIONS_HPP
#define TERRACES_BIPARTITIONS_HPP

#include <cmath>

#include "trees.hpp"

namespace terraces {

using bipartition = std::tuple<std::vector<std::size_t>, std::vector<std::size_t>>;

std::vector<bipartition> sets_to_bipartitions(std::vector<std::vector<std::size_t>>&);

} // namespace terraces

#endif
