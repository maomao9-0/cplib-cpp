---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: library/data_structure/hashmap.hpp
    title: Hash Map
  - icon: ':heavy_check_mark:'
    path: library/internal/concepts.hpp
    title: library/internal/concepts.hpp
  - icon: ':heavy_check_mark:'
    path: library/internal/hashing.hpp
    title: library/internal/hashing.hpp
  - icon: ':heavy_check_mark:'
    path: library/internal/type_traits.hpp
    title: library/internal/type_traits.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/associative_array
    links:
    - https://judge.yosupo.jp/problem/associative_array
  bundledCode: "#line 1 \"verify/yosupo/data_structure/associative_array.test.cpp\"\
    \n#define PROBLEM \"https://judge.yosupo.jp/problem/associative_array\"\n\n#include\
    \ <iostream>\nusing namespace std;\n\n#line 2 \"library/data_structure/hashmap.hpp\"\
    \n\n#include <bitset>\n#include <vector>\n\n#line 2 \"library/internal/hashing.hpp\"\
    \n\n#include <array>\n#include <chrono>\n#include <random>\n\n#line 2 \"library/internal/concepts.hpp\"\
    \n\n#line 2 \"library/internal/type_traits.hpp\"\n\n#include <limits>\n#include\
    \ <type_traits>\n#include <utility>\n\nnamespace maomao90::internal::type_traits\
    \ {\nusing namespace std;\n#define ENABLE_VALUE(x) template <typename T> constexpr\
    \ bool x##_v = x<T>::value\n#define ENABLE_TYPE(x) template <typename T> using\
    \ x##_t = typename x<T>::type\n\ntemplate <typename T>\nusing is_broadly_signed\
    \ = disjunction<is_signed<T>, is_same<T, __int128>>;\nENABLE_VALUE(is_broadly_signed);\n\
    \ntemplate <typename T>\nusing is_broadly_unsigned =\n    disjunction<is_unsigned<T>,\
    \ is_same<T, unsigned __int128>>;\nENABLE_VALUE(is_broadly_unsigned);\n\ntemplate\
    \ <typename T>\nusing is_broadly_integral = disjunction<is_integral<T>, is_same<T,\
    \ __int128>,\n                                        is_same<T, unsigned __int128>>;\n\
    ENABLE_VALUE(is_broadly_integral);\n\ntemplate <typename T>\nusing is_broadly_signed_integral\
    \ =\n    conjunction<is_broadly_signed<T>, is_broadly_integral<T>>;\nENABLE_VALUE(is_broadly_signed_integral);\n\
    \ntemplate <typename T>\nusing is_broadly_unsigned_integral =\n    conjunction<is_broadly_unsigned<T>,\
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
    };\n} // namespace maomao90::internal::hashing\n#line 7 \"library/data_structure/hashmap.hpp\"\
    \n\nnamespace maomao90 {\nusing namespace std;\ntemplate <typename K, typename\
    \ T,\n          typename Hash = internal::hashing::HashObject<K>,\n          bool\
    \ KEEP_HISTORY = false, int LG = 20>\nstruct HashMap {\n  constexpr T &operator[](const\
    \ K &k) {\n    int index = get_index(k);\n    if (!vis[index]) {\n      vis[index]\
    \ = 1;\n      key[index] = k;\n      value[index] = T();\n      if constexpr (KEEP_HISTORY)\
    \ {\n        history.push_back(index);\n      }\n    }\n    return value[index];\n\
    \  }\n  constexpr bool erase(const K &k) {\n    int index = get_index(k);\n  \
    \  if (!vis[index]) {\n      return 0;\n    }\n    vis[index] = 0;\n    return\
    \ 1;\n  }\n  constexpr bool contains(const K &k) const {\n    int index = get_index(k);\n\
    \    return vis[index];\n  }\n\n  template <typename F>\n    requires requires(F\
    \ f, const K &k, T &v) { f(k, v); }\n  constexpr void for_each(F f) {\n    if\
    \ constexpr (KEEP_HISTORY) {\n      for (int i : history) {\n        if (vis[i])\
    \ {\n          f(key[i], value[i]);\n        }\n      }\n    } else {\n      for\
    \ (int i = 0; i < MOD; i++) {\n        if (vis[i]) {\n          f(key[i], value[i]);\n\
    \        }\n      }\n    }\n  }\n\n  constexpr void clear() {\n    if constexpr\
    \ (KEEP_HISTORY) {\n      for (int i : history) {\n        vis[i] = 0;\n     \
    \ }\n      history.clear();\n    } else {\n      vis.reset();\n    }\n  }\n\n\
    private:\n  static constexpr int MOD = 1 << LG;\n  bitset<MOD> vis;\n  K key[MOD];\n\
    \  T value[MOD];\n  vector<int> history;\n\n  constexpr int get_index(const K\
    \ &k) const {\n    unsigned long long hash = Hash()(k);\n    int index = hash\
    \ >> (64 - LG);\n    while (vis[index] && key[index] != k) {\n      index = (index\
    \ + 1) & (MOD - 1);\n    }\n    return index;\n  }\n};\n} // namespace maomao90\n\
    #line 7 \"verify/yosupo/data_structure/associative_array.test.cpp\"\nusing namespace\
    \ maomao90;\n\nint main() {\n  ios::sync_with_stdio(0), cin.tie(0);\n  HashMap<long\
    \ long, long long> hash_map;\n  int q;\n  cin >> q;\n  while (q--) {\n    int\
    \ t;\n    cin >> t;\n    if (t == 0) {\n      long long k, v;\n      cin >> k\
    \ >> v;\n      hash_map[k] = v;\n    } else {\n      long long k;\n      cin >>\
    \ k;\n      cout << hash_map[k] << '\\n';\n    }\n  }\n  return 0;\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/associative_array\"\n\n\
    #include <iostream>\nusing namespace std;\n\n#include \"library/data_structure/hashmap.hpp\"\
    \nusing namespace maomao90;\n\nint main() {\n  ios::sync_with_stdio(0), cin.tie(0);\n\
    \  HashMap<long long, long long> hash_map;\n  int q;\n  cin >> q;\n  while (q--)\
    \ {\n    int t;\n    cin >> t;\n    if (t == 0) {\n      long long k, v;\n   \
    \   cin >> k >> v;\n      hash_map[k] = v;\n    } else {\n      long long k;\n\
    \      cin >> k;\n      cout << hash_map[k] << '\\n';\n    }\n  }\n  return 0;\n\
    }\n"
  dependsOn:
  - library/data_structure/hashmap.hpp
  - library/internal/hashing.hpp
  - library/internal/concepts.hpp
  - library/internal/type_traits.hpp
  isVerificationFile: true
  path: verify/yosupo/data_structure/associative_array.test.cpp
  requiredBy: []
  timestamp: '2024-12-13 23:21:45+08:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/yosupo/data_structure/associative_array.test.cpp
layout: document
redirect_from:
- /verify/verify/yosupo/data_structure/associative_array.test.cpp
- /verify/verify/yosupo/data_structure/associative_array.test.cpp.html
title: verify/yosupo/data_structure/associative_array.test.cpp
---
