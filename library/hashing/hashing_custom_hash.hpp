/**
 * @file hashing_custom_hash.hpp
 * @brief Adapter from library hash objects to standard hash containers.
 *
 * This is meant for types that already expose a compact modular hash vector and
 * need a `std::hash`-style wrapper for `unordered_map`, `unordered_set`, or the
 * library `HashMap`.
 */
#pragma once

#include <type_traits>

#include "library/internal/concepts.hpp"
#include "library/internal/hashing.hpp"
#include "library/math/modint.hpp"

namespace maomao90 {
using namespace std;
template <typename T>
/**
 * @brief Concept for objects exposing a modular hash vector through `get_v()`.
 *
 * A type satisfies `Hashing` when:
 * - it has a member type `mint` satisfying `StaticModInt`,
 * - `get_v()` returns an iterable container,
 * - the iterable stores values of type `mint::umod_type`.
 */
concept Hashing = requires(T v) {
  requires StaticModInt<typename T::mint>;
  requires internal::concepts::Iterable<decltype(v.get_v())>;
  requires is_same_v<
      typename remove_cvref_t<decltype(v.get_v())>::value_type,
                     typename T::mint::umod_type>;
};
/**
 * @brief `unordered_map` / `unordered_set` hash functor for library hash types.
 *
 * @tparam T Type satisfying `Hashing`.
 *
 * Example:
 * @code{.cpp}
 * #include <unordered_set>
 * #include "library/hashing/hashing_custom_hash.hpp"
 * #include "library/hashing/multiset_hash.hpp"
 * using namespace std;
 * using namespace maomao90;
 *
 * int main() {
 *   MultisetHash<> multiset_hash;
 *   unordered_set<MultisetHash<>, HashingCustomHash<MultisetHash<>>> st;
 *   st.insert(multiset_hash);
 * }
 * @endcode
 */
template <Hashing T> struct HashingCustomHash {
  /// @brief Mixes the `get_v()` sequence into a `size_t` hash value.
  constexpr size_t operator()(const T &o) const {
    return internal::hashing::hash_function(o.get_v());
  }
};
} // namespace maomao90
