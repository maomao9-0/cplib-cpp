---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: library/internal/concepts.hpp
    title: library/internal/concepts.hpp
  - icon: ':heavy_check_mark:'
    path: library/internal/type_traits.hpp
    title: library/internal/type_traits.hpp
  - icon: ':heavy_check_mark:'
    path: library/math/pow_mod.hpp
    title: Pow Mod
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
    path: library/math/poly.hpp
    title: Poly
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/unit_test/math/modint.test.cpp
    title: verify/unit_test/math/modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/convolution/convolution.test.cpp
    title: verify/yosupo/convolution/convolution.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/convolution/convolution_1000000007.test.cpp
    title: verify/yosupo/convolution/convolution_1000000007.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/convolution/convolution_fft.test.cpp
    title: verify/yosupo/convolution/convolution_fft.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/convolution/convolution_fft_1000000007.test.cpp
    title: verify/yosupo/convolution/convolution_fft_1000000007.test.cpp
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
  bundledCode: "#line 2 \"library/math/primality_test.hpp\"\n\n#include <concepts>\n\
    \n#line 2 \"library/internal/type_traits.hpp\"\n\n#include <limits>\n#include\
    \ <type_traits>\n#include <utility>\n\nnamespace maomao90::internal::type_traits\
    \ {\nusing namespace std;\n#define ENABLE_VALUE(x) template <typename T> constexpr\
    \ bool x##_v = x<T>::value\n#define ENABLE_TYPE(x) template <typename T> using\
    \ x##_t = typename x<T>::type\n\ntemplate <typename T>\nusing is_broadly_signed\
    \ = disjunction<is_signed<T>, is_same<T, __int128>>;\nENABLE_VALUE(is_broadly_signed);\n\
    \ntemplate <typename T>\nusing is_broadly_unsigned =\n    disjunction<is_unsigned<T>,\
    \ is_same<T, unsigned __int128>>;\nENABLE_VALUE(is_broadly_unsigned);\n\ntemplate\
    \ <typename T>\nusing is_broadly_integral = disjunction<is_integral<T>, is_same<T,\
    \ __int128>,\n                                        is_same<T, unsigned __int128>>;\n\
    ENABLE_VALUE(is_broadly_integral);\n\ntemplate <typename T>\nusing is_broadly_signed_integral\
    \ =\n    conjunction<is_broadly_signed<T>, is_broadly_integral<T>>;\nENABLE_VALUE(is_broadly_signed_integral);\n\
    \ntemplate <typename T>\nusing is_broadly_unsigned_integral =\n    conjunction<is_broadly_unsigned<T>,\
    \ is_broadly_integral<T>>;\nENABLE_VALUE(is_broadly_unsigned_integral);\n\ntemplate\
    \ <typename T>\nusing is_32bit_or_less = bool_constant<numeric_limits<T>::max()\
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
    #line 2 \"library/math/pow_mod.hpp\"\n\n#line 4 \"library/math/pow_mod.hpp\"\n\
    \n#line 2 \"library/internal/concepts.hpp\"\n\n#line 4 \"library/internal/concepts.hpp\"\
    \n\nnamespace maomao90::internal::concepts {\nusing namespace std;\ntemplate <typename\
    \ T>\nconcept broadly_integral = internal::type_traits::is_broadly_integral_v<T>;\n\
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
    \    p >>= 1;\n  }\n  return res;\n}\n} // namespace maomao90\n#line 7 \"library/math/primality_test.hpp\"\
    \n\nnamespace maomao90 {\nusing namespace std;\ntemplate <unsigned_integral T>\n\
    \  requires internal::type_traits::is_64bit_or_less_v<T>\nconstexpr bool miller_rabin(const\
    \ T &n, const T *bases, const size_t size) {\n  using U = internal::type_traits::safely_multipliable_t<T>;\n\
    \  if (n <= 1) {\n    return false;\n  }\n  if (n == 2) {\n    return true;\n\
    \  }\n  if (n % 2 == 0) {\n    return false;\n  }\n  T d = n - 1;\n  while (d\
    \ % 2 == 0)\n    d /= 2;\n  for (size_t i = 0; i < size; i++) {\n    T a = bases[i];\n\
    \    if (a % n == 0) {\n      continue;\n    }\n    T t = d, y = pow_mod<T, T>(a,\
    \ t, n);\n    while (t != n - 1 && y != 1 && y != n - 1) {\n      y = (U)y * y\
    \ % n;\n      t <<= 1;\n    }\n    if (y != n - 1 && t % 2 == 0) {\n      return\
    \ false;\n    }\n  }\n  return true;\n}\ntemplate <unsigned_integral T>\n  requires\
    \ internal::type_traits::is_64bit_or_less_v<T>\nconstexpr bool is_prime(T n) {\n\
    \  if constexpr (internal::type_traits::is_32bit_or_less_v<T>) {\n    T bases[3]\
    \ = {2, 7, 61};\n    return miller_rabin<T>(n, bases, 3);\n  } else {\n    T bases[7]\
    \ = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};\n    return miller_rabin<T>(n,\
    \ bases, 7);\n  }\n}\n} // namespace maomao90\n"
  code: "#pragma once\n\n#include <concepts>\n\n#include \"library/internal/type_traits.hpp\"\
    \n#include \"library/math/pow_mod.hpp\"\n\nnamespace maomao90 {\nusing namespace\
    \ std;\ntemplate <unsigned_integral T>\n  requires internal::type_traits::is_64bit_or_less_v<T>\n\
    constexpr bool miller_rabin(const T &n, const T *bases, const size_t size) {\n\
    \  using U = internal::type_traits::safely_multipliable_t<T>;\n  if (n <= 1) {\n\
    \    return false;\n  }\n  if (n == 2) {\n    return true;\n  }\n  if (n % 2 ==\
    \ 0) {\n    return false;\n  }\n  T d = n - 1;\n  while (d % 2 == 0)\n    d /=\
    \ 2;\n  for (size_t i = 0; i < size; i++) {\n    T a = bases[i];\n    if (a %\
    \ n == 0) {\n      continue;\n    }\n    T t = d, y = pow_mod<T, T>(a, t, n);\n\
    \    while (t != n - 1 && y != 1 && y != n - 1) {\n      y = (U)y * y % n;\n \
    \     t <<= 1;\n    }\n    if (y != n - 1 && t % 2 == 0) {\n      return false;\n\
    \    }\n  }\n  return true;\n}\ntemplate <unsigned_integral T>\n  requires internal::type_traits::is_64bit_or_less_v<T>\n\
    constexpr bool is_prime(T n) {\n  if constexpr (internal::type_traits::is_32bit_or_less_v<T>)\
    \ {\n    T bases[3] = {2, 7, 61};\n    return miller_rabin<T>(n, bases, 3);\n\
    \  } else {\n    T bases[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};\n\
    \    return miller_rabin<T>(n, bases, 7);\n  }\n}\n} // namespace maomao90\n"
  dependsOn:
  - library/internal/type_traits.hpp
  - library/math/pow_mod.hpp
  - library/internal/concepts.hpp
  isVerificationFile: false
  path: library/math/primality_test.hpp
  requiredBy:
  - library/math/poly.hpp
  - library/math/modint.hpp
  - library/hashing/multiset_hash.hpp
  - library/hashing/hashing_custom_hash.hpp
  timestamp: '2024-12-13 23:21:45+08:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
  - verify/yosupo/number_theory/primality_test.test.cpp
  - verify/yosupo/convolution/convolution_1000000007.test.cpp
  - verify/yosupo/convolution/convolution_fft.test.cpp
  - verify/yosupo/convolution/convolution.test.cpp
  - verify/yosupo/convolution/convolution_fft_1000000007.test.cpp
  - verify/unit_test/math/modint.test.cpp
documentation_of: library/math/primality_test.hpp
layout: document
title: Primality Test
---
