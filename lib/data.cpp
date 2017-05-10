#include<vector>
#include<string>
#include<iostream>

namespace terraces{
namespace data{

/*  A node in our unrooted binary trees. Its members are indices to the
    adjacent nodes held in the same array.
    
    A node is considered the root
    of our tree, if and only if one of its references is set to -1.
    
    A node is considered a leaf, if two of its references are set to -1.
*/
typedef
struct {
    int id;
    int ref1;
    int ref2;
    int ref3;
} node;

} // namespace data

std::string printable_node(data::node n) {
    std::string buffer = "[id="  + std::to_string(n.id) +","+ 
                          "ref1="+ std::to_string(n.ref1) +","+ 
                          "ref2="+ std::to_string(n.ref2) +","+
                          "ref3="+ std::to_string(n.ref3) +"]";
    return buffer;
}

} // namespace terraces