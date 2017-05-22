
#ifndef TERRACES_TREES_HPP
#define TERRACES_TREES_HPP

#include <array>
#include <cassert>
#include <cstdint>
#include <iosfwd>
#include <limits>
#include <unordered_map>
#include <vector>

namespace terraces {

/// Our index-type.
// please do not use anything else directly,
// as we might be willing to change it's definition
// and it's easier to change one line than twenty.
//
// You may rely on the fact that it's unsigned and
// that sizeof(index) >= sizeof(int)
using index = std::size_t;

constexpr static const auto none = std::numeric_limits<index>::max();

// This struct will represent both nodes of rooted and
// unrooted trees, depending on context
//
// The specific implementation is still subject to change until
// we know better what exactly we need, so don't rely too much on
// details yet.
struct node {
	node(index parent = none, index left = none, index right = none)
	        : data{{parent, left, right}} {}
	// In the rooted case:
	// data[0]: parent
	// data[1]: left child
	// data[2]: right child
	// everything else: undefined behavio ;-)
	std::array<index, 3> data = {{none, none, none}};

	index parent() const { return data[0]; }
	index& parent() { return data[0]; }

	index lchild() const { return data[1]; }
	index& lchild() { return data[1]; }

	index rchild() const { return data[2]; }
	index& rchild() { return data[2]; }
};

std::ostream& operator<<(std::ostream& s, const node& n);

// The same thing as above applies: The details of this
// type will most likely change:
using tree = std::vector<node>;

// to look up the name associated with an index, simply
// reuse the index to check in a std::vector:
using name_map = std::vector<std::string>;

// maps the name of a species to it's index in the tree:
using index_map = std::unordered_map<std::string, index>;

/**
 * Returns whether a node is the root-node
 */
inline bool is_root(const node& n) { return n.parent() == none; }

/**
 * Returns whether a node is a leaf.
 */
inline bool is_leaf(const node& n) {
	assert((n.lchild() == n.rchild()) == (n.lchild() == none));
	return n.lchild() == none;
}

template <typename F>
void foreach_postorder(const tree& t, index root_idx, F cb) {
	// index root_idx = t.size() - 1;
	// assert(is_root(t[root_idx]));

	auto root_node = t[root_idx];
	if (!is_leaf(root_node)) {
		foreach_postorder(t, root_node.lchild(), cb);
		foreach_postorder(t, root_node.rchild(), cb);
	}
	cb(root_idx);
}

} // namespace terraces

#endif
