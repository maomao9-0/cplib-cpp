#pragma once

#include "library/internal/type_traits.hpp"

namespace maomao90::internal::concepts {
using namespace std;
template <typename T>
concept broadly_integral = internal::type_traits::is_broadly_integral_v<T>;

template <typename T>
concept broadly_signed_integral =
    internal::type_traits::is_broadly_signed_integral_v<T>;

template <typename T>
concept broadly_unsigned_integral =
    internal::type_traits::is_broadly_unsigned_integral_v<T>;

template <typename T>
concept Addable = requires(T a) {
  a + a;
  a += a;
};
template <typename T>
concept Subtractable = requires(T a) {
  a - a;
  a -= a;
};
template <typename T>
concept Multipliable = requires(T a) {
  a *a;
  a *= a;
};
template <typename T>
concept Dividable = requires(T a) {
  a / a;
  a /= a;
};

template <typename T>
concept Iterable = requires(T &t) {
  begin(t) != end(t);
  ++std::declval<decltype(begin(t)) &>();
  *begin(t);
};

template <typename T>
concept Monoid = requires(const T &a, const T &b) {
  { T::id() } -> same_as<T>;
  { a.merge(b) } -> same_as<T>;
};

template <typename L, typename T>
concept Lazy = requires(const L &lz, const T &x, const int len) {
  { lz.apply(x, len) } -> same_as<T>;
  requires Monoid<L>;
};
} // namespace maomao90::internal::concepts
