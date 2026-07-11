/**
 * @file extended_gcd.hpp
 * @brief Extended Euclid helpers for non-negative signed integers.
 */
#pragma once

#include <cassert>
#include <concepts>
#include <utility>

#include "library/internal/type_traits.hpp"

namespace maomao90 {
using namespace std;
template <signed_integral T>
  requires internal::type_traits::is_64bit_or_less_v<T>
/**
 * @brief Computes the modular inverse of `x` modulo `mod`.
 * @param x Residue in `[0, mod)`.
 * @param mod Positive modulus.
 * @return Unique inverse in `[0, mod)`.
 * @warning Requires `gcd(x, mod) == 1`.
 */
constexpr T inv_gcd(T x, T mod) {
  using U = internal::type_traits::safely_multipliable_t<T>;
  assert(x >= 0 && mod > 0);
  T a = mod, b = x;
  U va = 0, vb = 1;
  while (b) {
    T k = a / b;
    a -= k * b;
    va -= k * vb;
    swap(a, b);
    swap(va, vb);
  }
  assert(a == 1); // gcd should be equal to 1
  T res = va % mod;
  if (res < 0) {
    res += mod;
  }
  return res;
}

template <signed_integral T>
  requires internal::type_traits::is_64bit_or_less_v<T>
/**
 * @brief Extended Euclidean algorithm.
 *
 * Finds `x` and `y` such that `a * x + b * y == gcd(a, b)`.
 *
 * @param a Non-negative integer.
 * @param b Non-negative integer.
 * @param x Output coefficient for `a`.
 * @param y Output coefficient for `b`.
 * @return `gcd(a, b)`.
 */
constexpr T extended_gcd(T a, T b, T &x, T &y) {
  using U = internal::type_traits::safely_multipliable_t<T>;
  assert(a >= 0 && b >= 0);
  T cur_a = a, cur_b = b;
  // x_a * a + y_a * b = cur_a
  U x_a = 1, y_a = 0;
  // x_b * a + y_b * b = cur_b
  U x_b = 0, y_b = 1;
  while (cur_b) {
    T k = cur_a / cur_b;
    cur_a -= k * cur_b;
    x_a -= x_b * k;
    y_a -= y_b * k;
    swap(cur_a, cur_b);
    swap(x_a, x_b);
    swap(y_a, y_b);
  }
  x = x_a;
  y = y_a;
  return cur_a;
}
} // namespace maomao90
