---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: library/math/multiplicative_function.hpp
    title: Multiplicative Function
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
    title: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"library/math/poly.hpp\"\n\n#include <vector>\n\nnamespace\
    \ maomao90 {\n    template <typename T>\n    struct Poly {\n        constexpr\
    \ Poly(): _v(1, 0) {}\n        constexpr Poly(int n): _v(n) {}\n        constexpr\
    \ Poly(vector<T> v): _v(v) {}\n\n        constexpr int degree() const {\n    \
    \        return _v.size() - 1;\n        }\n        constexpr T& operator[](int\
    \ i) {\n            return _v[i];\n        }\n    private:\n        vector<T>\
    \ _v;\n    };\n}\n"
  code: "#pragma once\n\n#include <vector>\n\nnamespace maomao90 {\n    template <typename\
    \ T>\n    struct Poly {\n        constexpr Poly(): _v(1, 0) {}\n        constexpr\
    \ Poly(int n): _v(n) {}\n        constexpr Poly(vector<T> v): _v(v) {}\n\n   \
    \     constexpr int degree() const {\n            return _v.size() - 1;\n    \
    \    }\n        constexpr T& operator[](int i) {\n            return _v[i];\n\
    \        }\n    private:\n        vector<T> _v;\n    };\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: library/math/poly.hpp
  requiredBy:
  - library/math/multiplicative_function.hpp
  timestamp: '2024-10-22 15:04:08+08:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
documentation_of: library/math/poly.hpp
layout: document
title: Poly
---
