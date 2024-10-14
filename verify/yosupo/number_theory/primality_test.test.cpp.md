---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: library/math/pow_mod.hpp
    title: library/math/pow_mod.hpp
  - icon: ':heavy_check_mark:'
    path: library/math/primality_test.hpp
    title: library/math/primality_test.hpp
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/primality_test
    links:
    - https://judge.yosupo.jp/problem/primality_test
  bundledCode: "#line 1 \"verify/yosupo/number_theory/primality_test.test.cpp\"\n\
    #define PROBLEM \"https://judge.yosupo.jp/problem/primality_test\"\n\n#include\
    \ <iostream>\n#include <cassert>\n#include <limits>\nusing namespace std;\n\n\
    #line 2 \"library/math/primality_test.hpp\"\n\n#line 2 \"library/math/pow_mod.hpp\"\
    \n\n#include <type_traits>\n#include <concepts>\n#line 6 \"library/math/pow_mod.hpp\"\
    \n\nnamespace maomao90 {\n    template <integral T>\n    constexpr T pow_mod(T\
    \ b, T p, T mod) {\n        using U = conditional_t<numeric_limits<T>::max() <=\
    \ numeric_limits<unsigned int>::max(), unsigned long long, unsigned __int128>;\n\
    \        T res = 1;\n        b %= mod;\n        if (b < 0) {\n            b +=\
    \ mod;\n        }\n        while (p) {\n            if (p & 1) {\n           \
    \     res = (U) res * b % mod;\n            }\n            b = (U) b * b % mod;\n\
    \            p >>= 1;\n        }\n        return res;\n    }\n}\n#line 4 \"library/math/primality_test.hpp\"\
    \n\n#line 8 \"library/math/primality_test.hpp\"\n\nnamespace maomao90 {\n    template\
    \ <integral T>\n    constexpr bool miller_rabin(const T &n, const T *bases, const\
    \ size_t size) {\n        using U = conditional_t<numeric_limits<T>::max() <=\
    \ numeric_limits<unsigned int>::max(), unsigned long long, unsigned __int128>;\n\
    \        if (n <= 1) {\n            return false;\n        }\n        if (n ==\
    \ 2) {\n            return true;\n        }\n        if (n % 2 == 0) {\n     \
    \       return false;\n        }\n        T d = n - 1;\n        while (d % 2 ==\
    \ 0) d /= 2;\n        for (int i = 0; i < size; i++) {\n            T a = bases[i];\n\
    \            if (a % n == 0) {\n                continue;\n            }\n   \
    \         T t = d, y = pow_mod<T>(a, t, n);\n            while (t != n - 1 &&\
    \ y != 1 && y != n - 1) {\n                y = (U) y * y % n;\n              \
    \  t <<= 1;\n            }\n            if (y != n - 1 && t % 2 == 0) {\n    \
    \            return false;\n            }\n        }\n        return true;\n \
    \   }\n    template <integral T>\n    constexpr bool is_prime(T n) {\n       \
    \ constexpr bool is_32bit = numeric_limits<T>::max() <= numeric_limits<unsigned\
    \ int>::max();\n        if constexpr (is_32bit) {\n            T bases[3] = {2,\
    \ 7, 61};\n            return miller_rabin<T>(n, bases, 3);\n        } else {\n\
    \            T bases[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};\n\
    \            return miller_rabin<T>(n, bases, 7);\n        }\n    }\n}\n#line\
    \ 9 \"verify/yosupo/number_theory/primality_test.test.cpp\"\nusing namespace maomao90;\n\
    \nint main() {\n    int q; cin >> q;\n    while (q--) {\n        long long n;\
    \ cin >> n;\n        bool ans = is_prime((unsigned long long) n);\n        if\
    \ (n <= numeric_limits<unsigned int>::max()) {\n            assert(ans == is_prime((unsigned\
    \ int) n));\n        }\n        if (ans) {\n            cout << \"Yes\\n\";\n\
    \        } else {\n            cout << \"No\\n\";\n        }\n    }\n    return\
    \ 0;\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/primality_test\"\n\n#include\
    \ <iostream>\n#include <cassert>\n#include <limits>\nusing namespace std;\n\n\
    #include \"library/math/primality_test.hpp\"\nusing namespace maomao90;\n\nint\
    \ main() {\n    int q; cin >> q;\n    while (q--) {\n        long long n; cin\
    \ >> n;\n        bool ans = is_prime((unsigned long long) n);\n        if (n <=\
    \ numeric_limits<unsigned int>::max()) {\n            assert(ans == is_prime((unsigned\
    \ int) n));\n        }\n        if (ans) {\n            cout << \"Yes\\n\";\n\
    \        } else {\n            cout << \"No\\n\";\n        }\n    }\n    return\
    \ 0;\n}\n"
  dependsOn:
  - library/math/primality_test.hpp
  - library/math/pow_mod.hpp
  isVerificationFile: true
  path: verify/yosupo/number_theory/primality_test.test.cpp
  requiredBy: []
  timestamp: '2024-10-14 14:00:38+08:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/yosupo/number_theory/primality_test.test.cpp
layout: document
redirect_from:
- /verify/verify/yosupo/number_theory/primality_test.test.cpp
- /verify/verify/yosupo/number_theory/primality_test.test.cpp.html
title: verify/yosupo/number_theory/primality_test.test.cpp
---
