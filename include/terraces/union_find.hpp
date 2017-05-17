#ifndef TERRACES_UNION_FIND_HPP
#define TERRACES_UNION_FIND_HPP

#include "trees.hpp"

namespace terraces {

struct union_find {
        std::vector<index> id;
        std::vector<index> size;
};

union_find make_set(std::size_t);

index find(union_find&, index);

void merge(union_find&, index, index);

} // namespace terraces

#endif
