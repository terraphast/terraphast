
#ifndef TERRACES_NODES_HPP
#define TERRACES_NODES_HPP

#include <array>
#include <cstdint>
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


// This struct will represent both nodes of rooted and
// unrooted trees, depending on context
//
// The specific implementation is still subject to change until
// we know better what exactly we need, so don't rely too much on
// details yet.
struct node {
	// In the rooted case:
	// data[0]: parent
	// data[1]: left child
	// data[2]: right child
	// everything else: undefined behavio ;-)
	std::array<index, 3> data;
};


// The same thing as above applies: The details of this
// type will most likely change:
using tree = std::vector<node>;


// to look up the name associated with an index, simply
// reuse the index to check in a std::vector:
using name_map = std::vector<std::string>;

} // namespace terraces


#endif
