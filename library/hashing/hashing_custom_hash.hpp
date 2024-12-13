#pragma once

#include <type_traits>

#include "library/internal/concepts.hpp"
#include "library/internal/hashing.hpp"
#include "library/math/modint.hpp"

namespace maomao90 {
using namespace std;
template <typename T>
concept Hashing = requires(T v) {
  requires StaticModInt<typename T::mint>;
  requires internal::concepts::Iterable<decltype(v.get_v())>;
  requires is_same_v<typename decltype(v.get_v())::value_type,
                     typename T::mint::umod_type>;
};
template <Hashing T> struct HashingCustomHash {
  constexpr size_t operator()(const T &o) const {
    return internal::hashing::hash_function(o.get_v());
  }
};
} // namespace maomao90
