---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':warning:'
    path: library/hashing/multiset_hash.hpp
    title: Multiset Hash
  - icon: ':heavy_check_mark:'
    path: library/math/extended_gcd.hpp
    title: Extended GCD
  - icon: ':heavy_check_mark:'
    path: library/math/modint.hpp
    title: ModInt
  - icon: ':heavy_check_mark:'
    path: library/math/pow_mod.hpp
    title: Pow Mod
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
  bundledCode: "#line 2 \"library/internal/type_traits.hpp\"\n\n#include <type_traits>\n\
    #include <limits>\n\nnamespace maomao90::internal::type_traits {\n#define ENABLE_VALUE(x)\
    \ \\\n    template <typename T> \\\n    constexpr bool x##_v = x<T>::value\n#define\
    \ ENABLE_TYPE(x) \\\n    template <typename T> \\\n    using x##_t = typename\
    \ x<T>::type\n\n    template <typename T>\n    using is_32bit_or_less = bool_constant<numeric_limits<T>::max()\
    \ <= numeric_limits<unsigned int>::max()>;\n    ENABLE_VALUE(is_32bit_or_less);\n\
    \n    template <typename T>\n    using is_64bit_or_less = bool_constant<numeric_limits<T>::max()\
    \ <= numeric_limits<unsigned long long>::max()>;\n    ENABLE_VALUE(is_64bit_or_less);\n\
    \n    template <typename T>\n    using safely_multipliable_unsigned = conditional<is_32bit_or_less_v<T>,\
    \ unsigned long long, unsigned __int128>;\n    ENABLE_TYPE(safely_multipliable_unsigned);\n\
    \n    template <typename T>\n    using safely_multipliable_signed = conditional<is_32bit_or_less_v<T>,\
    \ long long, __int128>;\n    ENABLE_TYPE(safely_multipliable_signed);\n\n    template\
    \ <typename T>\n    using safely_multipliable = conditional<is_signed_v<T>, safely_multipliable_signed_t<T>,\
    \ safely_multipliable_unsigned_t<T>>;\n    ENABLE_TYPE(safely_multipliable);\n\
    \n#undef ENABLE_VALUE\n}\n"
  code: "#pragma once\n\n#include <type_traits>\n#include <limits>\n\nnamespace maomao90::internal::type_traits\
    \ {\n#define ENABLE_VALUE(x) \\\n    template <typename T> \\\n    constexpr bool\
    \ x##_v = x<T>::value\n#define ENABLE_TYPE(x) \\\n    template <typename T> \\\
    \n    using x##_t = typename x<T>::type\n\n    template <typename T>\n    using\
    \ is_32bit_or_less = bool_constant<numeric_limits<T>::max() <= numeric_limits<unsigned\
    \ int>::max()>;\n    ENABLE_VALUE(is_32bit_or_less);\n\n    template <typename\
    \ T>\n    using is_64bit_or_less = bool_constant<numeric_limits<T>::max() <= numeric_limits<unsigned\
    \ long long>::max()>;\n    ENABLE_VALUE(is_64bit_or_less);\n\n    template <typename\
    \ T>\n    using safely_multipliable_unsigned = conditional<is_32bit_or_less_v<T>,\
    \ unsigned long long, unsigned __int128>;\n    ENABLE_TYPE(safely_multipliable_unsigned);\n\
    \n    template <typename T>\n    using safely_multipliable_signed = conditional<is_32bit_or_less_v<T>,\
    \ long long, __int128>;\n    ENABLE_TYPE(safely_multipliable_signed);\n\n    template\
    \ <typename T>\n    using safely_multipliable = conditional<is_signed_v<T>, safely_multipliable_signed_t<T>,\
    \ safely_multipliable_unsigned_t<T>>;\n    ENABLE_TYPE(safely_multipliable);\n\
    \n#undef ENABLE_VALUE\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: library/internal/type_traits.hpp
  requiredBy:
  - library/math/extended_gcd.hpp
  - library/math/primality_test.hpp
  - library/math/pow_mod.hpp
  - library/math/modint.hpp
  - library/hashing/multiset_hash.hpp
  timestamp: '2024-10-22 15:04:08+08:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
  - verify/yosupo/number_theory/primality_test.test.cpp
  - verify/unit_test/math/modint.test.cpp
documentation_of: library/internal/type_traits.hpp
layout: document
redirect_from:
- /library/library/internal/type_traits.hpp
- /library/library/internal/type_traits.hpp.html
title: library/internal/type_traits.hpp
---
