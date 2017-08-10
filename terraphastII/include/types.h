#pragma once

#include <cstddef>
#include <assert.h>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

typedef size_t leaf_number;

struct label_mapper {
    std::vector<std::string> labels;
    std::string root_label;
    
    label_mapper() {}
    label_mapper(std::vector<std::string> labels) : labels(labels) {}
    
    inline const std::string& operator[](leaf_number leaf_id) const {
        assert(leaf_id < labels.size());
        return labels.at(leaf_id);
    }
    inline size_t size() const {
        return labels.size();
    }
};

//lca(smaller_left, smaller_right) < lca(bigger_left, bigger_right)
struct constraint {
    leaf_number smaller_left;
    leaf_number smaller_right;
    leaf_number bigger;

    constraint() = default;

    constraint(leaf_number smaller_left, leaf_number smaller_right,
               leaf_number bigger)
            : smaller_left(smaller_left), smaller_right(smaller_right),
              bigger(bigger) {}
    friend std::ostream& operator<<(std::ostream &strm, const constraint& c) {
        return strm << "lca(" << c.smaller_left << "," << c.smaller_right
                    << ") < lca(" << c.bigger << ")";
    }
};

#include "types/UnionFind.h"
#include "types/tree.h"
#include "types/leaf_set.h"          // includes all sub-LeafSet types itself

