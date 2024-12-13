---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links:
    - http://xorshift.di.unimi.it/splitmix64.c
  bundledCode: "#line 2 \"library/hashing/splitmix.hpp\"\n\nnamespace maomao90 {\n\
    constexpr unsigned long long splitmix64(unsigned long long x) {\n  // http://xorshift.di.unimi.it/splitmix64.c\n\
    \  x += 0x9e3779b97f4a7c15;\n  x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;\n  x\
    \ = (x ^ (x >> 27)) * 0x94d049bb133111eb;\n  return x ^ (x >> 31);\n}\n} // namespace\
    \ maomao90\n"
  code: "#pragma once\n\nnamespace maomao90 {\nconstexpr unsigned long long splitmix64(unsigned\
    \ long long x) {\n  // http://xorshift.di.unimi.it/splitmix64.c\n  x += 0x9e3779b97f4a7c15;\n\
    \  x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;\n  x = (x ^ (x >> 27)) * 0x94d049bb133111eb;\n\
    \  return x ^ (x >> 31);\n}\n} // namespace maomao90\n"
  dependsOn: []
  isVerificationFile: false
  path: library/hashing/splitmix.hpp
  requiredBy: []
  timestamp: '2024-12-13 23:21:45+08:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: library/hashing/splitmix.hpp
layout: document
title: Splitmix
---