#pragma once

#include <concepts>
#include <limits>
#include <tuple>
#include <utility>

namespace maomao90 {
using namespace std;

namespace internal::monoid {
const bool MIN_FLAG = true, MAX_FLAG = false;
template <bool is_min, typename T> struct limits;
template <bool is_min, typename T>
constexpr T limits_v = limits<is_min, T>::value;

template <integral T> struct limits<true, T> {
  static constexpr T value = numeric_limits<T>::min() / 2;
};
template <integral T> struct limits<false, T> {
  static constexpr T value = numeric_limits<T>::max() / 2;
};
template <> struct limits<MIN_FLAG, __int128> {
  static constexpr __int128 value = (__int128)numeric_limits<long long>::min() /
                                    2 * numeric_limits<long long>::max() / 2;
};
template <> struct limits<MAX_FLAG, __int128> {
  static constexpr __int128 value = (__int128)numeric_limits<long long>::max() /
                                    2 * numeric_limits<long long>::max() / 2;
};

template <bool is_min, typename A, typename B>
struct limits<is_min, pair<A, B>> {
  static constexpr pair<A, B> value =
      make_pair(limits_v<is_min, A>, limits_v<is_min, B>);
};

template <bool is_min, typename Head, typename... Tail>
struct limits<is_min, tuple<Head, Tail...>> {
  static constexpr tuple<Head, Tail...> value = tuple_cat(
      make_tuple(limits_v<is_min, Head>), limits_v<is_min, tuple<Tail...>>);
};
template <bool is_min> struct limits<is_min, tuple<>> {
  static constexpr tuple<> value = make_tuple();
};
} // namespace internal::monoid

template <typename T> struct MinMonoid {
  T v;
  static MinMonoid id() {
    return {internal::monoid::limits_v<internal::monoid::MAX_FLAG, T>};
  }
  MinMonoid merge(const MinMonoid &o) const { return {min(v, o.v)}; }
};
template <typename T> struct MaxMonoid {
  T v;
  static MaxMonoid id() {
    return {internal::monoid::limits_v<internal::monoid::MIN_FLAG, T>};
  }
  MaxMonoid merge(const MaxMonoid &o) const { return {max(v, o.v)}; }
};
template <typename T> struct SumMonoid {
  T v;
  static SumMonoid id() { return {T(0)}; }
  SumMonoid merge(const SumMonoid &o) const { return {v + o.v}; }
};
} // namespace maomao90
