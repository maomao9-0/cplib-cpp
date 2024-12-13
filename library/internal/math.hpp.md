---
data:
  _extendedDependsOn: []
  _extendedRequiredBy:
  - icon: ':warning:'
    path: library/hashing/hashing_custom_hash.hpp
    title: Hashing Custom Hash
  - icon: ':warning:'
    path: library/hashing/multiset_hash.hpp
    title: Multiset Hash
  - icon: ':heavy_check_mark:'
    path: library/math/modint.hpp
    title: ModInt
  - icon: ':heavy_check_mark:'
    path: library/math/poly.hpp
    title: Poly
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/unit_test/math/modint.test.cpp
    title: verify/unit_test/math/modint.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/convolution/convolution.test.cpp
    title: verify/yosupo/convolution/convolution.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/convolution/convolution_1000000007.test.cpp
    title: verify/yosupo/convolution/convolution_1000000007.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/convolution/convolution_fft.test.cpp
    title: verify/yosupo/convolution/convolution_fft.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/convolution/convolution_fft_1000000007.test.cpp
    title: verify/yosupo/convolution/convolution_fft_1000000007.test.cpp
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
    title: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"library/internal/math.hpp\"\n\nnamespace maomao90::internal::math\
    \ {\nconstexpr long long pow_mod_constexpr(long long x, long long n, int m) {\n\
    \  if (m == 1)\n    return 0;\n  unsigned int _m = (unsigned int)(m);\n  unsigned\
    \ long long r = 1;\n  unsigned long long y = x < 0 ? x % m + m : x % m;\n  while\
    \ (n) {\n    if (n & 1)\n      r = (r * y) % _m;\n    y = (y * y) % _m;\n    n\
    \ >>= 1;\n  }\n  return r;\n}\n// Compile time primitive root\n// @param m must\
    \ be prime\n// @return primitive root (and minimum in now)\nconstexpr int primitive_root_constexpr(int\
    \ m) {\n  if (m == 2)\n    return 1;\n  if (m == 167772161)\n    return 3;\n \
    \ if (m == 469762049)\n    return 3;\n  if (m == 754974721)\n    return 11;\n\
    \  if (m == 998244353)\n    return 3;\n  int divs[20] = {};\n  divs[0] = 2;\n\
    \  int cnt = 1;\n  int x = (m - 1) / 2;\n  while (x % 2 == 0)\n    x /= 2;\n \
    \ for (int i = 3; (long long)(i)*i <= x; i += 2) {\n    if (x % i == 0) {\n  \
    \    divs[cnt++] = i;\n      while (x % i == 0) {\n        x /= i;\n      }\n\
    \    }\n  }\n  if (x > 1) {\n    divs[cnt++] = x;\n  }\n  for (int g = 2;; g++)\
    \ {\n    bool ok = true;\n    for (int i = 0; i < cnt; i++) {\n      if (pow_mod_constexpr(g,\
    \ (m - 1) / divs[i], m) == 1) {\n        ok = false;\n        break;\n      }\n\
    \    }\n    if (ok)\n      return g;\n  }\n}\ntemplate <int m> constexpr int primitive_root\
    \ = primitive_root_constexpr(m);\n\nstruct barrett {\n  unsigned int _m;\n  unsigned\
    \ long long im;\n\n  explicit barrett(unsigned int m)\n      : _m(m), im((unsigned\
    \ long long)(-1) / m + 1) {}\n\n  unsigned int umod() const { return _m; }\n\n\
    \  unsigned int mul(unsigned int a, unsigned int b) const {\n    // [1] m = 1\n\
    \    // a = b = im = 0, so okay\n\n    // [2] m >= 2\n    // im = ceil(2^64 /\
    \ m)\n    // -> im * m = 2^64 + r (0 <= r < m)\n    // let z = a*b = c*m + d (0\
    \ <= c, d < m)\n    // a*b * im = (c*m + d) * im = c*(im*m) + d*im = c*2^64 +\
    \ c*r + d*im\n    // c*r + d*im < m * m + m * im < m * m + 2^64 + m <= 2^64 +\
    \ m * (m + 1) <\n    // 2^64 * 2\n    // ((ab * im) >> 64) == c or c + 1\n   \
    \ unsigned long long z = a;\n    z *= b;\n#ifdef _MSC_VER\n    unsigned long long\
    \ x;\n    _umul128(z, im, &x);\n#else\n    unsigned long long x =\n        (unsigned\
    \ long long)(((unsigned __int128)(z)*im) >> 64);\n#endif\n    unsigned long long\
    \ y = x * _m;\n    return (unsigned int)(z - y + (z < y ? _m : 0));\n  }\n};\n\
    } // namespace maomao90::internal::math\n"
  code: "#pragma once\n\nnamespace maomao90::internal::math {\nconstexpr long long\
    \ pow_mod_constexpr(long long x, long long n, int m) {\n  if (m == 1)\n    return\
    \ 0;\n  unsigned int _m = (unsigned int)(m);\n  unsigned long long r = 1;\n  unsigned\
    \ long long y = x < 0 ? x % m + m : x % m;\n  while (n) {\n    if (n & 1)\n  \
    \    r = (r * y) % _m;\n    y = (y * y) % _m;\n    n >>= 1;\n  }\n  return r;\n\
    }\n// Compile time primitive root\n// @param m must be prime\n// @return primitive\
    \ root (and minimum in now)\nconstexpr int primitive_root_constexpr(int m) {\n\
    \  if (m == 2)\n    return 1;\n  if (m == 167772161)\n    return 3;\n  if (m ==\
    \ 469762049)\n    return 3;\n  if (m == 754974721)\n    return 11;\n  if (m ==\
    \ 998244353)\n    return 3;\n  int divs[20] = {};\n  divs[0] = 2;\n  int cnt =\
    \ 1;\n  int x = (m - 1) / 2;\n  while (x % 2 == 0)\n    x /= 2;\n  for (int i\
    \ = 3; (long long)(i)*i <= x; i += 2) {\n    if (x % i == 0) {\n      divs[cnt++]\
    \ = i;\n      while (x % i == 0) {\n        x /= i;\n      }\n    }\n  }\n  if\
    \ (x > 1) {\n    divs[cnt++] = x;\n  }\n  for (int g = 2;; g++) {\n    bool ok\
    \ = true;\n    for (int i = 0; i < cnt; i++) {\n      if (pow_mod_constexpr(g,\
    \ (m - 1) / divs[i], m) == 1) {\n        ok = false;\n        break;\n      }\n\
    \    }\n    if (ok)\n      return g;\n  }\n}\ntemplate <int m> constexpr int primitive_root\
    \ = primitive_root_constexpr(m);\n\nstruct barrett {\n  unsigned int _m;\n  unsigned\
    \ long long im;\n\n  explicit barrett(unsigned int m)\n      : _m(m), im((unsigned\
    \ long long)(-1) / m + 1) {}\n\n  unsigned int umod() const { return _m; }\n\n\
    \  unsigned int mul(unsigned int a, unsigned int b) const {\n    // [1] m = 1\n\
    \    // a = b = im = 0, so okay\n\n    // [2] m >= 2\n    // im = ceil(2^64 /\
    \ m)\n    // -> im * m = 2^64 + r (0 <= r < m)\n    // let z = a*b = c*m + d (0\
    \ <= c, d < m)\n    // a*b * im = (c*m + d) * im = c*(im*m) + d*im = c*2^64 +\
    \ c*r + d*im\n    // c*r + d*im < m * m + m * im < m * m + 2^64 + m <= 2^64 +\
    \ m * (m + 1) <\n    // 2^64 * 2\n    // ((ab * im) >> 64) == c or c + 1\n   \
    \ unsigned long long z = a;\n    z *= b;\n#ifdef _MSC_VER\n    unsigned long long\
    \ x;\n    _umul128(z, im, &x);\n#else\n    unsigned long long x =\n        (unsigned\
    \ long long)(((unsigned __int128)(z)*im) >> 64);\n#endif\n    unsigned long long\
    \ y = x * _m;\n    return (unsigned int)(z - y + (z < y ? _m : 0));\n  }\n};\n\
    } // namespace maomao90::internal::math\n"
  dependsOn: []
  isVerificationFile: false
  path: library/internal/math.hpp
  requiredBy:
  - library/math/poly.hpp
  - library/math/modint.hpp
  - library/hashing/multiset_hash.hpp
  - library/hashing/hashing_custom_hash.hpp
  timestamp: '2024-12-13 23:21:45+08:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
  - verify/yosupo/convolution/convolution_1000000007.test.cpp
  - verify/yosupo/convolution/convolution_fft.test.cpp
  - verify/yosupo/convolution/convolution.test.cpp
  - verify/yosupo/convolution/convolution_fft_1000000007.test.cpp
  - verify/unit_test/math/modint.test.cpp
documentation_of: library/internal/math.hpp
layout: document
redirect_from:
- /library/library/internal/math.hpp
- /library/library/internal/math.hpp.html
title: library/internal/math.hpp
---
