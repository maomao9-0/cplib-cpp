---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: library/internal/type_traits.hpp
    title: library/internal/type_traits.hpp
  _extendedRequiredBy:
  - icon: ':warning:'
    path: library/hashing/multiset_hash.hpp
    title: Multiset Hash
  - icon: ':heavy_check_mark:'
    path: library/math/modint.hpp
    title: ModInt
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/unit_test/math/modint.test.cpp
    title: verify/unit_test/math/modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
    title: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"library/math/extended_gcd.hpp\"\n\n#line 2 \"library/internal/type_traits.hpp\"\
    \n\n#include <type_traits>\n#include <limits>\n\nnamespace maomao90::internal::type_traits\
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
    \n#undef ENABLE_VALUE\n}\n#line 4 \"library/math/extended_gcd.hpp\"\n\n#include\
    \ <cassert>\n#line 7 \"library/math/extended_gcd.hpp\"\n#include <concepts>\n\n\
    namespace maomao90 {\n    template <signed_integral T> requires\n        internal::type_traits::is_64bit_or_less_v<T>\n\
    \    constexpr T inv_gcd(T x, T mod) {\n        using U = internal::type_traits::safely_multipliable_t<T>;\n\
    \        U a = mod, b = x, va = 0, vb = 1;\n        while (b) {\n            T\
    \ k = a / b;\n            a -= k * b; va -= k * vb;\n            swap(a, b);\n\
    \            swap(va, vb);\n        }\n        assert(a == 1); // gcd should be\
    \ equal to 1\n        return (va % mod + mod) % mod;\n    }\n}\n"
  code: "#pragma once\n\n#include \"library/internal/type_traits.hpp\"\n\n#include\
    \ <cassert>\n#include <type_traits>\n#include <concepts>\n\nnamespace maomao90\
    \ {\n    template <signed_integral T> requires\n        internal::type_traits::is_64bit_or_less_v<T>\n\
    \    constexpr T inv_gcd(T x, T mod) {\n        using U = internal::type_traits::safely_multipliable_t<T>;\n\
    \        U a = mod, b = x, va = 0, vb = 1;\n        while (b) {\n            T\
    \ k = a / b;\n            a -= k * b; va -= k * vb;\n            swap(a, b);\n\
    \            swap(va, vb);\n        }\n        assert(a == 1); // gcd should be\
    \ equal to 1\n        return (va % mod + mod) % mod;\n    }\n}\n"
  dependsOn:
  - library/internal/type_traits.hpp
  isVerificationFile: false
  path: library/math/extended_gcd.hpp
  requiredBy:
  - library/math/modint.hpp
  - library/hashing/multiset_hash.hpp
  timestamp: '2024-10-22 15:04:08+08:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
  - verify/unit_test/math/modint.test.cpp
documentation_of: library/math/extended_gcd.hpp
layout: document
title: Extended GCD
---
