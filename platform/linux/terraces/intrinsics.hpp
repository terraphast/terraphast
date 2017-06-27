#include <terraces/trees.hpp>

namespace terraces {
namespace bits {

inline index popcount(index word) {
	return (index)__builtin_popcountll(word);
}

inline index bitscan(index word) {
	return (index)__builtin_ctzll(word);
}

inline index rbitscan(index word) {
	return (index)(63 - __builtin_clzll(word));
}

} // namespace bits
} // namespace terraces
