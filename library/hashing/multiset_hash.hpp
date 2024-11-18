#pragma once

#include <array>

#include "library/internal/hashing.hpp"
#include "library/math/modint.hpp"

namespace maomao90 {
using namespace std;
template <ModInt modint = static_modint<(1ll << 61) - 1>, size_t num_bases = 1,
          size_t CACHE = 1'000'000>
struct MultisetHash {
  using mint = modint;

  constexpr array<typename mint::umod_type, num_bases> get_v() const {
    array<typename mint::umod_type, num_bases> res;
    for (int i = 0; i < num_bases; i++) {
      res[i] = _v[i].val();
    }
    return res;
  }
  // insert `cnt` copies of `a` into multiset
  constexpr MultisetHash insert(long long a, long long cnt = 1) const {
    MultisetHash res = *this;
    for (int i = 0; i < num_bases; i++) {
      res._v[i] += get_pow(i, a) * cnt;
    }
    return res;
  }
  // erase `cnt` copies of `a` into multiset
  constexpr MultisetHash erase(long long a, long long cnt = 1) const {
    MultisetHash res = *this;
    for (int i = 0; i < num_bases; i++) {
      res._v[i] -= get_pow(i, a) * cnt;
    }
    return res;
  }
  // increases all values in multiset by `delta`
  // (`delta` can be negative)
  constexpr MultisetHash offset(long long delta) const {
    MultisetHash res = *this;
    for (int i = 0; i < num_bases; i++) {
      res._v[i] *= get_pow(i, delta);
    }
    return res;
  }
  constexpr MultisetHash operator+(const MultisetHash &o) const {
    MultisetHash res = *this;
    return res += o;
  }
  constexpr MultisetHash &operator+=(const MultisetHash &o) {
    for (int i = 0; i < num_bases; i++) {
      _v[i] += o._v[i];
    }
    return *this;
  }
  constexpr MultisetHash operator-(const MultisetHash &o) const {
    MultisetHash res = *this;
    return res -= o;
  }
  constexpr MultisetHash &operator-=(const MultisetHash &o) {
    for (int i = 0; i < num_bases; i++) {
      _v[i] -= o._v[i];
    }
    return *this;
  }
  constexpr bool operator==(const MultisetHash &o) const {
    for (int i = 0; i < num_bases; i++) {
      if (_v[i] != o._v[i]) {
        return false;
      }
    }
    return true;
  }
  constexpr bool operator!=(const MultisetHash &o) const {
    return !(*this == o);
  }

private:
  array<mint, num_bases> _v;
  inline static const array<mint, num_bases>
      bases = internal::hashing::gen_bases<mint, num_bases>(),
      inv_bases = internal::hashing::gen_inverse<mint, num_bases>(bases);
  inline static const array<array<mint, CACHE>, num_bases>
      bases_pow = internal::hashing::init_power<mint, num_bases, CACHE>(bases),
      inv_bases_pow =
          internal::hashing::init_power<mint, num_bases, CACHE>(inv_bases);
  constexpr mint get_pow(int i, long long p) const {
    assert(i < num_bases);
    if (abs(p) < CACHE) {
      return p >= 0 ? bases_pow[i][p] : inv_bases_pow[i][-p];
    } else {
      return p >= 0 ? bases[i].pow(p) : inv_bases[i].pow(-p);
    }
  }
};
} // namespace maomao90
