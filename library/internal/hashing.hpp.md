---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: library/internal/concepts.hpp
    title: library/internal/concepts.hpp
  - icon: ':heavy_check_mark:'
    path: library/internal/type_traits.hpp
    title: library/internal/type_traits.hpp
  _extendedRequiredBy:
  - icon: ':heavy_check_mark:'
    path: library/data_structure/hashmap.hpp
    title: Hash Map
  - icon: ':warning:'
    path: library/hashing/hashing_custom_hash.hpp
    title: Hashing Custom Hash
  - icon: ':warning:'
    path: library/hashing/multiset_hash.hpp
    title: Multiset Hash
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/data_structure/associative_array.test.cpp
    title: verify/yosupo/data_structure/associative_array.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"library/internal/hashing.hpp\"\n\n#include <array>\n#include\
    \ <chrono>\n#include <random>\n\n#line 2 \"library/internal/concepts.hpp\"\n\n\
    #line 2 \"library/internal/type_traits.hpp\"\n\n#include <limits>\n#include <type_traits>\n\
    #include <utility>\n\nnamespace maomao90::internal::type_traits {\nusing namespace\
    \ std;\n#define ENABLE_VALUE(x) template <typename T> constexpr bool x##_v = x<T>::value\n\
    #define ENABLE_TYPE(x) template <typename T> using x##_t = typename x<T>::type\n\
    \ntemplate <typename T>\nusing is_broadly_signed = disjunction<is_signed<T>, is_same<T,\
    \ __int128>>;\nENABLE_VALUE(is_broadly_signed);\n\ntemplate <typename T>\nusing\
    \ is_broadly_unsigned =\n    disjunction<is_unsigned<T>, is_same<T, unsigned __int128>>;\n\
    ENABLE_VALUE(is_broadly_unsigned);\n\ntemplate <typename T>\nusing is_broadly_integral\
    \ = disjunction<is_integral<T>, is_same<T, __int128>,\n                      \
    \                  is_same<T, unsigned __int128>>;\nENABLE_VALUE(is_broadly_integral);\n\
    \ntemplate <typename T>\nusing is_broadly_signed_integral =\n    conjunction<is_broadly_signed<T>,\
    \ is_broadly_integral<T>>;\nENABLE_VALUE(is_broadly_signed_integral);\n\ntemplate\
    \ <typename T>\nusing is_broadly_unsigned_integral =\n    conjunction<is_broadly_unsigned<T>,\
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
    #line 4 \"library/internal/concepts.hpp\"\n\nnamespace maomao90::internal::concepts\
    \ {\nusing namespace std;\ntemplate <typename T>\nconcept broadly_integral = internal::type_traits::is_broadly_integral_v<T>;\n\
    \ntemplate <typename T>\nconcept broadly_signed_integral =\n    internal::type_traits::is_broadly_signed_integral_v<T>;\n\
    \ntemplate <typename T>\nconcept broadly_unsigned_integral =\n    internal::type_traits::is_broadly_unsigned_integral_v<T>;\n\
    \ntemplate <typename T>\nconcept Addable = requires(T a) {\n  a + a;\n  a += a;\n\
    };\ntemplate <typename T>\nconcept Subtractable = requires(T a) {\n  a - a;\n\
    \  a -= a;\n};\ntemplate <typename T>\nconcept Multipliable = requires(T a) {\n\
    \  a *a;\n  a *= a;\n};\ntemplate <typename T>\nconcept Dividable = requires(T\
    \ a) {\n  a / a;\n  a /= a;\n};\n\ntemplate <typename T>\nconcept Iterable = requires(T\
    \ &t) {\n  begin(t) != end(t);\n  ++std::declval<decltype(begin(t)) &>();\n  *begin(t);\n\
    };\n} // namespace maomao90::internal::concepts\n#line 9 \"library/internal/hashing.hpp\"\
    \n\nnamespace maomao90::internal::hashing {\nusing namespace std;\nconst int MIN_HASH_BASE\
    \ = 128;\nstatic mt19937_64\n    rng(chrono::high_resolution_clock::now().time_since_epoch().count());\n\
    template <typename mint, size_t num_bases>\nconstexpr array<mint, num_bases> gen_bases()\
    \ {\n  array<mint, num_bases> res;\n  for (int i = 0; i < num_bases; i++) {\n\
    \    res[i] = mint::raw(rng() % (mint::umod() - MIN_HASH_BASE) + MIN_HASH_BASE);\n\
    \  }\n  return res;\n}\ntemplate <typename mint, size_t num_bases>\nconstexpr\
    \ array<mint, num_bases>\ngen_inverse(const array<mint, num_bases> &bases) {\n\
    \  array<mint, num_bases> res;\n  for (int i = 0; i < num_bases; i++) {\n    res[i]\
    \ = bases[i].inv();\n  }\n  return res;\n}\ntemplate <typename mint, size_t num_bases,\
    \ size_t CACHE>\nconstexpr array<array<mint, CACHE>, num_bases>\ninit_power(const\
    \ array<mint, num_bases> &bases) {\n  array<array<mint, CACHE>, num_bases> res;\n\
    \  for (int i = 0; i < num_bases; i++) {\n    res[i][0] = 1;\n    for (int j =\
    \ 1; j < CACHE; j++) {\n      res[i][j] = res[i][j - 1] * bases[i];\n    }\n \
    \ }\n  return res;\n}\n\ntemplate <typename T> unsigned long long hash_function(const\
    \ T &x) {\n  static unsigned long long r =\n      chrono::high_resolution_clock::now().time_since_epoch().count();\n\
    \  constexpr unsigned long long z1 = 11995408973635179863ull;\n  if constexpr\
    \ (internal::type_traits::is_broadly_integral_v<T>) {\n    return ((unsigned long\
    \ long)x ^ r) * z1;\n  } else if constexpr (internal::type_traits::is_pair_v<T>)\
    \ {\n    constexpr unsigned long long z2 = 10150724397891781847ull;\n    return\
    \ hash_function(x.first) + hash_function(x.second) * z2;\n  } else if constexpr\
    \ (internal::concepts::Iterable<T>) {\n    constexpr unsigned long long mod =\
    \ (1ll << 61) - 1;\n    constexpr unsigned long long base = 950699498548472943ull;\n\
    \    unsigned long long m = 0;\n    for (auto &i : x) {\n      unsigned long long\
    \ v = hash_function(i);\n      unsigned __int128 r = (unsigned __int128)m * base\
    \ + (v & mod);\n      m = (r & mod) + (r >> 61);\n      if (m >= mod) {\n    \
    \    m -= mod;\n      }\n    }\n    m ^= m << 24;\n    m ^= m >> 31;\n    m ^=\
    \ m << 35;\n    return m;\n  }\n}\n\ntemplate <typename T> struct HashObject {\n\
    \  constexpr size_t operator()(const T &o) const { return hash_function(o); }\n\
    };\n} // namespace maomao90::internal::hashing\n"
  code: "#pragma once\n\n#include <array>\n#include <chrono>\n#include <random>\n\n\
    #include \"library/internal/concepts.hpp\"\n#include \"library/internal/type_traits.hpp\"\
    \n\nnamespace maomao90::internal::hashing {\nusing namespace std;\nconst int MIN_HASH_BASE\
    \ = 128;\nstatic mt19937_64\n    rng(chrono::high_resolution_clock::now().time_since_epoch().count());\n\
    template <typename mint, size_t num_bases>\nconstexpr array<mint, num_bases> gen_bases()\
    \ {\n  array<mint, num_bases> res;\n  for (int i = 0; i < num_bases; i++) {\n\
    \    res[i] = mint::raw(rng() % (mint::umod() - MIN_HASH_BASE) + MIN_HASH_BASE);\n\
    \  }\n  return res;\n}\ntemplate <typename mint, size_t num_bases>\nconstexpr\
    \ array<mint, num_bases>\ngen_inverse(const array<mint, num_bases> &bases) {\n\
    \  array<mint, num_bases> res;\n  for (int i = 0; i < num_bases; i++) {\n    res[i]\
    \ = bases[i].inv();\n  }\n  return res;\n}\ntemplate <typename mint, size_t num_bases,\
    \ size_t CACHE>\nconstexpr array<array<mint, CACHE>, num_bases>\ninit_power(const\
    \ array<mint, num_bases> &bases) {\n  array<array<mint, CACHE>, num_bases> res;\n\
    \  for (int i = 0; i < num_bases; i++) {\n    res[i][0] = 1;\n    for (int j =\
    \ 1; j < CACHE; j++) {\n      res[i][j] = res[i][j - 1] * bases[i];\n    }\n \
    \ }\n  return res;\n}\n\ntemplate <typename T> unsigned long long hash_function(const\
    \ T &x) {\n  static unsigned long long r =\n      chrono::high_resolution_clock::now().time_since_epoch().count();\n\
    \  constexpr unsigned long long z1 = 11995408973635179863ull;\n  if constexpr\
    \ (internal::type_traits::is_broadly_integral_v<T>) {\n    return ((unsigned long\
    \ long)x ^ r) * z1;\n  } else if constexpr (internal::type_traits::is_pair_v<T>)\
    \ {\n    constexpr unsigned long long z2 = 10150724397891781847ull;\n    return\
    \ hash_function(x.first) + hash_function(x.second) * z2;\n  } else if constexpr\
    \ (internal::concepts::Iterable<T>) {\n    constexpr unsigned long long mod =\
    \ (1ll << 61) - 1;\n    constexpr unsigned long long base = 950699498548472943ull;\n\
    \    unsigned long long m = 0;\n    for (auto &i : x) {\n      unsigned long long\
    \ v = hash_function(i);\n      unsigned __int128 r = (unsigned __int128)m * base\
    \ + (v & mod);\n      m = (r & mod) + (r >> 61);\n      if (m >= mod) {\n    \
    \    m -= mod;\n      }\n    }\n    m ^= m << 24;\n    m ^= m >> 31;\n    m ^=\
    \ m << 35;\n    return m;\n  }\n}\n\ntemplate <typename T> struct HashObject {\n\
    \  constexpr size_t operator()(const T &o) const { return hash_function(o); }\n\
    };\n} // namespace maomao90::internal::hashing\n"
  dependsOn:
  - library/internal/concepts.hpp
  - library/internal/type_traits.hpp
  isVerificationFile: false
  path: library/internal/hashing.hpp
  requiredBy:
  - library/data_structure/hashmap.hpp
  - library/hashing/multiset_hash.hpp
  - library/hashing/hashing_custom_hash.hpp
  timestamp: '2024-11-18 20:19:33+08:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/data_structure/associative_array.test.cpp
documentation_of: library/internal/hashing.hpp
layout: document
redirect_from:
- /library/library/internal/hashing.hpp
- /library/library/internal/hashing.hpp.html
title: library/internal/hashing.hpp
---
