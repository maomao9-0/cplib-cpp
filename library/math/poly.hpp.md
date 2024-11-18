---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
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
  requiredBy: []
  timestamp: '2024-11-18 20:19:33+08:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: library/math/poly.hpp
layout: document
title: Poly
---
