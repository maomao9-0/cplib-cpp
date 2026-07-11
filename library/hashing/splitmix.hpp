/**
 * @file splitmix.hpp
 * @brief Stateless 64-bit mixing function.
 */
#pragma once

namespace maomao90 {
/// @brief Applies the SplitMix64 output permutation to a 64-bit word.
constexpr unsigned long long splitmix64(unsigned long long x) {
  // http://xorshift.di.unimi.it/splitmix64.c
  x += 0x9e3779b97f4a7c15;
  x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
  x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
  return x ^ (x >> 31);
}
} // namespace maomao90
