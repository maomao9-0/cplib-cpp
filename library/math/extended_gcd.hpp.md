---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':warning:'
    path: library/hashing/multiset_hash.hpp
    title: library/hashing/multiset_hash.hpp
  - icon: ':heavy_check_mark:'
    path: library/math/modint.hpp
    title: library/math/modint.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/unit_test/math/modint.test.cpp
    title: verify/unit_test/math/modint.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"library/math/extended_gcd.hpp\"\n\n#include <cassert>\n\
    #include <type_traits>\n#include <concepts>\n#include <limits>\n\nnamespace maomao90\
    \ {\n    template <signed_integral T>\n    constexpr T inv_gcd(T x, T mod) {\n\
    \        using U = conditional_t<numeric_limits<T>::max() <= numeric_limits<int>::max(),\
    \ long long, __int128>;\n        U a = mod, b = x, va = 0, vb = 1;\n        while\
    \ (b) {\n            T k = a / b;\n            a -= k * b; va -= k * vb;\n   \
    \         swap(a, b);\n            swap(va, vb);\n        }\n        assert(a\
    \ == 1); // gcd should be equal to 1\n        return (va % mod + mod) % mod;\n\
    \    }\n}\n"
  code: "#pragma once\n\n#include <cassert>\n#include <type_traits>\n#include <concepts>\n\
    #include <limits>\n\nnamespace maomao90 {\n    template <signed_integral T>\n\
    \    constexpr T inv_gcd(T x, T mod) {\n        using U = conditional_t<numeric_limits<T>::max()\
    \ <= numeric_limits<int>::max(), long long, __int128>;\n        U a = mod, b =\
    \ x, va = 0, vb = 1;\n        while (b) {\n            T k = a / b;\n        \
    \    a -= k * b; va -= k * vb;\n            swap(a, b);\n            swap(va,\
    \ vb);\n        }\n        assert(a == 1); // gcd should be equal to 1\n     \
    \   return (va % mod + mod) % mod;\n    }\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: library/math/extended_gcd.hpp
  requiredBy:
  - library/math/modint.hpp
  - library/hashing/multiset_hash.hpp
  timestamp: '2024-09-19 23:45:16+08:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/unit_test/math/modint.test.cpp
documentation_of: library/math/extended_gcd.hpp
layout: document
redirect_from:
- /library/library/math/extended_gcd.hpp
- /library/library/math/extended_gcd.hpp.html
title: library/math/extended_gcd.hpp
---
