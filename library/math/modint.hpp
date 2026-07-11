/**
 * @file modint.hpp
 * @brief Static and dynamic modular integer types.
 *
 * These types normalize values into `[0, mod)` and provide constant-time
 * arithmetic, but inverses require coprime values and the static modulus must
 * be at least `2`.
 */
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
/// @brief Satisfied by `static_modint` and `dynamic_modint`.
concept ModInt = is_base_of_v<internal::modint::modint_base, T>;
template <typename T>
/// @brief Satisfied only by `static_modint`.
concept StaticModInt = is_base_of_v<internal::modint::static_modint_base, T>;

/**
 * @brief Modular integer with compile-time modulus.
 *
 * @tparam mod Positive signed modulus with `2 <= mod`.
 * @note All stored values are normalized into `[0, mod)`.
 * @note `inv()` assumes the stored value is coprime to `mod`.
 */
template <auto mod = 998244353, enable_if_t<(mod >= 2), nullptr_t> = nullptr>
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
  /// @brief Signed modulus.
  static constexpr M imod() { return mod; }
  /// @brief Unsigned modulus.
  static constexpr UM umod() { return mod; }

  /// @brief Returns a value without normalization. Requires `0 <= a < mod`.
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

  /// @brief Stored representative in `[0, mod)`.
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

  /**
   * @brief Computes the stored value raised to the power of `p`.
   * @param p Non-negative exponent.
   * @return Value raised to `p`.
   * Complexity: `O(log p)`.
   */
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
  /**
   * @brief Computes the multiplicative inverse of the stored value.
   * @return `x` such that `val() * x == 1`.
   * @warning Requires `gcd(val(), mod) == 1`.
   * Complexity: `O(log mod)`.
   */
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
  explicit constexpr operator bool() const { return v; }

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

/**
 * @brief Modular integer with runtime modulus shared by `id`.
 *
 * @tparam id Distinguishes independent runtime moduli.
 * @warning Call `set_mod()` before constructing values for a given `id`.
 * @note The runtime modulus must be at least `2`.
 */
template <int id = -1> struct dynamic_modint : internal::modint::modint_base {
public:
  /// @brief Sets the modulus for this `id`. Requires `mod >= 2`.
  static void set_mod(int mod) {
    assert(2 <= mod);
    bt = internal::math::barrett(mod);
  }
  /// @brief Signed modulus.
  static constexpr int imod() { return bt.umod(); }
  /// @brief Unsigned modulus.
  static constexpr unsigned int umod() { return bt.umod(); }

  /// @brief Returns a value without normalization. Requires `0 <= a < mod`.
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

  /// @brief Stored representative in `[0, mod)`.
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
    return *this;
  }
  constexpr dynamic_modint &operator/=(const dynamic_modint &o) {
    return *this *= o.inv();
  }

  /**
   * @brief Computes the stored value raised to the power of `p`.
   * @param p Non-negative exponent.
   * @return Value raised to `p`.
   * Complexity: `O(log p)`.
   */
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
  /**
   * @brief Computes the multiplicative inverse of the stored value.
   * @return `x` such that `val() * x == 1`.
   * @warning Requires `gcd(val(), mod) == 1`.
   * Complexity: `O(log mod)`.
   */
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
  explicit constexpr operator bool() const { return v; }

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
