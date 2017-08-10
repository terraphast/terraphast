#ifndef ADVANCED_HPP
#define ADVANCED_HPP

#include "constraints.hpp"
#include "parser.hpp"
#include "rooting.hpp"

#include <gmpxx.h>

namespace terraces {

/**
 * Contains the necessary data to describe all possible supertrees equivalent to a certain tree.
 */
struct supertree_data {
	/** The lca constraints describing the structure of the possible supertrees */
	terraces::constraints constraints;
	/** The number of leaves of the supertree. */
	index num_leaves;
	/** The index of the 'root leaf' in leaf-based numbering.
	 * It will be placed as a leaf below the root of all supertrees.
	 * In terms of phylogenetic trees, this means the index of a comprehensive taxon. */
	index root;
};

/**
 * Computes the necessary data to enumerate the supertrees of the given tree and missing data
 * matrix.
 * \param tree The phylogenetic tree. It must be rooted at a comprehensive taxon!
 * \param data The missing data matrix. It must only contain data for the leaves.
 * \param names The name map containing names for all nodes of the tree.
 *              All inner node names will be removed after this call.
 * \param root The node index of the 'root leaf' in the input tree.
 *             It must refer to a comprehensive taxon, otherwise the constraints will be
 * inconsistent.
 * \returns \ref supertree_data object describing all possible supertrees equivalent to the input
 * tree.
 */
supertree_data prepare_constraints(const tree& tree, const bitmatrix& data, name_map& names,
                                   index root);

/**
 * Checks if a phylogenetic tree lies on a phylogenetic terrace.
 * \param data The constraints extracted from the tree and missing data matrix describing all
 * possible supertrees.
 * \return True if and only if the tree lies on a phylogenetic terrace,
 *         i.e. there are at least 2 different trees (called supertrees)
 *         that are equivalent with respect to the missing data matrix.
 */
bool check_terrace(const supertree_data& data);
/**
 * Counts all trees on a terrace around a phylogenetic tree.
 * Note that this number might not be representable as a 64 bit integer, and might thus be clamped.
 * \param data The constraints extracted from the tree and missing data matrix describing all
 * possible supertrees.
 * \return The number of trees on the phylogenetic terrace containing the input tree.
 *         Note that if this result is UINT64_MAX = 2^64 - 1, the computations resulted in an
 * overflow,
 *         i.e. the result is only a lower bound on the number of trees on this terrace.
 */
uint64_t count_terrace(const supertree_data& data);
/**
 * Counts all trees on a terrace around a phylogenetic tree.
 * \param data The constraints extracted from the tree and missing data matrix describing all
 * possible supertrees.
 * \return The number of trees on the phylogenetic terrace containing the input tree.
 */
mpz_class count_terrace_bigint(const supertree_data& data);
/**
 * Enumerates all trees on a terrace around a phylogenetic tree.
 * The trees will be printed in a compressed <b>multitree format</b>,
 * which is an extension of the normal Newick format:
 * In place of a subtree <i>T</i>, there may be two additional types of nodes:
 * <ul>
 * <li>An <b>alternative list</b> of all possible sub(-multi-)trees that appear on the terrace:<br>
 *     <i>T1</i>|<i>T2</i>|<i>T3</i>|...</li>
 * <li>An <b>unconstrained subtree</b> where all possible subtrees with the given leaves are part of
 * the terrace:<br>
 * {<i>leaf1</i>,<i>leaf2</i>,<i>leaf3</i>,...}
 * </li>
 * </ul>
 * \param data The constraints extracted from the tree and missing data matrix describing all
 * possible supertrees.
 * \param names The name map containing only leaf names. It will be used to output the multitree.
 * \param output The output stream into which the multitree will be written.
 * \return The number of trees on the phylogenetic terrace containing the input tree.
 */
mpz_class print_terrace(const supertree_data& data, const name_map& names, std::ostream& output);

} // namespace terraces

#endif // ADVANCED_HPP
