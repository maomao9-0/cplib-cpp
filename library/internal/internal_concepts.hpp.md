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
    path: library/math/multiplicative_function.hpp
    title: library/math/multiplicative_function.hpp
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
  bundledCode: "#line 2 \"library/internal/internal_concepts.hpp\"\n\n#include <concepts>\n\
    \nnamespace internal::concepts {\n    template <typename T>\n    concept Addable\
    \ = requires (T a) {\n        a + a;\n        a += a;\n    };\n    template <typename\
    \ T>\n    concept Subtractable = requires (T a) {\n        a - a;\n        a -=\
    \ a;\n    };\n    template <typename T>\n    concept Multipliable = requires (T\
    \ a) {\n        a * a;\n        a *= a;\n    };\n    template <typename T>\n \
    \   concept Dividable =  requires (T a) {\n        a / a;\n        a /= a;\n \
    \   };\n}\n"
  code: "#pragma once\n\n#include <concepts>\n\nnamespace internal::concepts {\n \
    \   template <typename T>\n    concept Addable = requires (T a) {\n        a +\
    \ a;\n        a += a;\n    };\n    template <typename T>\n    concept Subtractable\
    \ = requires (T a) {\n        a - a;\n        a -= a;\n    };\n    template <typename\
    \ T>\n    concept Multipliable = requires (T a) {\n        a * a;\n        a *=\
    \ a;\n    };\n    template <typename T>\n    concept Dividable =  requires (T\
    \ a) {\n        a / a;\n        a /= a;\n    };\n}\n"
  dependsOn: []
  isVerificationFile: false
  path: library/internal/internal_concepts.hpp
  requiredBy:
  - library/math/multiplicative_function.hpp
  - library/math/modint.hpp
  - library/hashing/multiset_hash.hpp
  timestamp: '2024-10-14 14:00:38+08:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
  - verify/unit_test/math/modint.test.cpp
documentation_of: library/internal/internal_concepts.hpp
layout: document
redirect_from:
- /library/library/internal/internal_concepts.hpp
- /library/library/internal/internal_concepts.hpp.html
title: library/internal/internal_concepts.hpp
---
