
#ifndef TERRACES_TREES_HPP
#define TERRACES_TREES_HPP

#include <array>
#include <cassert>
#include <cstdint>
#include <iosfwd>
#include <limits>
#include <stack>
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

	bool operator==(const node& o) const {
		return std::equal(data.begin(), data.end(), o.data.begin(), o.data.end());
	}

	bool operator!=(const node& o) const { return !(o == *this); }
};

std::ostream& operator<<(std::ostream& s, const node& n);

// The same thing as above applies: The details of this
// type will most likely change:
using tree = std::vector<node>;

std::ostream& operator<<(std::ostream& ss, const tree& t);

class tree_cursor {
public:
	tree_cursor(tree& t, index n) : m_tree{&t}, m_node{n} {}

	node& get() const { return m_tree->at(m_node); }
	node& operator*() const { return get(); }

	index get_index() const { return m_node; }

	bool has_parent() const { return get().parent() != none; }
	void go_parent() { m_node = get().parent(); }

	bool has_lchild() const { return get().lchild() != none; }
	void go_lchild() { m_node = get().lchild(); }

	bool has_rchild() const { return get().rchild() != none; }
	void go_rchild() { m_node = get().rchild(); }

private:
	tree* m_tree;
	index m_node;
};

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

/**
 * Returns whether a tree is a valid rooted tree.
 * This means it is valid and the last node is its root.
 */
bool is_rooted_tree(const tree& t);

/**
 * Returns whether a tree is valid.
 * This means it has no cycles and only nodes of degree 1 and 3 (and the root)
 */
bool is_valid_tree(const tree& t);

/**
 * Traverses a tree in post-order while calling a given callback on every node.
 */
template <typename F>
void foreach_postorder(const tree& t, F cb) {
	index root_idx = 0;
	assert(is_root(t[root_idx]));

	enum class visited { none, left, both };

	std::stack<std::pair<index, visited>> stack;
	stack.push(std::make_pair(root_idx, visited::none));
	while (!stack.empty()) {
		auto current = stack.top();
		auto cur_idx = current.first;
		stack.pop();
		auto cur_node = t[cur_idx];
		if (is_leaf(cur_node)) {
			cb(current.first);
		} else {
			switch (current.second) {
			// descend into left tree
			case visited::none:
				stack.push(std::make_pair(cur_idx, visited::left));
				stack.push(std::make_pair(cur_node.lchild(), visited::none));
				break;
			// descend into right tree
			case visited::left:
				stack.push(std::make_pair(cur_idx, visited::both));
				stack.push(std::make_pair(cur_node.rchild(), visited::none));
				break;
			// move up again
			case visited::both:
				cb(cur_idx);
				break;
			}
		}
	}
}

template <typename F>
void foreach_preorder(const tree& t, F cb) {
	index root_idx = 0;
	assert(is_root(t[root_idx]));

	std::stack<index> stack;
	stack.push(root_idx);
	while (!stack.empty()) {
		auto node = t[stack.top()];
		cb(stack.top());
		stack.pop();
		// recursive descent
		if (!is_leaf(node)) {
			stack.push(node.rchild());
			stack.push(node.lchild());
		}
	}
}

// For testing purposes
std::vector<index> preorder(const tree& t);
std::vector<index> postorder(const tree& t);

} // namespace terraces

#endif
