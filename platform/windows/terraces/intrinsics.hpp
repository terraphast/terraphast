#include <terraces/trees.hpp>

#pragma intrinsic(_BitScanForward64, __popcnt64)

namespace terraces {
namespace bits {

index popcount(index word) {
	return (index)__popcnt64(word);
}

index bitscan(index word) {
	unsigned long idx;
	_BitScanForward64(&idx, word);
	return (index)idx;
}

} // namespace bits
} // namespace terraces
