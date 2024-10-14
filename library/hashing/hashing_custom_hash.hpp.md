---
data:
  _extendedDependsOn:
  - icon: ':warning:'
    path: library/hashing/internal.hpp
    title: library/hashing/internal.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"library/hashing/hashing_custom_hash.hpp\"\n\n#line 2 \"\
    library/hashing/internal.hpp\"\n\n#include <array>\n#include <random>\n#include\
    \ <chrono>\n\nnamespace maomao90 {\n    namespace internal::hashing {\n      \
    \  const int MIN_HASH_BASE = 128;\n        template <typename mint, size_t num_bases>\n\
    \        constexpr array<mint, num_bases> gen_bases() {\n            mt19937_64\
    \ rng(chrono::high_resolution_clock::now().time_since_epoch().count());\n    \
    \        array<mint, num_bases> res;\n            for (int i = 0; i < num_bases;\
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
    \            return res;\n        }\n    }\n}\n#line 4 \"library/hashing/hashing_custom_hash.hpp\"\
    \n\n#line 6 \"library/hashing/hashing_custom_hash.hpp\"\n#include <concepts>\n\
    \nnamespace maomao90 {\n    template <typename T>\n    concept Hashing = requires(T\
    \ v) {\n        { T::get_num_bases() } -> convertible_to<size_t>;\n        { v.get_v()\
    \ } -> same_as<array<typename T::mint::umod_type, T::get_num_bases()>>;\n    };\n\
    \    template <Hashing T>\n    struct HashingCustomHash {\n        inline static\
    \ const array<unsigned long long, T::get_num_bases()> fixed_random = \n      \
    \      internal::hashing::gen_fixed_random<T::get_num_bases()>();\n        size_t\
    \ operator()(const T &o) const {\n            unsigned long long res = 0;\n  \
    \          auto v = o.get_v();\n            for (int i = 0; i < T::get_num_bases();\
    \ i++) {\n                res ^= splitmix64(v[i] ^ fixed_random[i]);\n       \
    \     }\n            return res;\n        }\n    };\n}\n"
  code: "#pragma once\n\n#include \"library/hashing/internal.hpp\"\n\n#include <array>\n\
    #include <concepts>\n\nnamespace maomao90 {\n    template <typename T>\n    concept\
    \ Hashing = requires(T v) {\n        { T::get_num_bases() } -> convertible_to<size_t>;\n\
    \        { v.get_v() } -> same_as<array<typename T::mint::umod_type, T::get_num_bases()>>;\n\
    \    };\n    template <Hashing T>\n    struct HashingCustomHash {\n        inline\
    \ static const array<unsigned long long, T::get_num_bases()> fixed_random = \n\
    \            internal::hashing::gen_fixed_random<T::get_num_bases()>();\n    \
    \    size_t operator()(const T &o) const {\n            unsigned long long res\
    \ = 0;\n            auto v = o.get_v();\n            for (int i = 0; i < T::get_num_bases();\
    \ i++) {\n                res ^= splitmix64(v[i] ^ fixed_random[i]);\n       \
    \     }\n            return res;\n        }\n    };\n}\n"
  dependsOn:
  - library/hashing/internal.hpp
  isVerificationFile: false
  path: library/hashing/hashing_custom_hash.hpp
  requiredBy: []
  timestamp: '2024-10-14 14:00:38+08:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: library/hashing/hashing_custom_hash.hpp
layout: document
redirect_from:
- /library/library/hashing/hashing_custom_hash.hpp
- /library/library/hashing/hashing_custom_hash.hpp.html
title: library/hashing/hashing_custom_hash.hpp
---
