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

  static constexpr static_modint raw(M v) {
    static_modint res;
    res._v = v;
    return res;
  }

  constexpr static_modint() : _v(0) {}

  template <internal::concepts::broadly_signed_integral T>
  constexpr static_modint(T v) {
    M x = v % imod();
    if (x < 0) {
      x += imod();
    }
    _v = x;
  }

  template <internal::concepts::broadly_unsigned_integral T>
  constexpr static_modint(T v) : _v(v % umod()) {}

  constexpr UM val() const { return _v; }

  constexpr static_modint operator+() const { return *this; }
  constexpr static_modint operator-() const {
    return raw(_v == 0 ? 0 : imod() - _v);
  }

  constexpr static_modint &operator++() {
    ++_v;
    if (_v == umod()) {
      _v = 0;
    }
    return *this;
  }
  constexpr static_modint &operator--() {
    if (_v == 0) {
      _v = umod();
    }
    --_v;
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
    _v += o._v;
    if (_v >= umod()) {
      _v -= umod();
    }
    return *this;
  }
  constexpr static_modint &operator-=(const static_modint &o) {
    if (_v < o._v) {
      _v += umod();
    }
    _v -= o._v;
    return *this;
  }
  constexpr static_modint &operator*=(const static_modint &o) {
    _v = (BM)_v * o._v % umod();
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
      return raw(inv_gcd((M)_v, imod()));
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

  constexpr bool operator==(const static_modint &o) const { return _v == o._v; }
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
    return os << o._v;
  }

private:
  UM _v;
};

template <int id = -1> struct dynamic_modint : internal::modint::modint_base {
public:
  static void set_mod(int mod) {
    assert(1 <= mod);
    bt = internal::math::barrett(mod);
  }
  static constexpr int imod() { return bt.umod(); }
  static constexpr unsigned int umod() { return bt.umod(); }

  static constexpr dynamic_modint raw(int v) {
    dynamic_modint res;
    res._v = v;
    return res;
  }

  constexpr dynamic_modint() : _v(0) {}

  template <internal::concepts::broadly_signed_integral T>
  constexpr dynamic_modint(T v) {
    long long x = v % imod();
    if (x < 0) {
      x += imod();
    }
    _v = x;
  }

  template <internal::concepts::broadly_unsigned_integral T>
  constexpr dynamic_modint(T v) : _v(v % umod()) {}

  constexpr unsigned int val() const { return _v; }
  constexpr dynamic_modint operator+() const { return *this; }
  constexpr dynamic_modint operator-() const {
    return raw(_v == 0 ? 0 : imod() - _v);
  }

  constexpr dynamic_modint &operator++() {
    ++_v;
    if (_v == umod()) {
      _v = 0;
    }
    return *this;
  }
  constexpr dynamic_modint &operator--() {
    if (_v == 0) {
      _v = umod();
    }
    --_v;
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
    _v += o._v;
    if (_v >= umod()) {
      _v -= umod();
    }
    return *this;
  }
  constexpr dynamic_modint &operator-=(const dynamic_modint &o) {
    if (_v < o._v) {
      _v += umod();
    }
    _v -= o._v;
    return *this;
  }
  constexpr dynamic_modint &operator*=(const dynamic_modint &o) {
    _v = bt.mul(_v, o._v);
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
  constexpr dynamic_modint inv() const { return raw(inv_gcd((int)_v, imod())); }

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

  constexpr bool operator==(const dynamic_modint &o) const {
    return _v == o._v;
  }
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
    return os << o._v;
  }

private:
  unsigned int _v;
  static internal::math::barrett bt;
};
template <int id> internal::math::barrett dynamic_modint<id>::bt(998244353);
} // namespace maomao90
