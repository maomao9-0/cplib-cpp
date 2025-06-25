#pragma once

#include <cassert>
#include <concepts>
#include <iostream>
#include <type_traits>

#include "library/internal/concepts.hpp"
#include "library/internal/math.hpp"
#include "library/internal/type_traits.hpp"
#include "library/math/extended_gcd.hpp"
#include "library/math/primality_test.hpp"

namespace maomao90 {
namespace internal::modint {
struct modint_base {};
struct static_modint_base : modint_base {};
} // namespace internal::modint
using namespace std;
template <typename T>
concept ModInt = is_base_of_v<internal::modint::modint_base, T>;
template <typename T>
concept StaticModInt = is_base_of_v<internal::modint::static_modint_base, T>;

template <auto mod = 998244353, enable_if_t<(mod >= 1), nullptr_t> = nullptr>
  requires signed_integral<decltype(mod)> &&
           internal::type_traits::is_64bit_or_less_v<decltype(mod)>
struct static_modint : internal::modint::static_modint_base {
private:
  using M = decltype(mod);
  using UM = make_unsigned_t<M>;
  using BM = internal::type_traits::safely_multipliable_t<M>;

public:
  static constexpr bool is_prime_mod = is_prime((UM)mod);
  using mod_type = M;
  using umod_type = UM;
  static constexpr M imod() { return mod; }
  static constexpr UM umod() { return mod; }

  static constexpr static_modint raw(M a) {
    static_modint res;
    res.v = a;
    return res;
  }

  constexpr static_modint() : v(0) {}

  template <internal::concepts::broadly_signed_integral T>
  constexpr static_modint(T a) {
    M x = a % imod();
    if (x < 0) {
      x += imod();
    }
    v = x;
  }

  template <internal::concepts::broadly_unsigned_integral T>
  constexpr static_modint(T a) : v(a % umod()) {}

  constexpr UM val() const { return v; }

  constexpr static_modint operator+() const { return *this; }
  constexpr static_modint operator-() const {
    return raw(v == 0 ? 0 : imod() - v);
  }

  constexpr static_modint &operator++() {
    ++v;
    if (v == umod()) {
      v = 0;
    }
    return *this;
  }
  constexpr static_modint &operator--() {
    if (v == 0) {
      v = umod();
    }
    --v;
    return *this;
  }
  constexpr static_modint operator++(int) {
    static_modint x = *this;
    ++*this;
    return x;
  }
  constexpr static_modint operator--(int) {
    static_modint x = *this;
    --*this;
    return x;
  }

  constexpr static_modint &operator+=(const static_modint &o) {
    v += o.v;
    if (v >= umod()) {
      v -= umod();
    }
    return *this;
  }
  constexpr static_modint &operator-=(const static_modint &o) {
    if (v < o.v) {
      v += umod();
    }
    v -= o.v;
    return *this;
  }
  constexpr static_modint &operator*=(const static_modint &o) {
    v = (BM)v * o.v % umod();
    return *this;
  }
  constexpr static_modint &operator/=(const static_modint &o) {
    return *this *= o.inv();
  }

  constexpr static_modint pow(long long p) const {
    assert(p >= 0);
    static_modint res = 1, b = *this;
    while (p) {
      if (p & 1) {
        res *= b;
      }
      b *= b;
      p >>= 1;
    }
    return res;
  }
  constexpr static_modint inv() const {
    if constexpr (is_prime_mod) {
      return pow(imod() - 2);
    } else {
      return raw(inv_gcd((M)v, imod()));
    }
  }

  friend constexpr static_modint operator+(const static_modint &l,
                                           const static_modint &r) {
    static_modint res = l;
    return res += r;
  }
  friend constexpr static_modint operator-(const static_modint &l,
                                           const static_modint &r) {
    static_modint res = l;
    return res -= r;
  }
  friend constexpr static_modint operator*(const static_modint &l,
                                           const static_modint &r) {
    static_modint res = l;
    return res *= r;
  }
  friend constexpr static_modint operator/(const static_modint &l,
                                           const static_modint &r) {
    static_modint res = l;
    return res /= r;
  }

  constexpr bool operator==(const static_modint &o) const { return v == o.v; }
  constexpr bool operator!=(const static_modint &o) const {
    return !(*this == o);
  }

  friend constexpr istream &operator>>(istream &is, static_modint &o) {
    M v;
    is >> v;
    o = static_modint(v);
    return is;
  }
  friend constexpr ostream &operator<<(ostream &os, const static_modint &o) {
    return os << o.v;
  }

private:
  UM v;
};

template <int id = -1> struct dynamic_modint : internal::modint::modint_base {
public:
  static void set_mod(int mod) {
    assert(1 <= mod);
    bt = internal::math::barrett(mod);
  }
  static constexpr int imod() { return bt.umod(); }
  static constexpr unsigned int umod() { return bt.umod(); }

  static constexpr dynamic_modint raw(int a) {
    dynamic_modint res;
    res.v = a;
    return res;
  }

  constexpr dynamic_modint() : v(0) {}

  template <internal::concepts::broadly_signed_integral T>
  constexpr dynamic_modint(T a) {
    long long x = a % imod();
    if (x < 0) {
      x += imod();
    }
    v = x;
  }

  template <internal::concepts::broadly_unsigned_integral T>
  constexpr dynamic_modint(T a) : v(a % umod()) {}

  constexpr unsigned int val() const { return v; }
  constexpr dynamic_modint operator+() const { return *this; }
  constexpr dynamic_modint operator-() const {
    return raw(v == 0 ? 0 : imod() - v);
  }

  constexpr dynamic_modint &operator++() {
    ++v;
    if (v == umod()) {
      v = 0;
    }
    return *this;
  }
  constexpr dynamic_modint &operator--() {
    if (v == 0) {
      v = umod();
    }
    --v;
    return *this;
  }
  constexpr dynamic_modint operator++(int) {
    dynamic_modint x = *this;
    ++*this;
    return x;
  }
  constexpr dynamic_modint operator--(int) {
    dynamic_modint x = *this;
    --*this;
    return x;
  }

  constexpr dynamic_modint &operator+=(const dynamic_modint &o) {
    v += o.v;
    if (v >= umod()) {
      v -= umod();
    }
    return *this;
  }
  constexpr dynamic_modint &operator-=(const dynamic_modint &o) {
    if (v < o.v) {
      v += umod();
    }
    v -= o.v;
    return *this;
  }
  constexpr dynamic_modint &operator*=(const dynamic_modint &o) {
    v = bt.mul(v, o.v);
    //_v = (long long)_v * o._v % umod();
    return *this;
  }
  constexpr dynamic_modint &operator/=(const dynamic_modint &o) {
    return *this *= o.inv();
  }

  constexpr dynamic_modint pow(long long p) const {
    assert(p >= 0);
    dynamic_modint res = 1, b = *this;
    while (p) {
      if (p & 1) {
        res *= b;
      }
      b *= b;
      p >>= 1;
    }
    return res;
  }
  constexpr dynamic_modint inv() const { return raw(inv_gcd((int)v, imod())); }

  friend constexpr dynamic_modint operator+(const dynamic_modint &l,
                                            const dynamic_modint &r) {
    dynamic_modint res = l;
    return res += r;
  }
  friend constexpr dynamic_modint operator-(const dynamic_modint &l,
                                            const dynamic_modint &r) {
    dynamic_modint res = l;
    return res -= r;
  }
  friend constexpr dynamic_modint operator*(const dynamic_modint &l,
                                            const dynamic_modint &r) {
    dynamic_modint res = l;
    return res *= r;
  }
  friend constexpr dynamic_modint operator/(const dynamic_modint &l,
                                            const dynamic_modint &r) {
    dynamic_modint res = l;
    return res /= r;
  }

  constexpr bool operator==(const dynamic_modint &o) const { return v == o.v; }
  constexpr bool operator!=(const dynamic_modint &o) const {
    return !(*this == o);
  }

  friend constexpr istream &operator>>(istream &is, dynamic_modint &o) {
    int v;
    is >> v;
    o = dynamic_modint(v);
    return is;
  }
  friend constexpr ostream &operator<<(ostream &os, const dynamic_modint &o) {
    return os << o.v;
  }

private:
  unsigned int v;
  static internal::math::barrett bt;
};
template <int id> internal::math::barrett dynamic_modint<id>::bt(998244353);
} // namespace maomao90
