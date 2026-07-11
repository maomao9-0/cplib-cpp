/**
 * @file primality_test.hpp
 * @brief Deterministic Miller-Rabin primality testing for 64-bit unsigned integers.
 *
 * The provided base sets make `is_prime()` deterministic for unsigned integer
 * types up to 64 bits.
 */
#pragma once

#include <concepts>

#include "library/internal/type_traits.hpp"
#include "library/math/pow_mod.hpp"

namespace maomao90 {
using namespace std;
template <unsigned_integral T>
  requires internal::type_traits::is_64bit_or_less_v<T>
/**
 * @brief Miller-Rabin primality test with caller-supplied bases.
 * @param n Number to test.
 * @param bases Witness array.
 * @param size Number of witnesses.
 * @return `true` if `n` passes all witnesses.
 * Complexity: `O(size log n)`.
 */
constexpr bool miller_rabin(const T &n, const T *bases, const size_t size) {
  using U = internal::type_traits::safely_multipliable_t<T>;
  if (n <= 1) {
    return false;
  }
  if (n == 2) {
    return true;
  }
  if (n % 2 == 0) {
    return false;
  }
  T d = n - 1;
  while (d % 2 == 0)
    d /= 2;
  for (size_t i = 0; i < size; i++) {
    T a = bases[i];
    if (a % n == 0) {
      continue;
    }
    T t = d, y = pow_mod<T, T>(a, t, n);
    while (t != n - 1 && y != 1 && y != n - 1) {
      y = (U)y * y % n;
      t <<= 1;
    }
    if (y != n - 1 && t % 2 == 0) {
      return false;
    }
  }
  return true;
}
template <unsigned_integral T>
  requires internal::type_traits::is_64bit_or_less_v<T>
/// @brief Deterministic primality test for unsigned 32-bit and 64-bit inputs.
constexpr bool is_prime(T n) {
  if constexpr (internal::type_traits::is_32bit_or_less_v<T>) {
    T bases[3] = {2, 7, 61};
    return miller_rabin<T>(n, bases, 3);
  } else {
    T bases[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    return miller_rabin<T>(n, bases, 7);
  }
}
} // namespace maomao90
