/**
 * @file multiset_hash.hpp
 * @brief Randomized hash for multisets of signed integer values.
 *
 * This is useful when multisets need fast equality checks or affine shifts of
 * all values, but the result is still a probabilistic hash.
 */
#pragma once

#include <array>

#include "library/internal/hashing.hpp"
#include "library/math/modint.hpp"

namespace maomao90 {
using namespace std;
template <StaticModInt mint = static_modint<(1ll << 61) - 1>,
          size_t num_bases = 1, size_t CACHE = 1'000'000>
/**
 * @brief Probabilistic hash of an integer multiset.
 *
 * @tparam mint Static modint used for each base.
 * @tparam num_bases Number of independent random bases.
 * @tparam CACHE Number of precomputed powers for fast small offsets.
 * @details The hash is represented under `num_bases` random bases. Different
 * update sequences that describe the same multiset produce the same hash value.
 * The arithmetic operators also let the hash behave like a signed multiset sum.
 *
 * Typical costs are:
 * - `insert()` / `erase()` / `offset()`: \f$O(\texttt{num\_bases})\f$ when the
 *   queried power is cached, otherwise
 *   \f$O(\texttt{num\_bases} \log |x|)\f$.
 * - `get_v()`, `operator+`, `operator-`, comparisons:
 *   \f$O(\texttt{num\_bases})\f$.
 *
 * @note Equal objects mean hash equality, not a proof of multiset equality.
 * @note Hash values are not stable across different program runs.
 */
struct MultisetHash {
  static_assert(mint::umod() > internal::hashing::MIN_HASH_BASE,
                "MultisetHash requires mint::umod() > MIN_HASH_BASE");

  /**
   * @brief Returns the hash coordinates under each random base.
   * @return Array of residues, one per base.
   * Complexity: \f$O(\texttt{num\_bases})\f$.
   */
  constexpr array<typename mint::umod_type, num_bases> get_v() const {
    array<typename mint::umod_type, num_bases> res;
    for (int i = 0; i < num_bases; i++) {
      res[i] = v[i].val();
    }
    return res;
  }
  /**
   * @brief Returns the hash after inserting `cnt` copies of `a`.
   * Complexity: \f$O(\texttt{num\_bases})\f$ with cached powers, otherwise
   * \f$O(\texttt{num\_bases} \log |a|)\f$.
   */
  constexpr MultisetHash insert(long long a, long long cnt = 1) const {
    MultisetHash res = *this;
    for (int i = 0; i < num_bases; i++) {
      res.v[i] += get_pow(i, a) * cnt;
    }
    return res;
  }
  /**
   * @brief Returns the hash after erasing `cnt` copies of `a`.
   * Complexity: \f$O(\texttt{num\_bases})\f$ with cached powers, otherwise
   * \f$O(\texttt{num\_bases} \log |a|)\f$.
   */
  constexpr MultisetHash erase(long long a, long long cnt = 1) const {
    MultisetHash res = *this;
    for (int i = 0; i < num_bases; i++) {
      res.v[i] -= get_pow(i, a) * cnt;
    }
    return res;
  }
  /**
   * @brief Adds `delta` to every multiset element.
   * @param delta Signed offset applied to all stored values.
   * Complexity: \f$O(\texttt{num\_bases})\f$ with cached powers, otherwise
   * \f$O(\texttt{num\_bases} \log |\delta|)\f$.
   */
  constexpr MultisetHash offset(long long delta) const {
    MultisetHash res = *this;
    for (int i = 0; i < num_bases; i++) {
      res.v[i] *= get_pow(i, delta);
    }
    return res;
  }
  /// @brief Adds hashes as signed multisets. Complexity: \f$O(\texttt{num\_bases})\f$.
  constexpr MultisetHash operator+(const MultisetHash &o) const {
    MultisetHash res = *this;
    return res += o;
  }
  /// @brief In-place multiset hash addition. Complexity: \f$O(\texttt{num\_bases})\f$.
  constexpr MultisetHash &operator+=(const MultisetHash &o) {
    for (int i = 0; i < num_bases; i++) {
      v[i] += o.v[i];
    }
    return *this;
  }
  /// @brief Subtracts hashes as signed multisets. Complexity: \f$O(\texttt{num\_bases})\f$.
  constexpr MultisetHash operator-(const MultisetHash &o) const {
    MultisetHash res = *this;
    return res -= o;
  }
  /// @brief In-place multiset hash subtraction. Complexity: \f$O(\texttt{num\_bases})\f$.
  constexpr MultisetHash &operator-=(const MultisetHash &o) {
    for (int i = 0; i < num_bases; i++) {
      v[i] -= o.v[i];
    }
    return *this;
  }
  /// @brief Compares all bases for equality. Complexity: \f$O(\texttt{num\_bases})\f$.
  constexpr bool operator==(const MultisetHash &o) const {
    for (int i = 0; i < num_bases; i++) {
      if (v[i] != o.v[i]) {
        return false;
      }
    }
    return true;
  }
  /// @brief Negated equality comparison. Complexity: \f$O(\texttt{num\_bases})\f$.
  constexpr bool operator!=(const MultisetHash &o) const {
    return !(*this == o);
  }

private:
  array<mint, num_bases> v;
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
