#ifndef TERRACES_SUPERTREE_HPP
#define TERRACES_SUPERTREE_HPP

#include "constraints.hpp"
#include "fast_set.hpp"
#include "trees.hpp"
#include "union_find.hpp"
#include <string>

#include <tuple>

namespace terraces {

class tree_master {
private:
public:
	index count_supertree(const tree&, const constraints&, index root);
	index count_supertree(index count, const constraints&);
};

} // namespace terraces

#endif
