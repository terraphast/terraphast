#ifndef TERRACES_BIPARTITIONS_HPP
#define TERRACES_BIPARTITIONS_HPP

#include <cmath>
#include <iosfwd>

#include "trees.hpp"

namespace terraces {

using bipartition = std::tuple<std::vector<std::size_t>, std::vector<std::size_t>>;

class bipartition_iterator {
public:
	bipartition_iterator(const std::vector<std::vector<index>>&);
	bipartition get_bipartition();
	void increase();
	bool is_valid();
	std::ostream& write_binary(std::ostream& stream) const;

private:
	index bip;
	index end;
	const std::vector<std::vector<index>>& sets;
};

std::ostream& operator<<(std::ostream& stream, const bipartition_iterator& it);

} // namespace terraces

#endif
