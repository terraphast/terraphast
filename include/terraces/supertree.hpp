#ifndef TERRACES_SUPERTREE_HPP
#define TERRACES_SUPERTREE_HPP

#include "constraints.hpp"
#include "fast_set.hpp"
#include "trees.hpp"
#include "union_find.hpp"
#include <gmpxx.h>
#include <string>
#include <tuple>

namespace terraces {

class tree_master {
private:
public:
	mpz_class count_supertree(const tree&, const constraints&, index root);
	mpz_class count_supertree(index count, const constraints&);
};

} // namespace terraces

#endif
