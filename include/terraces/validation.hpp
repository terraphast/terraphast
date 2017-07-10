#ifndef VALIDATION_HPP
#define VALIDATION_HPP

#include <terraces/bitvector.hpp>
#include <terraces/parser.hpp>
#include <terraces/trees.hpp>

namespace terraces {

bool is_isomorphic(const tree_set& fst, const tree_set& snd);
bool is_isomorphic(const tree& fst, const std::vector<index>& fst_leaves, const tree& snd,
                   const std::vector<index>& snd_leaves);

std::vector<bitvector> tree_bipartitions(const tree& t, const std::vector<index>& leaves);

bool bipartition_equal(const std::vector<bitvector>& fst_bip,
                       const std::vector<bitvector>& snd_bip);
bool bipartition_cmp(const std::vector<bitvector>& fst_bip, const std::vector<bitvector>& snd_bip);

} // namespace terraces

#endif // TREE_BIPARTITIONS_HPP
