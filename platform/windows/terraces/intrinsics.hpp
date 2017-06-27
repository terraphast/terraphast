#include <terraces/trees.hpp>

#pragma intrinsic(_BitScanForward64, __popcnt64)

namespace terraces {
namespace bits {

inline index popcount(index word) {
	return (index)__popcnt64(word);
}

inline index bitscan(index word) {
	unsigned long idx;
	_BitScanForward64(&idx, word);
	return (index)idx;
}

inline index rbitscan(index word) {
	unsigned long idx;
	_BitScanReverse64(&idx, word);
	return (index)idx;
}

} // namespace bits
} // namespace terraces
