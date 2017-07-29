
#ifndef TERRACES_TREES_HPP
#define TERRACES_TREES_HPP

#include <array>
#include <cassert>
#include <cstdint>
#include <iosfwd>
#include <iostream>
#include <limits>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

namespace terraces {

/** Our index type. */
using index = std::size_t;

/** Index value used to represent a non-index. */
constexpr static const auto none = std::numeric_limits<index>::max();

/**
 * Represents a node of a (rooted or unrooted) tree.
 * Its three neighbor indices are stored in \ref data and
 * in the rooted case can be accessed using \ref lchild, \ref rchild
 * and \ref parent.
 */
struct node {
	node(index parent = none, index left = none, index right = none)
	        : data{{parent, left, right}} {}
	/* In the rooted case:
	 * data[0]: parent
	 * data[1]: left child
	 * data[2]: right child
	 * everything else: undefined behavior ;-) */
	std::array<index, 3> data = {{none, none, none}};

	index parent() const { return data[0]; }
	index& parent() { return data[0]; }

	index lchild() const { return data[1]; }
	index& lchild() { return data[1]; }

	index rchild() const { return data[2]; }
	index& rchild() { return data[2]; }

	bool operator==(const node& o) const {
		return std::equal(data.begin(), data.end(), o.data.begin(), o.data.end());
	}

	bool operator!=(const node& o) const { return !(o == *this); }
};

/** A tree is represented by its nodes. The root of a tree is stored at index 0. */
using tree = std::vector<node>;

/** Stores the name for every node (or leaf) of a tree. */
using name_map = std::vector<std::string>;

/** Stores a permutation of node indices used for enumerating different trees. */
using permutation = std::vector<index>;

/** Helper struct for Newick tree output. */
struct newick_t {
	const tree* t;
	const name_map* names;
};

/** Helper struct for Newick tree output with permuted leaves. */
struct newick_permuted_t {
	const tree* t;
	const name_map* names;
	const permutation* leaf_perm;
};

/** Helper function for Newick tree output. */
inline newick_t as_newick(const tree& t, const name_map& names) { return {&t, &names}; }
/** Helper function for Newick tree output with permuted leaves. */
inline newick_permuted_t as_newick(const tree& t, const name_map& names,
                                   const permutation& leaf_perm) {
	return {&t, &names, &leaf_perm};
}

/**
 * Prints a Newick tree to the given output stream.
 * @see as_newick
 */
std::ostream& operator<<(std::ostream& s, newick_t tree_pair);

/**
 * Prints a Newick tree with permuted leaves to the given output stream.
 * @see as_newick
 */
std::ostream& operator<<(std::ostream& s, newick_permuted_t tree_set);

/** Maps the name of a species to its index in the tree. */
using index_map = std::unordered_map<std::string, index>;

} // namespace terraces

#endif
