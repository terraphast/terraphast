#ifndef TREES_IMPL_HPP
#define TREES_IMPL_HPP

#include <terraces/trees.hpp>

namespace terraces {

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

template <typename Result>
Result count_unrooted_trees(index num_leaves) {
	Result result = 1;
	for (index i = 3; i <= num_leaves + 1; i++) {
		result *= (2 * i - 5);
	}
	return result;
}

template <typename F1, typename F2, typename F3, typename F4>
void tree_traversal(const tree& t, F1 pre_cb, F2 post_cb, F3 sibling_cb, F4 leaf_cb) {
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
			leaf_cb(current.first);
		} else {
			switch (current.second) {
			// descend into left tree
			case visited::none:
				pre_cb(cur_idx);
				stack.push(std::make_pair(cur_idx, visited::left));
				stack.push(std::make_pair(cur_node.lchild(), visited::none));
				break;
			// descend into right tree
			case visited::left:
				sibling_cb(cur_idx);
				stack.push(std::make_pair(cur_idx, visited::both));
				stack.push(std::make_pair(cur_node.rchild(), visited::none));
				break;
			// move up again
			case visited::both:
				post_cb(cur_idx);
				break;
			}
		}
	}
}

/**
 * Traverses a tree in post-order while calling a given callback on every node.
 */
template <typename F>
void foreach_postorder(const tree& t, F cb) {
	auto nop = [](index) {};
	tree_traversal(t, nop, cb, nop, cb);
}

template <typename F>
void foreach_preorder(const tree& t, F cb) {
	auto nop = [](index) {};
	tree_traversal(t, cb, nop, nop, cb);
}

// For testing purposes
std::vector<index> preorder(const tree& t);
std::vector<index> postorder(const tree& t);

// output trees
void print_tree_dot_unrooted(const tree& t, const name_map& names, std::ostream& output,
                             std::string name_prefix = "");

} // namespace terraces

#endif // TREES_IMPL_HPP
