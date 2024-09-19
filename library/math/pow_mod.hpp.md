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
  - icon: ':heavy_check_mark:'
    path: library/math/primality_test.hpp
    title: library/math/primality_test.hpp
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/unit_test/math/modint.test.cpp
    title: verify/unit_test/math/modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/number_theory/primality_test.test.cpp
    title: verify/yosupo/number_theory/primality_test.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"library/math/pow_mod.hpp\"\n\n#include <type_traits>\n#include\
    \ <concepts>\n#include <limits>\n\nnamespace maomao90 {\n    template <integral\
    \ T>\n    constexpr T pow_mod(T b, T p, T mod) {\n        using U = conditional_t<numeric_limits<T>::max()\
    \ <= numeric_limits<unsigned int>::max(), unsigned long long, unsigned __int128>;\n\
    \        T res = 1;\n        b %= mod;\n        if (b < 0) {\n            b +=\
    \ mod;\n        }\n        while (p) {\n            if (p & 1) {\n           \
    \     res = (U) res * b % mod;\n            }\n            b = (U) b * b % mod;\n\
    \            p >>= 1;\n        }\n        return res;\n    }\n}\n"
  code: "#pragma once\n\n#include <type_traits>\n#include <concepts>\n#include <limits>\n\
    \nnamespace maomao90 {\n    template <integral T>\n    constexpr T pow_mod(T b,\
    \ T p, T mod) {\n        using U = conditional_t<numeric_limits<T>::max() <= numeric_limits<unsigned\
    \ int>::max(), unsigned long long, unsigned __int128>;\n        T res = 1;\n \
    \       b %= mod;\n        if (b < 0) {\n            b += mod;\n        }\n  \
    \      while (p) {\n            if (p & 1) {\n                res = (U) res *\
    \ b % mod;\n            }\n            b = (U) b * b % mod;\n            p >>=\
    \ 1;\n        }\n        return res;\n    }\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: library/math/pow_mod.hpp
  requiredBy:
  - library/math/modint.hpp
  - library/math/primality_test.hpp
  - library/hashing/multiset_hash.hpp
  timestamp: '2024-09-19 23:45:16+08:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/unit_test/math/modint.test.cpp
  - verify/yosupo/number_theory/primality_test.test.cpp
documentation_of: library/math/pow_mod.hpp
layout: document
redirect_from:
- /library/library/math/pow_mod.hpp
- /library/library/math/pow_mod.hpp.html
title: library/math/pow_mod.hpp
---
