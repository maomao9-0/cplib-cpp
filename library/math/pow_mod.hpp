/**
 * @file pow_mod.hpp
 * @brief Binary exponentiation modulo a positive integer.
 *
 * Signed bases are normalized into `[0, mod)` first. This helper is intended
 * for the usual contest case `mod >= 2`.
 */
#pragma once

#include <concepts>

#include "library/internal/concepts.hpp"
#include "library/internal/type_traits.hpp"

namespace maomao90 {
using namespace std;
template <integral T, internal::concepts::broadly_unsigned_integral P>
  requires internal::type_traits::is_64bit_or_less_v<T>
/**
 * @brief Computes `b^p mod mod`.
 * @param b Base value.
 * @param p Non-negative exponent.
 * @param mod Positive modulus, typically at least `2`.
 * @return Normalized residue in `[0, mod)`.
 * Complexity: `O(log p)`.
 */
constexpr T pow_mod(T b, P p, T mod) {
  using U = internal::type_traits::safely_multipliable_t<T>;
  T res = 1;
  b %= mod;
  if (b < 0) {
    b += mod;
  }
  while (p) {
    if (p & 1) {
      res = (U)res * b % mod;
    }
    b = (U)b * b % mod;
    p >>= 1;
  }
  return res;
}
} // namespace maomao90
