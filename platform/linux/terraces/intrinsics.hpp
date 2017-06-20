#include <terraces/trees.hpp>

namespace terraces {
namespace bits {

inline index popcount(index word) {
	return (index)__builtin_popcountll(word);
}

inline index bitscan(index word) {
	return (index)__builtin_ctzll(word);
}

} // namespace bits
} // namespace terraces
