#pragma once

#include <cstddef>
typedef size_t leaf_number;
#include <assert.h>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <vector>

//lca(smaller_left, smaller_right) < lca(bigger_left, bigger_right)
struct constraint {
    leaf_number smaller_left;
    leaf_number bigger_left;
    leaf_number smaller_right;
    leaf_number bigger_right;

    constraint() = default;

    constraint(leaf_number smaller_left, leaf_number bigger_left, leaf_number smaller_right, leaf_number bigger_right)
            : smaller_left(smaller_left), bigger_left(bigger_left), smaller_right(smaller_right),
              bigger_right(bigger_right) {}
};

#include "types/UnionFind.h"
#include "types/tree.h"
#include "types/leaf_set.h"          // includes all sub-LeafSet types itself
