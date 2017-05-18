#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <assert.h>

#include <terraces/trees.hpp>

namespace terraces {


bool is_leaf(node n, index i) {
	if (n.lchild() == i && n.rchild() == i) {
		return true;
	} else {
		return false;
	}
}

bool is_root(node n, index i) {
	if (n.parent() == i) {
		return true;
	} else {
		return false;
	}
}

/* Appends the givtn subtree st to the given tree t. t's node with index t_index needs to be a leaf (rchild
   and lchild pointing to itself). Afterwards, t's node at t_index will be the new parent of st's node at
   st_index.
*/
void append_subtree(tree& t, name_map& t_nmap, index_map& t_imap, index t_index, std::string edge_reference, 
					tree& st, name_map& st_nmap, index_map& st_imap, index st_index) {
	// CHECK: A tree node's ID is its index.
	// CHECK: Can one species be found in both subtrees?
	// DONE: Update index_map and name_map with the new subtree species indices.
	// DONE: Update node references in the inserted subtree nodes.
	// DONE: Link t and st!
	// CHECK: Chan/Shall we clear st in the end?
	/* DONE: Set references so that node tt_index an_index poindex to
	         each other via references edge_reference. */
	// TODO: Assert whole tree structure in the end!

	if (!is_leaf(t[t_index], t_index)) {
		//FIXME: Throw exception!
	}
	if (!is_root(st[st_index], st_index)) {
		//FIXME: Throw exception!
	}

	bool connect_via_left_edge = false;
	bool connect_via_right_edge = false;

	if (edge_reference.compare("left") == 0) {
		connect_via_left_edge = true;
	}
	if (edge_reference.compare("right") == 0) {
		connect_via_right_edge = true;
	}
	assert(connect_via_left_edge != connect_via_right_edge);
	
	// INSERT SUBTREE NODES
	index index_difference = t.size();
	index last_t_index = t.size();

	t.reserve(t.size() + st.size());
	t.insert(t.end(), st.begin(), st.end());
	for (index i = last_t_index + 1; i < t.size(); i++) {
		t[i].parent() += index_difference;
		t[i].lchild() += index_difference;
		t[i].rchild() += index_difference;
	}

	// CONNECT TREE AND TO-BE SUBTREE
	index corrected_subtree_root = st_index + index_difference;
	if (edge_reference == "left") {
		t[t_index].lchild() = corrected_subtree_root;
	} else {
		t[t_index].rchild() = corrected_subtree_root;
	}
	t[corrected_subtree_root].parent() = t_index;

	// UPDATE NAME MAP AND INDEX MAP
	for (auto& it: st_imap) {
		it.second += index_difference;
	}
	t_imap.reserve(t_imap.size() + st_imap.size());
	t_imap.insert(/*t_imap.end(),*/st_imap.begin(), st_imap.end());

	t_nmap.reserve(t_nmap.size() + st_nmap.size());
	t_nmap.insert(t_nmap.end(), st_nmap.begin(), st_nmap.end());


	// Only there for testing!
	for (auto& it: st_imap) {
		std::string name = it.first;
		index i = it.second;
		assert(t_nmap[i].compare(name) == 0);
	}
	return;
}


} // namespace terraces
