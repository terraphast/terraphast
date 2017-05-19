#include <terraces/constraints.hpp>
#include <terraces/union_find.hpp>

namespace terraces {

std::vector<std::vector<index> > apply_constraints(std::vector<index> species, std::vector<constraint> constraints) {
        union_find leaves = make_set(species.size());
        for (size_t i = 0; i < constraints.size(); i++) {
                merge(leaves, constraints.at(i).shared, constraints.at(i).left);
        }
        return to_set_of_sets(leaves);
}


} // namespace terraces
