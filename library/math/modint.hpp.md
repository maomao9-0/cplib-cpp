---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: library/internal/concepts.hpp
    title: library/internal/concepts.hpp
  - icon: ':heavy_check_mark:'
    path: library/internal/math.hpp
    title: library/internal/math.hpp
  - icon: ':heavy_check_mark:'
    path: library/internal/type_traits.hpp
    title: library/internal/type_traits.hpp
  - icon: ':heavy_check_mark:'
    path: library/math/extended_gcd.hpp
    title: Extended GCD
  - icon: ':heavy_check_mark:'
    path: library/math/pow_mod.hpp
    title: Pow Mod
  - icon: ':heavy_check_mark:'
    path: library/math/primality_test.hpp
    title: Primality Test
  _extendedRequiredBy:
  - icon: ':warning:'
    path: library/hashing/hashing_custom_hash.hpp
    title: Hashing Custom Hash
  - icon: ':warning:'
    path: library/hashing/multiset_hash.hpp
    title: Multiset Hash
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
  bundledCode: "#line 2 \"library/math/modint.hpp\"\n\n#include <cassert>\n#include\
    \ <concepts>\n#include <iostream>\n#include <type_traits>\n\n#line 2 \"library/internal/concepts.hpp\"\
    \n\n#line 2 \"library/internal/type_traits.hpp\"\n\n#include <limits>\n#line 5\
    \ \"library/internal/type_traits.hpp\"\n#include <utility>\n\nnamespace maomao90::internal::type_traits\
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
    };\n} // namespace maomao90::internal::concepts\n#line 2 \"library/internal/math.hpp\"\
    \n\nnamespace maomao90::internal::math {\nconstexpr long long pow_mod_constexpr(long\
    \ long x, long long n, int m) {\n  if (m == 1)\n    return 0;\n  unsigned int\
    \ _m = (unsigned int)(m);\n  unsigned long long r = 1;\n  unsigned long long y\
    \ = x < 0 ? x % m + m : x % m;\n  while (n) {\n    if (n & 1)\n      r = (r *\
    \ y) % _m;\n    y = (y * y) % _m;\n    n >>= 1;\n  }\n  return r;\n}\n// Compile\
    \ time primitive root\n// @param m must be prime\n// @return primitive root (and\
    \ minimum in now)\nconstexpr int primitive_root_constexpr(int m) {\n  if (m ==\
    \ 2)\n    return 1;\n  if (m == 167772161)\n    return 3;\n  if (m == 469762049)\n\
    \    return 3;\n  if (m == 754974721)\n    return 11;\n  if (m == 998244353)\n\
    \    return 3;\n  int divs[20] = {};\n  divs[0] = 2;\n  int cnt = 1;\n  int x\
    \ = (m - 1) / 2;\n  while (x % 2 == 0)\n    x /= 2;\n  for (int i = 3; (long long)(i)*i\
    \ <= x; i += 2) {\n    if (x % i == 0) {\n      divs[cnt++] = i;\n      while\
    \ (x % i == 0) {\n        x /= i;\n      }\n    }\n  }\n  if (x > 1) {\n    divs[cnt++]\
    \ = x;\n  }\n  for (int g = 2;; g++) {\n    bool ok = true;\n    for (int i =\
    \ 0; i < cnt; i++) {\n      if (pow_mod_constexpr(g, (m - 1) / divs[i], m) ==\
    \ 1) {\n        ok = false;\n        break;\n      }\n    }\n    if (ok)\n   \
    \   return g;\n  }\n}\ntemplate <int m> constexpr int primitive_root = primitive_root_constexpr(m);\n\
    \nstruct barrett {\n  unsigned int _m;\n  unsigned long long im;\n\n  explicit\
    \ barrett(unsigned int m)\n      : _m(m), im((unsigned long long)(-1) / m + 1)\
    \ {}\n\n  unsigned int umod() const { return _m; }\n\n  unsigned int mul(unsigned\
    \ int a, unsigned int b) const {\n    // [1] m = 1\n    // a = b = im = 0, so\
    \ okay\n\n    // [2] m >= 2\n    // im = ceil(2^64 / m)\n    // -> im * m = 2^64\
    \ + r (0 <= r < m)\n    // let z = a*b = c*m + d (0 <= c, d < m)\n    // a*b *\
    \ im = (c*m + d) * im = c*(im*m) + d*im = c*2^64 + c*r + d*im\n    // c*r + d*im\
    \ < m * m + m * im < m * m + 2^64 + m <= 2^64 + m * (m + 1) <\n    // 2^64 * 2\n\
    \    // ((ab * im) >> 64) == c or c + 1\n    unsigned long long z = a;\n    z\
    \ *= b;\n#ifdef _MSC_VER\n    unsigned long long x;\n    _umul128(z, im, &x);\n\
    #else\n    unsigned long long x =\n        (unsigned long long)(((unsigned __int128)(z)*im)\
    \ >> 64);\n#endif\n    unsigned long long y = x * _m;\n    return (unsigned int)(z\
    \ - y + (z < y ? _m : 0));\n  }\n};\n} // namespace maomao90::internal::math\n\
    #line 2 \"library/math/extended_gcd.hpp\"\n\n#line 5 \"library/math/extended_gcd.hpp\"\
    \n\n#line 7 \"library/math/extended_gcd.hpp\"\n\nnamespace maomao90 {\nusing namespace\
    \ std;\ntemplate <signed_integral T>\n  requires internal::type_traits::is_64bit_or_less_v<T>\n\
    constexpr T inv_gcd(T x, T mod) {\n  using U = internal::type_traits::safely_multipliable_t<T>;\n\
    \  U a = mod, b = x, va = 0, vb = 1;\n  while (b) {\n    U k = a / b;\n    a -=\
    \ k * b;\n    va -= k * vb;\n    swap(a, b);\n    swap(va, vb);\n  }\n  assert(a\
    \ == 1); // gcd should be equal to 1\n  return (va % mod + mod) % mod;\n}\n} //\
    \ namespace maomao90\n#line 2 \"library/math/primality_test.hpp\"\n\n#line 4 \"\
    library/math/primality_test.hpp\"\n\n#line 2 \"library/math/pow_mod.hpp\"\n\n\
    #line 4 \"library/math/pow_mod.hpp\"\n\n#line 7 \"library/math/pow_mod.hpp\"\n\
    \nnamespace maomao90 {\nusing namespace std;\ntemplate <integral T, internal::concepts::broadly_unsigned_integral\
    \ P>\n  requires internal::type_traits::is_64bit_or_less_v<T>\nconstexpr T pow_mod(T\
    \ b, P p, T mod) {\n  using U = internal::type_traits::safely_multipliable_t<T>;\n\
    \  T res = 1;\n  b %= mod;\n  if (b < 0) {\n    b += mod;\n  }\n  while (p) {\n\
    \    if (p & 1) {\n      res = (U)res * b % mod;\n    }\n    b = (U)b * b % mod;\n\
    \    p >>= 1;\n  }\n  return res;\n}\n} // namespace maomao90\n#line 7 \"library/math/primality_test.hpp\"\
    \n\nnamespace maomao90 {\nusing namespace std;\ntemplate <unsigned_integral T>\n\
    \  requires internal::type_traits::is_64bit_or_less_v<T>\nconstexpr bool miller_rabin(const\
    \ T &n, const T *bases, const size_t size) {\n  using U = internal::type_traits::safely_multipliable_t<T>;\n\
    \  if (n <= 1) {\n    return false;\n  }\n  if (n == 2) {\n    return true;\n\
    \  }\n  if (n % 2 == 0) {\n    return false;\n  }\n  T d = n - 1;\n  while (d\
    \ % 2 == 0)\n    d /= 2;\n  for (size_t i = 0; i < size; i++) {\n    T a = bases[i];\n\
    \    if (a % n == 0) {\n      continue;\n    }\n    T t = d, y = pow_mod<T, T>(a,\
    \ t, n);\n    while (t != n - 1 && y != 1 && y != n - 1) {\n      y = (U)y * y\
    \ % n;\n      t <<= 1;\n    }\n    if (y != n - 1 && t % 2 == 0) {\n      return\
    \ false;\n    }\n  }\n  return true;\n}\ntemplate <unsigned_integral T>\n  requires\
    \ internal::type_traits::is_64bit_or_less_v<T>\nconstexpr bool is_prime(T n) {\n\
    \  if constexpr (internal::type_traits::is_32bit_or_less_v<T>) {\n    T bases[3]\
    \ = {2, 7, 61};\n    return miller_rabin<T>(n, bases, 3);\n  } else {\n    T bases[7]\
    \ = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};\n    return miller_rabin<T>(n,\
    \ bases, 7);\n  }\n}\n} // namespace maomao90\n#line 13 \"library/math/modint.hpp\"\
    \n\nnamespace maomao90 {\nnamespace internal::modint {\nstruct modint_base {};\n\
    struct static_modint_base : modint_base {};\n} // namespace internal::modint\n\
    using namespace std;\ntemplate <typename T>\nconcept ModInt = is_base_of_v<internal::modint::modint_base,\
    \ T>;\ntemplate <typename T>\nconcept StaticModInt = is_base_of_v<internal::modint::static_modint_base,\
    \ T>;\n\ntemplate <auto mod = 998244353, enable_if_t<(mod >= 1), nullptr_t> =\
    \ nullptr>\n  requires signed_integral<decltype(mod)> &&\n           internal::type_traits::is_64bit_or_less_v<decltype(mod)>\n\
    struct static_modint : internal::modint::static_modint_base {\nprivate:\n  using\
    \ M = decltype(mod);\n  using UM = make_unsigned_t<M>;\n  using BM = internal::type_traits::safely_multipliable_t<M>;\n\
    \npublic:\n  static constexpr bool is_prime_mod = is_prime((UM)mod);\n  using\
    \ mod_type = M;\n  using umod_type = UM;\n  static constexpr M imod() { return\
    \ mod; }\n  static constexpr UM umod() { return mod; }\n\n  static constexpr static_modint\
    \ raw(M v) {\n    static_modint res;\n    res._v = v;\n    return res;\n  }\n\n\
    \  constexpr static_modint() : _v(0) {}\n\n  template <internal::concepts::broadly_signed_integral\
    \ T>\n  constexpr static_modint(T v) {\n    M x = v % imod();\n    if (x < 0)\
    \ {\n      x += imod();\n    }\n    _v = x;\n  }\n\n  template <internal::concepts::broadly_unsigned_integral\
    \ T>\n  constexpr static_modint(T v) : _v(v % umod()) {}\n\n  constexpr UM val()\
    \ const { return _v; }\n\n  constexpr static_modint operator+() const { return\
    \ *this; }\n  constexpr static_modint operator-() const {\n    return raw(_v ==\
    \ 0 ? 0 : imod() - _v);\n  }\n\n  constexpr static_modint &operator++() {\n  \
    \  ++_v;\n    if (_v == umod()) {\n      _v = 0;\n    }\n    return *this;\n \
    \ }\n  constexpr static_modint &operator--() {\n    if (_v == 0) {\n      _v =\
    \ umod();\n    }\n    --_v;\n    return *this;\n  }\n  constexpr static_modint\
    \ operator++(int) {\n    static_modint x = *this;\n    ++*this;\n    return x;\n\
    \  }\n  constexpr static_modint operator--(int) {\n    static_modint x = *this;\n\
    \    --*this;\n    return x;\n  }\n\n  constexpr static_modint &operator+=(const\
    \ static_modint &o) {\n    _v += o._v;\n    if (_v >= umod()) {\n      _v -= umod();\n\
    \    }\n    return *this;\n  }\n  constexpr static_modint &operator-=(const static_modint\
    \ &o) {\n    if (_v < o._v) {\n      _v += umod();\n    }\n    _v -= o._v;\n \
    \   return *this;\n  }\n  constexpr static_modint &operator*=(const static_modint\
    \ &o) {\n    _v = (BM)_v * o._v % umod();\n    return *this;\n  }\n  constexpr\
    \ static_modint &operator/=(const static_modint &o) {\n    return *this *= o.inv();\n\
    \  }\n\n  constexpr static_modint pow(long long p) const {\n    assert(p >= 0);\n\
    \    static_modint res = 1, b = *this;\n    while (p) {\n      if (p & 1) {\n\
    \        res *= b;\n      }\n      b *= b;\n      p >>= 1;\n    }\n    return\
    \ res;\n  }\n  constexpr static_modint inv() const {\n    if constexpr (is_prime_mod)\
    \ {\n      return pow(imod() - 2);\n    } else {\n      return raw(inv_gcd((M)_v,\
    \ imod()));\n    }\n  }\n\n  friend constexpr static_modint operator+(const static_modint\
    \ &l,\n                                           const static_modint &r) {\n\
    \    static_modint res = l;\n    return res += r;\n  }\n  friend constexpr static_modint\
    \ operator-(const static_modint &l,\n                                        \
    \   const static_modint &r) {\n    static_modint res = l;\n    return res -= r;\n\
    \  }\n  friend constexpr static_modint operator*(const static_modint &l,\n   \
    \                                        const static_modint &r) {\n    static_modint\
    \ res = l;\n    return res *= r;\n  }\n  friend constexpr static_modint operator/(const\
    \ static_modint &l,\n                                           const static_modint\
    \ &r) {\n    static_modint res = l;\n    return res /= r;\n  }\n\n  constexpr\
    \ bool operator==(const static_modint &o) const { return _v == o._v; }\n  constexpr\
    \ bool operator!=(const static_modint &o) const {\n    return !(*this == o);\n\
    \  }\n\n  friend constexpr istream &operator>>(istream &is, static_modint &o)\
    \ {\n    M v;\n    is >> v;\n    o = static_modint(v);\n    return is;\n  }\n\
    \  friend constexpr ostream &operator<<(ostream &os, const static_modint &o) {\n\
    \    return os << o._v;\n  }\n\nprivate:\n  UM _v;\n};\n\ntemplate <int id = -1>\
    \ struct dynamic_modint : internal::modint::modint_base {\npublic:\n  static void\
    \ set_mod(int mod) {\n    assert(1 <= mod);\n    bt = internal::math::barrett(mod);\n\
    \  }\n  static constexpr int imod() { return bt.umod(); }\n  static constexpr\
    \ unsigned int umod() { return bt.umod(); }\n\n  static constexpr dynamic_modint\
    \ raw(int v) {\n    dynamic_modint res;\n    res._v = v;\n    return res;\n  }\n\
    \n  constexpr dynamic_modint() : _v(0) {}\n\n  template <internal::concepts::broadly_signed_integral\
    \ T>\n  constexpr dynamic_modint(T v) {\n    long long x = v % imod();\n    if\
    \ (x < 0) {\n      x += imod();\n    }\n    _v = x;\n  }\n\n  template <internal::concepts::broadly_unsigned_integral\
    \ T>\n  constexpr dynamic_modint(T v) : _v(v % umod()) {}\n\n  constexpr unsigned\
    \ int val() const { return _v; }\n  constexpr dynamic_modint operator+() const\
    \ { return *this; }\n  constexpr dynamic_modint operator-() const {\n    return\
    \ raw(_v == 0 ? 0 : imod() - _v);\n  }\n\n  constexpr dynamic_modint &operator++()\
    \ {\n    ++_v;\n    if (_v == umod()) {\n      _v = 0;\n    }\n    return *this;\n\
    \  }\n  constexpr dynamic_modint &operator--() {\n    if (_v == 0) {\n      _v\
    \ = umod();\n    }\n    --_v;\n    return *this;\n  }\n  constexpr dynamic_modint\
    \ operator++(int) {\n    dynamic_modint x = *this;\n    ++*this;\n    return x;\n\
    \  }\n  constexpr dynamic_modint operator--(int) {\n    dynamic_modint x = *this;\n\
    \    --*this;\n    return x;\n  }\n\n  constexpr dynamic_modint &operator+=(const\
    \ dynamic_modint &o) {\n    _v += o._v;\n    if (_v >= umod()) {\n      _v -=\
    \ umod();\n    }\n    return *this;\n  }\n  constexpr dynamic_modint &operator-=(const\
    \ dynamic_modint &o) {\n    if (_v < o._v) {\n      _v += umod();\n    }\n   \
    \ _v -= o._v;\n    return *this;\n  }\n  constexpr dynamic_modint &operator*=(const\
    \ dynamic_modint &o) {\n    _v = bt.mul(_v, o._v);\n    //_v = (long long)_v *\
    \ o._v % umod();\n    return *this;\n  }\n  constexpr dynamic_modint &operator/=(const\
    \ dynamic_modint &o) {\n    return *this *= o.inv();\n  }\n\n  constexpr dynamic_modint\
    \ pow(long long p) const {\n    assert(p >= 0);\n    dynamic_modint res = 1, b\
    \ = *this;\n    while (p) {\n      if (p & 1) {\n        res *= b;\n      }\n\
    \      b *= b;\n      p >>= 1;\n    }\n    return res;\n  }\n  constexpr dynamic_modint\
    \ inv() const { return raw(inv_gcd((int)_v, imod())); }\n\n  friend constexpr\
    \ dynamic_modint operator+(const dynamic_modint &l,\n                        \
    \                    const dynamic_modint &r) {\n    dynamic_modint res = l;\n\
    \    return res += r;\n  }\n  friend constexpr dynamic_modint operator-(const\
    \ dynamic_modint &l,\n                                            const dynamic_modint\
    \ &r) {\n    dynamic_modint res = l;\n    return res -= r;\n  }\n  friend constexpr\
    \ dynamic_modint operator*(const dynamic_modint &l,\n                        \
    \                    const dynamic_modint &r) {\n    dynamic_modint res = l;\n\
    \    return res *= r;\n  }\n  friend constexpr dynamic_modint operator/(const\
    \ dynamic_modint &l,\n                                            const dynamic_modint\
    \ &r) {\n    dynamic_modint res = l;\n    return res /= r;\n  }\n\n  constexpr\
    \ bool operator==(const dynamic_modint &o) const {\n    return _v == o._v;\n \
    \ }\n  constexpr bool operator!=(const dynamic_modint &o) const {\n    return\
    \ !(*this == o);\n  }\n\n  friend constexpr istream &operator>>(istream &is, dynamic_modint\
    \ &o) {\n    int v;\n    is >> v;\n    o = dynamic_modint(v);\n    return is;\n\
    \  }\n  friend constexpr ostream &operator<<(ostream &os, const dynamic_modint\
    \ &o) {\n    return os << o._v;\n  }\n\nprivate:\n  unsigned int _v;\n  static\
    \ internal::math::barrett bt;\n};\ntemplate <int id> internal::math::barrett dynamic_modint<id>::bt(998244353);\n\
    } // namespace maomao90\n"
  code: "#pragma once\n\n#include <cassert>\n#include <concepts>\n#include <iostream>\n\
    #include <type_traits>\n\n#include \"library/internal/concepts.hpp\"\n#include\
    \ \"library/internal/math.hpp\"\n#include \"library/internal/type_traits.hpp\"\
    \n#include \"library/math/extended_gcd.hpp\"\n#include \"library/math/primality_test.hpp\"\
    \n\nnamespace maomao90 {\nnamespace internal::modint {\nstruct modint_base {};\n\
    struct static_modint_base : modint_base {};\n} // namespace internal::modint\n\
    using namespace std;\ntemplate <typename T>\nconcept ModInt = is_base_of_v<internal::modint::modint_base,\
    \ T>;\ntemplate <typename T>\nconcept StaticModInt = is_base_of_v<internal::modint::static_modint_base,\
    \ T>;\n\ntemplate <auto mod = 998244353, enable_if_t<(mod >= 1), nullptr_t> =\
    \ nullptr>\n  requires signed_integral<decltype(mod)> &&\n           internal::type_traits::is_64bit_or_less_v<decltype(mod)>\n\
    struct static_modint : internal::modint::static_modint_base {\nprivate:\n  using\
    \ M = decltype(mod);\n  using UM = make_unsigned_t<M>;\n  using BM = internal::type_traits::safely_multipliable_t<M>;\n\
    \npublic:\n  static constexpr bool is_prime_mod = is_prime((UM)mod);\n  using\
    \ mod_type = M;\n  using umod_type = UM;\n  static constexpr M imod() { return\
    \ mod; }\n  static constexpr UM umod() { return mod; }\n\n  static constexpr static_modint\
    \ raw(M v) {\n    static_modint res;\n    res._v = v;\n    return res;\n  }\n\n\
    \  constexpr static_modint() : _v(0) {}\n\n  template <internal::concepts::broadly_signed_integral\
    \ T>\n  constexpr static_modint(T v) {\n    M x = v % imod();\n    if (x < 0)\
    \ {\n      x += imod();\n    }\n    _v = x;\n  }\n\n  template <internal::concepts::broadly_unsigned_integral\
    \ T>\n  constexpr static_modint(T v) : _v(v % umod()) {}\n\n  constexpr UM val()\
    \ const { return _v; }\n\n  constexpr static_modint operator+() const { return\
    \ *this; }\n  constexpr static_modint operator-() const {\n    return raw(_v ==\
    \ 0 ? 0 : imod() - _v);\n  }\n\n  constexpr static_modint &operator++() {\n  \
    \  ++_v;\n    if (_v == umod()) {\n      _v = 0;\n    }\n    return *this;\n \
    \ }\n  constexpr static_modint &operator--() {\n    if (_v == 0) {\n      _v =\
    \ umod();\n    }\n    --_v;\n    return *this;\n  }\n  constexpr static_modint\
    \ operator++(int) {\n    static_modint x = *this;\n    ++*this;\n    return x;\n\
    \  }\n  constexpr static_modint operator--(int) {\n    static_modint x = *this;\n\
    \    --*this;\n    return x;\n  }\n\n  constexpr static_modint &operator+=(const\
    \ static_modint &o) {\n    _v += o._v;\n    if (_v >= umod()) {\n      _v -= umod();\n\
    \    }\n    return *this;\n  }\n  constexpr static_modint &operator-=(const static_modint\
    \ &o) {\n    if (_v < o._v) {\n      _v += umod();\n    }\n    _v -= o._v;\n \
    \   return *this;\n  }\n  constexpr static_modint &operator*=(const static_modint\
    \ &o) {\n    _v = (BM)_v * o._v % umod();\n    return *this;\n  }\n  constexpr\
    \ static_modint &operator/=(const static_modint &o) {\n    return *this *= o.inv();\n\
    \  }\n\n  constexpr static_modint pow(long long p) const {\n    assert(p >= 0);\n\
    \    static_modint res = 1, b = *this;\n    while (p) {\n      if (p & 1) {\n\
    \        res *= b;\n      }\n      b *= b;\n      p >>= 1;\n    }\n    return\
    \ res;\n  }\n  constexpr static_modint inv() const {\n    if constexpr (is_prime_mod)\
    \ {\n      return pow(imod() - 2);\n    } else {\n      return raw(inv_gcd((M)_v,\
    \ imod()));\n    }\n  }\n\n  friend constexpr static_modint operator+(const static_modint\
    \ &l,\n                                           const static_modint &r) {\n\
    \    static_modint res = l;\n    return res += r;\n  }\n  friend constexpr static_modint\
    \ operator-(const static_modint &l,\n                                        \
    \   const static_modint &r) {\n    static_modint res = l;\n    return res -= r;\n\
    \  }\n  friend constexpr static_modint operator*(const static_modint &l,\n   \
    \                                        const static_modint &r) {\n    static_modint\
    \ res = l;\n    return res *= r;\n  }\n  friend constexpr static_modint operator/(const\
    \ static_modint &l,\n                                           const static_modint\
    \ &r) {\n    static_modint res = l;\n    return res /= r;\n  }\n\n  constexpr\
    \ bool operator==(const static_modint &o) const { return _v == o._v; }\n  constexpr\
    \ bool operator!=(const static_modint &o) const {\n    return !(*this == o);\n\
    \  }\n\n  friend constexpr istream &operator>>(istream &is, static_modint &o)\
    \ {\n    M v;\n    is >> v;\n    o = static_modint(v);\n    return is;\n  }\n\
    \  friend constexpr ostream &operator<<(ostream &os, const static_modint &o) {\n\
    \    return os << o._v;\n  }\n\nprivate:\n  UM _v;\n};\n\ntemplate <int id = -1>\
    \ struct dynamic_modint : internal::modint::modint_base {\npublic:\n  static void\
    \ set_mod(int mod) {\n    assert(1 <= mod);\n    bt = internal::math::barrett(mod);\n\
    \  }\n  static constexpr int imod() { return bt.umod(); }\n  static constexpr\
    \ unsigned int umod() { return bt.umod(); }\n\n  static constexpr dynamic_modint\
    \ raw(int v) {\n    dynamic_modint res;\n    res._v = v;\n    return res;\n  }\n\
    \n  constexpr dynamic_modint() : _v(0) {}\n\n  template <internal::concepts::broadly_signed_integral\
    \ T>\n  constexpr dynamic_modint(T v) {\n    long long x = v % imod();\n    if\
    \ (x < 0) {\n      x += imod();\n    }\n    _v = x;\n  }\n\n  template <internal::concepts::broadly_unsigned_integral\
    \ T>\n  constexpr dynamic_modint(T v) : _v(v % umod()) {}\n\n  constexpr unsigned\
    \ int val() const { return _v; }\n  constexpr dynamic_modint operator+() const\
    \ { return *this; }\n  constexpr dynamic_modint operator-() const {\n    return\
    \ raw(_v == 0 ? 0 : imod() - _v);\n  }\n\n  constexpr dynamic_modint &operator++()\
    \ {\n    ++_v;\n    if (_v == umod()) {\n      _v = 0;\n    }\n    return *this;\n\
    \  }\n  constexpr dynamic_modint &operator--() {\n    if (_v == 0) {\n      _v\
    \ = umod();\n    }\n    --_v;\n    return *this;\n  }\n  constexpr dynamic_modint\
    \ operator++(int) {\n    dynamic_modint x = *this;\n    ++*this;\n    return x;\n\
    \  }\n  constexpr dynamic_modint operator--(int) {\n    dynamic_modint x = *this;\n\
    \    --*this;\n    return x;\n  }\n\n  constexpr dynamic_modint &operator+=(const\
    \ dynamic_modint &o) {\n    _v += o._v;\n    if (_v >= umod()) {\n      _v -=\
    \ umod();\n    }\n    return *this;\n  }\n  constexpr dynamic_modint &operator-=(const\
    \ dynamic_modint &o) {\n    if (_v < o._v) {\n      _v += umod();\n    }\n   \
    \ _v -= o._v;\n    return *this;\n  }\n  constexpr dynamic_modint &operator*=(const\
    \ dynamic_modint &o) {\n    _v = bt.mul(_v, o._v);\n    //_v = (long long)_v *\
    \ o._v % umod();\n    return *this;\n  }\n  constexpr dynamic_modint &operator/=(const\
    \ dynamic_modint &o) {\n    return *this *= o.inv();\n  }\n\n  constexpr dynamic_modint\
    \ pow(long long p) const {\n    assert(p >= 0);\n    dynamic_modint res = 1, b\
    \ = *this;\n    while (p) {\n      if (p & 1) {\n        res *= b;\n      }\n\
    \      b *= b;\n      p >>= 1;\n    }\n    return res;\n  }\n  constexpr dynamic_modint\
    \ inv() const { return raw(inv_gcd((int)_v, imod())); }\n\n  friend constexpr\
    \ dynamic_modint operator+(const dynamic_modint &l,\n                        \
    \                    const dynamic_modint &r) {\n    dynamic_modint res = l;\n\
    \    return res += r;\n  }\n  friend constexpr dynamic_modint operator-(const\
    \ dynamic_modint &l,\n                                            const dynamic_modint\
    \ &r) {\n    dynamic_modint res = l;\n    return res -= r;\n  }\n  friend constexpr\
    \ dynamic_modint operator*(const dynamic_modint &l,\n                        \
    \                    const dynamic_modint &r) {\n    dynamic_modint res = l;\n\
    \    return res *= r;\n  }\n  friend constexpr dynamic_modint operator/(const\
    \ dynamic_modint &l,\n                                            const dynamic_modint\
    \ &r) {\n    dynamic_modint res = l;\n    return res /= r;\n  }\n\n  constexpr\
    \ bool operator==(const dynamic_modint &o) const {\n    return _v == o._v;\n \
    \ }\n  constexpr bool operator!=(const dynamic_modint &o) const {\n    return\
    \ !(*this == o);\n  }\n\n  friend constexpr istream &operator>>(istream &is, dynamic_modint\
    \ &o) {\n    int v;\n    is >> v;\n    o = dynamic_modint(v);\n    return is;\n\
    \  }\n  friend constexpr ostream &operator<<(ostream &os, const dynamic_modint\
    \ &o) {\n    return os << o._v;\n  }\n\nprivate:\n  unsigned int _v;\n  static\
    \ internal::math::barrett bt;\n};\ntemplate <int id> internal::math::barrett dynamic_modint<id>::bt(998244353);\n\
    } // namespace maomao90\n"
  dependsOn:
  - library/internal/concepts.hpp
  - library/internal/type_traits.hpp
  - library/internal/math.hpp
  - library/math/extended_gcd.hpp
  - library/math/primality_test.hpp
  - library/math/pow_mod.hpp
  isVerificationFile: false
  path: library/math/modint.hpp
  requiredBy:
  - library/math/poly.hpp
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
documentation_of: library/math/modint.hpp
layout: document
title: ModInt
---

```c++
template <
  typename T
> concept ModInt;
template <
  typename T
> concept StaticModInt;
```

The concept `ModInt<T>` is satisfied if `T` is either `static_modint` or `dynamic_modint`.
`StaticModInt<T>` is only satisfied if `T` is `static_modint`.

## Static Modulo
```c++
template <
  auto mod = 998244353
> struct static_modint;	
```

### Template parameters

- `mod` - the value of the modulo.

**Template requirements**

- `mod` has to be a positive integer that is of type `signed_integral` with 64-bits or less.

### Member types

- `mod_type` - the type of `mod`.
- `umod_type` - unsigned type of `mod`.

### Constructor

- `static_modint();`	 
  Initialise modint with value $0$.
  
- `static_modint(T v);`  
  Initialise modint with value `v`.

### Member functions

- `static static_modint raw(mod_type v);`  
  Constant time optimisation to create `static_modint` if $0\le \texttt{v} < \texttt{mod}$.

- `static constexpr mod_type imod();`  
  Returns the signed value of `mod`.
  
- `static constexpr umod_type umod();`  
  Returns the unsigned value of `mod`.

- `umod_type val() const;`  
  Return value stored in `static_modint`.

- `static_modint pow(long long p) const;`  
  Returns `static_modint` of the current value raised to the `p`-th power.  
	**Performance**: $O(\log_2 \texttt{p})$

- `static_modint inv() const;`  
  Returns `static_modint` of the modular inverse of the current value.  
  Note that the value stored in `static_modint` has to be co-prime to `mod`. Otherwise, an assertion failure will occur.  
	**Performance**: $O(\log_2 \texttt{mod})$

- `operator+`, `operator+=`, `operator-`, `operator-=`, `operator*`, `operator*=`  
	Add / subtract / multiply two modints.  
	**Performance**: $O(1)$

- `operator++`, `operator--`  
	Prefix and postfix increment / decrement modint.  
	**Performance**: $O(1)$

- `operator/`, `operator/=`  
	Divide two modints.  
	**Performance**: $O(\log_2 \texttt{mod})$

- `operator+`, `operator-`  
	Unary operators.  
	**Performance**: $O(1)$
	
- `operator==`, `operator!=`  
	Compare two modints.  
	**Performance**: $O(1)$
	
- `operator<<`, `operator>>`  
	Serializes and deserializes a modint.  
	**Performance**: $O(1)$

## Dynamic Modulo

```c++
template <
  int id = -1
> struct dynamic_modint;	
```

### Template parameters

- `id` - arbitrary id. Set different id for different mods.

### Constructor

- `dynamic_modint();`	 
  Initialise modint with value $0$.
  
- `dynamic_modint(T v);`  
  Initialise modint with value `v`.

### Member functions

- `static void set_mod(int mod);`  
  Set mod to use.

- `static constexpr int imod();`  
  Returns the signed value of `mod`.
  
- `static constexpr unsigned int umod();`  
  Returns the unsigned value of `mod`.

- `static dynamic_modint raw(int v);`  
  Constant time optimisation to create `dynamic_modint` if $0\le \texttt{v} < \texttt{mod}$.

- `unsigned int val() const;`  
  Return value stored in `dynamic_modint`.

- `dynamic_modint pow(long long p) const;`  
  Returns `dynamic_modint` of the current value raised to the `p`-th power.  
	**Performance**: $O(\log_2 \texttt{p})$

- `dynamic_modint inv() const;`  
  Returns `dynamic_modint` of the modular inverse of the current value.  
  Note that the value stored in `dynamic_modint` has to be co-prime to `mod`. Otherwise, an assertion failure will occur.  
	**Performance**: $O(\log_2 \texttt{mod})$

- `operator+`, `operator+=`, `operator-`, `operator-=`, `operator*`, `operator*=`  
	Add / subtract / multiply two modints.  
	**Performance**: $O(1)$

- `operator++`, `operator--`  
	Prefix and postfix increment / decrement modint.  
	**Performance**: $O(1)$

- `operator/`, `operator/=`  
	Divide two modints.  
	**Performance**: $O(\log_2 \texttt{mod})$

- `operator+`, `operator-`  
	Unary operators.  
	**Performance**: $O(1)$
	
- `operator==`, `operator!=`  
	Compare two modints.  
	**Performance**: $O(1)$
	
- `operator<<`, `operator>>`  
	Serializes and deserializes a modint.  
	**Performance**: $O(1)$