#ifndef BITS_HPP
#define BITS_HPP

#include <terraces/intrinsics.hpp>

namespace terraces {
namespace bits {

static_assert(std::numeric_limits<index>::radix == 2, "Our integers must be of base 2");
constexpr index word_bits = std::numeric_limits<index>::digits;

} // namespace bits
} // namespace terraces

#endif // BITS_HPP
