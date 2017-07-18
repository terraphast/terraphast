#ifndef VALIDATION_HPP
#define VALIDATION_HPP

#include <terraces/parser.hpp>
#include <terraces/trees.hpp>

#include "bitvector.hpp"

namespace terraces {

bool is_isomorphic(const tree_set& fst, const tree_set& snd);
bool is_isomorphic(const tree& fst, const std::vector<index>& fst_leaves, const tree& snd,
                   const std::vector<index>& snd_leaves);

std::vector<bitvector> tree_bipartitions(const tree& t, const std::vector<index>& leaves,
                                         utils::stack_allocator<index> alloc);

} // namespace terraces

#endif // TREE_BIPARTITIONS_HPP
