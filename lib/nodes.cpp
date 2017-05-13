
#include <terraces/nodes.hpp>


#include <ostream>

namespace terraces {

std::ostream& operator<<(std::ostream& s, const node& n) {
	return s << '('
		<< n.data[0] << ", "
		<< n.data[1] << ", "
		<< n.data[2] << ')';
}

} // namespace terraces
