#pragma once

#include <array>
#include <chrono>
#include <random>

#include "library/internal/concepts.hpp"
#include "library/internal/type_traits.hpp"

namespace maomao90::internal::hashing {
using namespace std;
const int MIN_HASH_BASE = 128;
static mt19937_64
    rng(chrono::high_resolution_clock::now().time_since_epoch().count());
template <typename mint, size_t num_bases>
constexpr array<mint, num_bases> gen_bases() {
  array<mint, num_bases> res;
  for (int i = 0; i < num_bases; i++) {
    res[i] = mint::raw(rng() % (mint::umod() - MIN_HASH_BASE) + MIN_HASH_BASE);
  }
  return res;
}
template <typename mint, size_t num_bases>
constexpr array<mint, num_bases>
gen_inverse(const array<mint, num_bases> &bases) {
  array<mint, num_bases> res;
  for (int i = 0; i < num_bases; i++) {
    res[i] = bases[i].inv();
  }
  return res;
}
template <typename mint, size_t num_bases, size_t CACHE>
constexpr array<array<mint, CACHE>, num_bases>
init_power(const array<mint, num_bases> &bases) {
  array<array<mint, CACHE>, num_bases> res;
  for (int i = 0; i < num_bases; i++) {
    res[i][0] = 1;
    for (int j = 1; j < CACHE; j++) {
      res[i][j] = res[i][j - 1] * bases[i];
    }
  }
  return res;
}

template <typename T> unsigned long long hash_function(const T &x) {
  static unsigned long long r =
      chrono::high_resolution_clock::now().time_since_epoch().count();
  constexpr unsigned long long z1 = 11995408973635179863ull;
  if constexpr (internal::type_traits::is_broadly_integral_v<T>) {
    return ((unsigned long long)x ^ r) * z1;
  } else if constexpr (internal::type_traits::is_pair_v<T>) {
    constexpr unsigned long long z2 = 10150724397891781847ull;
    return hash_function(x.first) + hash_function(x.second) * z2;
  } else if constexpr (internal::concepts::Iterable<T>) {
    constexpr unsigned long long mod = (1ll << 61) - 1;
    constexpr unsigned long long base = 950699498548472943ull;
    unsigned long long m = 0;
    for (auto &i : x) {
      unsigned long long v = hash_function(i);
      unsigned __int128 r = (unsigned __int128)m * base + (v & mod);
      m = (r & mod) + (r >> 61);
      if (m >= mod) {
        m -= mod;
      }
    }
    m ^= m << 24;
    m ^= m >> 31;
    m ^= m << 35;
    return m;
  }
}

template <typename T> struct HashObject {
  constexpr size_t operator()(const T &o) const { return hash_function(o); }
};
} // namespace maomao90::internal::hashing
