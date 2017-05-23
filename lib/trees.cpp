#include <ostream>
#include <terraces/rooting.hpp>


namespace terraces {

std::ostream& operator<<(std::ostream& ss, const tree& t) {
	ss << "TREE [\n";
	for (auto node : t) {
		ss << "\t{" << node.parent() << ", " << node.lchild() << ", " << node.rchild()
		   << "}\n";
	}
	ss << "]\n";
	return ss;
}

}