#ifndef ADVANCED_HPP
#define ADVANCED_HPP

#include "constraints.hpp"
#include "parser.hpp"
#include "rooting.hpp"

#include <gmpxx.h>

namespace terraces {

struct supertree_data {
	terraces::constraints constraints;
	index num_leaves;
	index root;
};

supertree_data prepare_constraints(const tree& tree, const bitmatrix& data, name_map& names,
                                   index root);

bool check_terrace(const supertree_data& data);
uint64_t count_terrace(const supertree_data& data);
mpz_class count_terrace_bigint(const supertree_data& data);
mpz_class print_terrace(const supertree_data& data, const name_map& names, std::ostream& output);

} // namespace terraces

#endif // ADVANCED_HPP
