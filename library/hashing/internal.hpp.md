---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':warning:'
    path: library/hashing/hashing_custom_hash.hpp
    title: library/hashing/hashing_custom_hash.hpp
  - icon: ':warning:'
    path: library/hashing/multiset_hash.hpp
    title: library/hashing/multiset_hash.hpp
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"library/hashing/internal.hpp\"\n\n#include <array>\n#include\
    \ <random>\n#include <chrono>\n\nnamespace maomao90 {\n    namespace internal::hashing\
    \ {\n        const int MIN_HASH_BASE = 128;\n        template <typename mint,\
    \ size_t num_bases>\n        constexpr array<mint, num_bases> gen_bases() {\n\
    \            mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());\n\
    \            array<mint, num_bases> res;\n            for (int i = 0; i < num_bases;\
    \ i++) {\n                res[i] = mint::raw(rng() % (mint::umod() - MIN_HASH_BASE)\
    \ + MIN_HASH_BASE);\n            }\n            return res;\n        }\n     \
    \   template <typename mint, size_t num_bases>\n        constexpr array<mint,\
    \ num_bases> gen_inverse(const array<mint, num_bases> &bases) {\n            array<mint,\
    \ num_bases> res;\n            for (int i = 0; i < num_bases; i++) {\n       \
    \         res[i] = bases[i].inv();\n            }\n            return res;\n \
    \       }\n        template <typename mint, size_t num_bases, size_t CACHE>\n\
    \        constexpr array<array<mint, CACHE>, num_bases> init_power(const array<mint,\
    \ num_bases> &bases) {\n            array<array<mint, CACHE>, num_bases> res;\n\
    \            for (int i = 0; i < num_bases; i++) {\n                res[i][0]\
    \ = 1;\n                for (int j = 1; j < CACHE; j++) {\n                  \
    \  res[i][j] = res[i][j - 1] * bases[i];\n                }\n            }\n \
    \           return res;\n        }\n        template <size_t num_bases>\n    \
    \    constexpr array<unsigned long long, num_bases> gen_fixed_random() {\n   \
    \         mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());\n\
    \            array<unsigned long long, num_bases> res;\n            for (int i\
    \ = 0; i < num_bases; i++) {\n                res[i] = rng();\n            }\n\
    \            return res;\n        }\n    }\n}\n"
  code: "#pragma once\n\n#include <array>\n#include <random>\n#include <chrono>\n\n\
    namespace maomao90 {\n    namespace internal::hashing {\n        const int MIN_HASH_BASE\
    \ = 128;\n        template <typename mint, size_t num_bases>\n        constexpr\
    \ array<mint, num_bases> gen_bases() {\n            mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());\n\
    \            array<mint, num_bases> res;\n            for (int i = 0; i < num_bases;\
    \ i++) {\n                res[i] = mint::raw(rng() % (mint::umod() - MIN_HASH_BASE)\
    \ + MIN_HASH_BASE);\n            }\n            return res;\n        }\n     \
    \   template <typename mint, size_t num_bases>\n        constexpr array<mint,\
    \ num_bases> gen_inverse(const array<mint, num_bases> &bases) {\n            array<mint,\
    \ num_bases> res;\n            for (int i = 0; i < num_bases; i++) {\n       \
    \         res[i] = bases[i].inv();\n            }\n            return res;\n \
    \       }\n        template <typename mint, size_t num_bases, size_t CACHE>\n\
    \        constexpr array<array<mint, CACHE>, num_bases> init_power(const array<mint,\
    \ num_bases> &bases) {\n            array<array<mint, CACHE>, num_bases> res;\n\
    \            for (int i = 0; i < num_bases; i++) {\n                res[i][0]\
    \ = 1;\n                for (int j = 1; j < CACHE; j++) {\n                  \
    \  res[i][j] = res[i][j - 1] * bases[i];\n                }\n            }\n \
    \           return res;\n        }\n        template <size_t num_bases>\n    \
    \    constexpr array<unsigned long long, num_bases> gen_fixed_random() {\n   \
    \         mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());\n\
    \            array<unsigned long long, num_bases> res;\n            for (int i\
    \ = 0; i < num_bases; i++) {\n                res[i] = rng();\n            }\n\
    \            return res;\n        }\n    }\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: library/hashing/internal.hpp
  requiredBy:
  - library/hashing/hashing_custom_hash.hpp
  - library/hashing/multiset_hash.hpp
  timestamp: '2024-09-19 23:45:16+08:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: library/hashing/internal.hpp
layout: document
redirect_from:
- /library/library/hashing/internal.hpp
- /library/library/hashing/internal.hpp.html
title: library/hashing/internal.hpp
---
