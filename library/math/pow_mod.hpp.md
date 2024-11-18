---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: library/internal/concepts.hpp
    title: library/internal/concepts.hpp
  - icon: ':heavy_check_mark:'
    path: library/internal/type_traits.hpp
    title: library/internal/type_traits.hpp
  _extendedRequiredBy:
  - icon: ':warning:'
    path: library/hashing/hashing_custom_hash.hpp
    title: Hashing Custom Hash
  - icon: ':warning:'
    path: library/hashing/multiset_hash.hpp
    title: Multiset Hash
  - icon: ':heavy_check_mark:'
    path: library/math/modint.hpp
    title: ModInt
  - icon: ':heavy_check_mark:'
    path: library/math/primality_test.hpp
    title: Primality Test
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/unit_test/math/modint.test.cpp
    title: verify/unit_test/math/modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/number_theory/primality_test.test.cpp
    title: verify/yosupo/number_theory/primality_test.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
    title: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"library/math/pow_mod.hpp\"\n\n#include <concepts>\n\n#line\
    \ 2 \"library/internal/concepts.hpp\"\n\n#line 2 \"library/internal/type_traits.hpp\"\
    \n\n#include <limits>\n#include <type_traits>\n#include <utility>\n\nnamespace\
    \ maomao90::internal::type_traits {\nusing namespace std;\n#define ENABLE_VALUE(x)\
    \ template <typename T> constexpr bool x##_v = x<T>::value\n#define ENABLE_TYPE(x)\
    \ template <typename T> using x##_t = typename x<T>::type\n\ntemplate <typename\
    \ T>\nusing is_broadly_signed = disjunction<is_signed<T>, is_same<T, __int128>>;\n\
    ENABLE_VALUE(is_broadly_signed);\n\ntemplate <typename T>\nusing is_broadly_unsigned\
    \ =\n    disjunction<is_unsigned<T>, is_same<T, unsigned __int128>>;\nENABLE_VALUE(is_broadly_unsigned);\n\
    \ntemplate <typename T>\nusing is_broadly_integral = disjunction<is_integral<T>,\
    \ is_same<T, __int128>,\n                                        is_same<T, unsigned\
    \ __int128>>;\nENABLE_VALUE(is_broadly_integral);\n\ntemplate <typename T>\nusing\
    \ is_broadly_signed_integral =\n    conjunction<is_broadly_signed<T>, is_broadly_integral<T>>;\n\
    ENABLE_VALUE(is_broadly_signed_integral);\n\ntemplate <typename T>\nusing is_broadly_unsigned_integral\
    \ =\n    conjunction<is_broadly_unsigned<T>, is_broadly_integral<T>>;\nENABLE_VALUE(is_broadly_unsigned_integral);\n\
    \ntemplate <typename T>\nusing is_32bit_or_less = bool_constant<numeric_limits<T>::max()\
    \ <=\n                                       numeric_limits<unsigned int>::max()>;\n\
    ENABLE_VALUE(is_32bit_or_less);\n\ntemplate <typename T>\nusing is_64bit_or_less\
    \ =\n    bool_constant<numeric_limits<T>::max() <=\n                  numeric_limits<unsigned\
    \ long long>::max()>;\nENABLE_VALUE(is_64bit_or_less);\n\ntemplate <typename T>\n\
    using safely_multipliable_unsigned =\n    conditional<is_32bit_or_less_v<T>, unsigned\
    \ long long, unsigned __int128>;\nENABLE_TYPE(safely_multipliable_unsigned);\n\
    \ntemplate <typename T>\nusing safely_multipliable_signed =\n    conditional<is_32bit_or_less_v<T>,\
    \ long long, __int128>;\nENABLE_TYPE(safely_multipliable_signed);\n\ntemplate\
    \ <typename T>\nusing safely_multipliable =\n    conditional<is_signed_v<T>, safely_multipliable_signed_t<T>,\n\
    \                safely_multipliable_unsigned_t<T>>;\nENABLE_TYPE(safely_multipliable);\n\
    \ntemplate <typename> struct is_pair : false_type {};\ntemplate <typename T, typename\
    \ U> struct is_pair<pair<T, U>> : true_type {};\nENABLE_VALUE(is_pair);\n\n#undef\
    \ ENABLE_VALUE\n#undef ENABLE_TYPE\n} // namespace maomao90::internal::type_traits\n\
    #line 4 \"library/internal/concepts.hpp\"\n\nnamespace maomao90::internal::concepts\
    \ {\nusing namespace std;\ntemplate <typename T>\nconcept broadly_integral = internal::type_traits::is_broadly_integral_v<T>;\n\
    \ntemplate <typename T>\nconcept broadly_signed_integral =\n    internal::type_traits::is_broadly_signed_integral_v<T>;\n\
    \ntemplate <typename T>\nconcept broadly_unsigned_integral =\n    internal::type_traits::is_broadly_unsigned_integral_v<T>;\n\
    \ntemplate <typename T>\nconcept Addable = requires(T a) {\n  a + a;\n  a += a;\n\
    };\ntemplate <typename T>\nconcept Subtractable = requires(T a) {\n  a - a;\n\
    \  a -= a;\n};\ntemplate <typename T>\nconcept Multipliable = requires(T a) {\n\
    \  a *a;\n  a *= a;\n};\ntemplate <typename T>\nconcept Dividable = requires(T\
    \ a) {\n  a / a;\n  a /= a;\n};\n\ntemplate <typename T>\nconcept Iterable = requires(T\
    \ &t) {\n  begin(t) != end(t);\n  ++std::declval<decltype(begin(t)) &>();\n  *begin(t);\n\
    };\n} // namespace maomao90::internal::concepts\n#line 7 \"library/math/pow_mod.hpp\"\
    \n\nnamespace maomao90 {\nusing namespace std;\ntemplate <integral T, internal::concepts::broadly_unsigned_integral\
    \ P>\n  requires internal::type_traits::is_64bit_or_less_v<T>\nconstexpr T pow_mod(T\
    \ b, P p, T mod) {\n  using U = internal::type_traits::safely_multipliable_t<T>;\n\
    \  T res = 1;\n  b %= mod;\n  if (b < 0) {\n    b += mod;\n  }\n  while (p) {\n\
    \    if (p & 1) {\n      res = (U)res * b % mod;\n    }\n    b = (U)b * b % mod;\n\
    \    p >>= 1;\n  }\n  return res;\n}\n} // namespace maomao90\n"
  code: "#pragma once\n\n#include <concepts>\n\n#include \"library/internal/concepts.hpp\"\
    \n#include \"library/internal/type_traits.hpp\"\n\nnamespace maomao90 {\nusing\
    \ namespace std;\ntemplate <integral T, internal::concepts::broadly_unsigned_integral\
    \ P>\n  requires internal::type_traits::is_64bit_or_less_v<T>\nconstexpr T pow_mod(T\
    \ b, P p, T mod) {\n  using U = internal::type_traits::safely_multipliable_t<T>;\n\
    \  T res = 1;\n  b %= mod;\n  if (b < 0) {\n    b += mod;\n  }\n  while (p) {\n\
    \    if (p & 1) {\n      res = (U)res * b % mod;\n    }\n    b = (U)b * b % mod;\n\
    \    p >>= 1;\n  }\n  return res;\n}\n} // namespace maomao90\n"
  dependsOn:
  - library/internal/concepts.hpp
  - library/internal/type_traits.hpp
  isVerificationFile: false
  path: library/math/pow_mod.hpp
  requiredBy:
  - library/math/primality_test.hpp
  - library/math/modint.hpp
  - library/hashing/multiset_hash.hpp
  - library/hashing/hashing_custom_hash.hpp
  timestamp: '2024-11-18 20:19:33+08:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
  - verify/yosupo/number_theory/primality_test.test.cpp
  - verify/unit_test/math/modint.test.cpp
documentation_of: library/math/pow_mod.hpp
layout: document
title: Pow Mod
---
