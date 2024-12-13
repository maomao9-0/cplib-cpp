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
    path: library/math/modint.hpp
    title: ModInt
  - icon: ':heavy_check_mark:'
    path: library/math/pow_mod.hpp
    title: Pow Mod
  - icon: ':heavy_check_mark:'
    path: library/math/primality_test.hpp
    title: Primality Test
  _extendedRequiredBy: []
  _extendedVerifiedWith:
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
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"library/math/poly.hpp\"\n\n#include <algorithm>\n#include\
    \ <array>\n#include <bit>\n#include <complex>\n#include <type_traits>\n#include\
    \ <vector>\n\n#line 2 \"library/internal/concepts.hpp\"\n\n#line 2 \"library/internal/type_traits.hpp\"\
    \n\n#include <limits>\n#line 5 \"library/internal/type_traits.hpp\"\n#include\
    \ <utility>\n\nnamespace maomao90::internal::type_traits {\nusing namespace std;\n\
    #define ENABLE_VALUE(x) template <typename T> constexpr bool x##_v = x<T>::value\n\
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
    #line 2 \"library/math/extended_gcd.hpp\"\n\n#include <cassert>\n#include <concepts>\n\
    \n#line 7 \"library/math/extended_gcd.hpp\"\n\nnamespace maomao90 {\nusing namespace\
    \ std;\ntemplate <signed_integral T>\n  requires internal::type_traits::is_64bit_or_less_v<T>\n\
    constexpr T inv_gcd(T x, T mod) {\n  using U = internal::type_traits::safely_multipliable_t<T>;\n\
    \  U a = mod, b = x, va = 0, vb = 1;\n  while (b) {\n    U k = a / b;\n    a -=\
    \ k * b;\n    va -= k * vb;\n    swap(a, b);\n    swap(va, vb);\n  }\n  assert(a\
    \ == 1); // gcd should be equal to 1\n  return (va % mod + mod) % mod;\n}\n} //\
    \ namespace maomao90\n#line 2 \"library/math/modint.hpp\"\n\n#line 5 \"library/math/modint.hpp\"\
    \n#include <iostream>\n#line 7 \"library/math/modint.hpp\"\n\n#line 2 \"library/math/primality_test.hpp\"\
    \n\n#line 4 \"library/math/primality_test.hpp\"\n\n#line 2 \"library/math/pow_mod.hpp\"\
    \n\n#line 4 \"library/math/pow_mod.hpp\"\n\n#line 7 \"library/math/pow_mod.hpp\"\
    \n\nnamespace maomao90 {\nusing namespace std;\ntemplate <integral T, internal::concepts::broadly_unsigned_integral\
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
    } // namespace maomao90\n#line 14 \"library/math/poly.hpp\"\n\nnamespace maomao90\
    \ {\nusing namespace std;\nnamespace internal::poly {\ntemplate <class T>\nvector<T>\
    \ convolution_naive(const vector<T> &a, const vector<T> &b) {\n  int n = a.size(),\
    \ m = b.size();\n  vector<T> ans(n + m - 1);\n  if (n < m) {\n    for (int j =\
    \ 0; j < m; j++) {\n      for (int i = 0; i < n; i++) {\n        ans[i + j] +=\
    \ a[i] * b[j];\n      }\n    }\n  } else {\n    for (int i = 0; i < n; i++) {\n\
    \      for (int j = 0; j < m; j++) {\n        ans[i + j] += a[i] * b[j];\n   \
    \   }\n    }\n  }\n  return ans;\n}\nnamespace ntt {\ntemplate <StaticModInt mint,\n\
    \          int g = internal::math::primitive_root<mint::imod()>>\nstruct ntt_info\
    \ {\n  static constexpr int rank2 = __builtin_ctz(mint::umod() - 1);\n  array<mint,\
    \ rank2 + 1> root;  // root[i]^(2^i) == 1\n  array<mint, rank2 + 1> iroot; //\
    \ root[i] * iroot[i] == 1\n\n  array<mint, max(0, rank2 - 2 + 1)> rate2;\n  array<mint,\
    \ max(0, rank2 - 2 + 1)> irate2;\n\n  array<mint, max(0, rank2 - 3 + 1)> rate3;\n\
    \  array<mint, max(0, rank2 - 3 + 1)> irate3;\n\n  ntt_info() {\n    root[rank2]\
    \ = mint(g).pow((mint::imod() - 1) >> rank2);\n    iroot[rank2] = root[rank2].inv();\n\
    \    for (int i = rank2 - 1; i >= 0; i--) {\n      root[i] = root[i + 1] * root[i\
    \ + 1];\n      iroot[i] = iroot[i + 1] * iroot[i + 1];\n    }\n\n    {\n     \
    \ mint prod = 1, iprod = 1;\n      for (int i = 0; i <= rank2 - 2; i++) {\n  \
    \      rate2[i] = root[i + 2] * prod;\n        irate2[i] = iroot[i + 2] * iprod;\n\
    \        prod *= iroot[i + 2];\n        iprod *= root[i + 2];\n      }\n    }\n\
    \    {\n      mint prod = 1, iprod = 1;\n      for (int i = 0; i <= rank2 - 3;\
    \ i++) {\n        rate3[i] = root[i + 3] * prod;\n        irate3[i] = iroot[i\
    \ + 3] * iprod;\n        prod *= iroot[i + 3];\n        iprod *= root[i + 3];\n\
    \      }\n    }\n  }\n};\n\ntemplate <StaticModInt mint> void butterfly(vector<mint>\
    \ &a) {\n  int n = a.size();\n  int h = __builtin_ctz((unsigned int)n);\n\n  static\
    \ const ntt_info<mint> info;\n\n  int len = 0; // a[i, i+(n>>len), i+2*(n>>len),\
    \ ..] is transformed\n  while (len < h) {\n    if (h - len == 1) {\n      int\
    \ p = 1 << (h - len - 1);\n      mint rot = 1;\n      for (int s = 0; s < (1 <<\
    \ len); s++) {\n        int offset = s << (h - len);\n        for (int i = 0;\
    \ i < p; i++) {\n          auto l = a[i + offset];\n          auto r = a[i + offset\
    \ + p] * rot;\n          a[i + offset] = l + r;\n          a[i + offset + p] =\
    \ l - r;\n        }\n        if (s + 1 != (1 << len)) {\n          rot *= info.rate2[countr_zero(~(unsigned\
    \ int)(s))];\n        }\n      }\n      len++;\n    } else {\n      // 4-base\n\
    \      int p = 1 << (h - len - 2);\n      mint rot = 1, imag = info.root[2];\n\
    \      for (int s = 0; s < (1 << len); s++) {\n        mint rot2 = rot * rot;\n\
    \        mint rot3 = rot2 * rot;\n        int offset = s << (h - len);\n     \
    \   for (int i = 0; i < p; i++) {\n          auto mod2 = 1ull * mint::imod() *\
    \ mint::imod();\n          auto a0 = 1ull * a[i + offset].val();\n          auto\
    \ a1 = 1ull * a[i + offset + p].val() * rot.val();\n          auto a2 = 1ull *\
    \ a[i + offset + 2 * p].val() * rot2.val();\n          auto a3 = 1ull * a[i +\
    \ offset + 3 * p].val() * rot3.val();\n          auto a1na3imag = 1ull * mint(a1\
    \ + mod2 - a3).val() * imag.val();\n          auto na2 = mod2 - a2;\n        \
    \  a[i + offset] = a0 + a2 + a1 + a3;\n          a[i + offset + 1 * p] = a0 +\
    \ a2 + (2 * mod2 - (a1 + a3));\n          a[i + offset + 2 * p] = a0 + na2 + a1na3imag;\n\
    \          a[i + offset + 3 * p] = a0 + na2 + (mod2 - a1na3imag);\n        }\n\
    \        if (s + 1 != (1 << len)) {\n          rot *= info.rate3[countr_zero(~(unsigned\
    \ int)(s))];\n        }\n      }\n      len += 2;\n    }\n  }\n}\n\ntemplate <StaticModInt\
    \ mint> void butterfly_inv(vector<mint> &a) {\n  int n = a.size();\n  int h =\
    \ __builtin_ctz((unsigned int)n);\n\n  static const ntt_info<mint> info;\n\n \
    \ int len = h; // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed\n  while (len)\
    \ {\n    if (len == 1) {\n      int p = 1 << (h - len);\n      mint irot = 1;\n\
    \      for (int s = 0; s < (1 << (len - 1)); s++) {\n        int offset = s <<\
    \ (h - len + 1);\n        for (int i = 0; i < p; i++) {\n          auto l = a[i\
    \ + offset];\n          auto r = a[i + offset + p];\n          a[i + offset] =\
    \ l + r;\n          a[i + offset + p] =\n              (unsigned long long)(mint::imod()\
    \ + l.val() - r.val()) *\n              irot.val();\n          ;\n        }\n\
    \        if (s + 1 != (1 << (len - 1))) {\n          irot *= info.irate2[countr_zero(~(unsigned\
    \ int)(s))];\n        }\n      }\n      len--;\n    } else {\n      // 4-base\n\
    \      int p = 1 << (h - len);\n      mint irot = 1, iimag = info.iroot[2];\n\
    \      for (int s = 0; s < (1 << (len - 2)); s++) {\n        mint irot2 = irot\
    \ * irot;\n        mint irot3 = irot2 * irot;\n        int offset = s << (h -\
    \ len + 2);\n        for (int i = 0; i < p; i++) {\n          auto a0 = 1ull *\
    \ a[i + offset + 0 * p].val();\n          auto a1 = 1ull * a[i + offset + 1 *\
    \ p].val();\n          auto a2 = 1ull * a[i + offset + 2 * p].val();\n       \
    \   auto a3 = 1ull * a[i + offset + 3 * p].val();\n\n          auto a2na3iimag\
    \ =\n              1ull * mint((mint::imod() + a2 - a3) * iimag.val()).val();\n\
    \n          a[i + offset] = a0 + a1 + a2 + a3;\n          a[i + offset + 1 * p]\
    \ =\n              (a0 + (mint::imod() - a1) + a2na3iimag) * irot.val();\n   \
    \       a[i + offset + 2 * p] =\n              (a0 + a1 + (mint::imod() - a2)\
    \ + (mint::imod() - a3)) *\n              irot2.val();\n          a[i + offset\
    \ + 3 * p] =\n              (a0 + (mint::imod() - a1) + (mint::imod() - a2na3iimag))\
    \ *\n              irot3.val();\n        }\n        if (s + 1 != (1 << (len -\
    \ 2))) {\n          irot *= info.irate3[countr_zero(~(unsigned int)(s))];\n  \
    \      }\n      }\n      len -= 2;\n    }\n  }\n}\ntemplate <StaticModInt mint>\n\
    vector<mint> convolution_ntt(vector<mint> a, vector<mint> b) {\n  int n = a.size(),\
    \ m = b.size();\n  int z = bit_ceil((unsigned int)(n + m - 1));\n  a.resize(z);\n\
    \  butterfly(a);\n  b.resize(z);\n  butterfly(b);\n  for (int i = 0; i < z; i++)\
    \ {\n    a[i] *= b[i];\n  }\n  butterfly_inv(a);\n  a.resize(n + m - 1);\n  mint\
    \ iz = mint(z).inv();\n  for (int i = 0; i < n + m - 1; i++)\n    a[i] *= iz;\n\
    \  return a;\n}\ntemplate <StaticModInt mint>\nvector<mint> convolution(const\
    \ vector<mint> &a, const vector<mint> &b) {\n  int n = a.size(), m = b.size();\n\
    \  if (!n || !m) {\n    return {};\n  }\n\n  int z = bit_ceil((unsigned int)(n\
    \ + m - 1));\n  assert((mint::imod() - 1) % z == 0);\n\n  if (min(n, m) <= 60)\
    \ {\n    return convolution_naive(a, b);\n  }\n  return convolution_ntt(a, b);\n\
    }\ntemplate <int mod, class T>\n  requires is_integral_v<T>\nvector<T> convolution(const\
    \ vector<T> &a, const vector<T> &b) {\n  int n = a.size(), m = b.size();\n  if\
    \ (!n || !m)\n    return {};\n\n  using mint = static_modint<mod>;\n\n  int z\
    \ = bit_ceil((unsigned int)(n + m - 1));\n  assert((mint::imod() - 1) % z == 0);\n\
    \n  vector<mint> a2(n), b2(m);\n  for (int i = 0; i < n; i++) {\n    a2[i] = mint(a[i]);\n\
    \  }\n  for (int i = 0; i < m; i++) {\n    b2[i] = mint(b[i]);\n  }\n  auto c2\
    \ = convolution(std::move(a2), std::move(b2));\n  vector<T> c(n + m - 1);\n  for\
    \ (int i = 0; i < n + m - 1; i++) {\n    c[i] = c2[i].val();\n  }\n  return c;\n\
    }\ninline vector<long long> convolution_ll(const vector<long long> &a,\n     \
    \                                   const vector<long long> &b) {\n  int n = a.size(),\
    \ m = b.size();\n  if (!n || !m) {\n    return {};\n  }\n\n  if (min(n, m) <=\
    \ 60) {\n    return convolution_naive(a, b);\n  }\n\n  static constexpr unsigned\
    \ long long MOD1 = 754974721; // 2^24\n  static constexpr unsigned long long MOD2\
    \ = 167772161; // 2^25\n  static constexpr unsigned long long MOD3 = 469762049;\
    \ // 2^26\n  static constexpr unsigned long long M2M3 = MOD2 * MOD3;\n  static\
    \ constexpr unsigned long long M1M3 = MOD1 * MOD3;\n  static constexpr unsigned\
    \ long long M1M2 = MOD1 * MOD2;\n  static constexpr unsigned long long M1M2M3\
    \ = MOD1 * MOD2 * MOD3;\n\n  static constexpr unsigned long long i1 =\n      inv_gcd<long\
    \ long>(MOD2 * MOD3, MOD1);\n  static constexpr unsigned long long i2 =\n    \
    \  inv_gcd<long long>(MOD1 * MOD3, MOD2);\n  static constexpr unsigned long long\
    \ i3 =\n      inv_gcd<long long>(MOD1 * MOD2, MOD3);\n\n  static constexpr int\
    \ MAX_AB_BIT = 24;\n  static_assert(MOD1 % (1ull << MAX_AB_BIT) == 1,\n      \
    \          \"MOD1 isn't enough to support an array length of 2^24.\");\n  static_assert(MOD2\
    \ % (1ull << MAX_AB_BIT) == 1,\n                \"MOD2 isn't enough to support\
    \ an array length of 2^24.\");\n  static_assert(MOD3 % (1ull << MAX_AB_BIT) ==\
    \ 1,\n                \"MOD3 isn't enough to support an array length of 2^24.\"\
    );\n  assert(n + m - 1 <= (1 << MAX_AB_BIT));\n\n  vector<long long> c1 = convolution<MOD1>(a,\
    \ b);\n  vector<long long> c2 = convolution<MOD2>(a, b);\n  vector<long long>\
    \ c3 = convolution<MOD3>(a, b);\n\n  vector<long long> c(n + m - 1);\n  for (int\
    \ i = 0; i < n + m - 1; i++) {\n    unsigned long long x = 0;\n    x += (c1[i]\
    \ * i1) % MOD1 * M2M3;\n    x += (c2[i] * i2) % MOD2 * M1M3;\n    x += (c3[i]\
    \ * i3) % MOD3 * M1M2;\n    // B = 2^63, -B <= x, r(real value) < B\n    // (x,\
    \ x - M, x - 2M, or x - 3M) = r (mod 2B)\n    // r = c1[i] (mod MOD1)\n    //\
    \ focus on MOD1\n    // r = x, x - M', x - 2M', x - 3M' (M' = M % 2^64) (mod 2B)\n\
    \    // r = x,\n    //     x - M' + (0 or 2B),\n    //     x - 2M' + (0, 2B or\
    \ 4B),\n    //     x - 3M' + (0, 2B, 4B or 6B) (without mod!)\n    // (r - x)\
    \ = 0, (0)\n    //           - M' + (0 or 2B), (1)\n    //           -2M' + (0\
    \ or 2B or 4B), (2)\n    //           -3M' + (0 or 2B or 4B or 6B) (3) (mod MOD1)\n\
    \    // we checked that\n    //   ((1) mod MOD1) mod 5 = 2\n    //   ((2) mod\
    \ MOD1) mod 5 = 3\n    //   ((3) mod MOD1) mod 5 = 4\n    long long _x = (long\
    \ long)x % (long long)MOD1;\n    if (_x < 0) {\n      _x += MOD1;\n    }\n   \
    \ long long diff = c1[i] - _x;\n    if (diff < 0)\n      diff += MOD1;\n    static\
    \ constexpr unsigned long long offset[5] = {0, 0, M1M2M3, 2 * M1M2M3,\n      \
    \                                               3 * M1M2M3};\n    x -= offset[diff\
    \ % 5];\n    c[i] = x;\n  }\n\n  return c;\n}\ntemplate <ModInt mint>\ninline\
    \ vector<mint> convolution_arb_mod(const vector<mint> &a,\n                  \
    \                      const vector<mint> &b) {\n  int n = a.size(), m = b.size();\n\
    \  if (!n || !m) {\n    return {};\n  }\n\n  if (min(n, m) <= 60) {\n    return\
    \ convolution_naive(a, b);\n  }\n\n  static constexpr long long MOD1 = 167772161;\
    \ // 2^25\n  static constexpr long long MOD2 = 469762049; // 2^26\n  static constexpr\
    \ long long MOD3 = 754974721; // 2^24\n\n  static constexpr long long INV12 =\
    \ inv_gcd(MOD1, MOD2);\n  static constexpr long long INV13 = inv_gcd(MOD1, MOD3);\n\
    \  static constexpr long long INV23 = inv_gcd(MOD2, MOD3);\n  static constexpr\
    \ long long INV13INV23 = INV13 * INV23 % MOD3;\n  static constexpr long long W1\
    \ = MOD1 % mint::imod();\n  static constexpr long long W2 = W1 * MOD2 % mint::imod();\n\
    \n  static constexpr int MAX_AB_BIT = 24;\n  static_assert(MOD1 % (1ull << MAX_AB_BIT)\
    \ == 1,\n                \"MOD1 isn't enough to support an array length of 2^24.\"\
    );\n  static_assert(MOD2 % (1ull << MAX_AB_BIT) == 1,\n                \"MOD2\
    \ isn't enough to support an array length of 2^24.\");\n  static_assert(MOD3 %\
    \ (1ull << MAX_AB_BIT) == 1,\n                \"MOD3 isn't enough to support an\
    \ array length of 2^24.\");\n  assert(n + m - 1 <= (1 << MAX_AB_BIT));\n\n  vector<long\
    \ long> _a(n), _b(m);\n  for (int i = 0; i < n; i++) {\n    _a[i] = a[i].val();\n\
    \  }\n  for (int i = 0; i < m; i++) {\n    _b[i] = b[i].val();\n  }\n\n  vector<long\
    \ long> c1 = convolution<MOD1>(_a, _b);\n  vector<long long> c2 = convolution<MOD2>(_a,\
    \ _b);\n  vector<long long> c3 = convolution<MOD3>(_a, _b);\n\n  vector<mint>\
    \ c(n + m - 1);\n  for (int i = 0; i < n + m - 1; i++) {\n    long long x = (c2[i]\
    \ + MOD2 - c1[i]) * INV12 % MOD2;\n    long long y =\n        ((c3[i] + MOD3 -\
    \ c1[i]) * INV13INV23 + (MOD3 - x) * INV23) % MOD3;\n    c[i] = c1[i] + x * W1\
    \ + y * W2;\n  }\n\n  return c;\n}\n} // namespace ntt\nnamespace fft {\ntemplate\
    \ <typename T>\n  requires is_floating_point_v<T>\ninline void fft(vector<complex<T>>\
    \ &a) {\n  int n = a.size(), L = 31 - __builtin_clz(n);\n  static vector<complex<long\
    \ double>> R(2, 1);\n  static vector<complex<T>> rt(2, 1); // (^ 10% faster if\
    \ double)\n  for (static int k = 2; k < n; k *= 2) {\n    R.resize(n);\n    rt.resize(n);\n\
    \    auto x = polar(1.0L, acos(-1.0L) / k);\n    for (int i = k; i < 2 * k; i++)\
    \ {\n      rt[i] = R[i] = i & 1 ? R[i / 2] * x : R[i / 2];\n    }\n  }\n  vector<int>\
    \ rev(n);\n  for (int i = 0; i < n; i++) {\n    rev[i] = (rev[i / 2] | (i & 1)\
    \ << L) / 2;\n  }\n  for (int i = 0; i < n; i++) {\n    if (i < rev[i]) {\n  \
    \    swap(a[i], a[rev[i]]);\n    }\n  }\n  for (int k = 1; k < n; k *= 2) {\n\
    \    for (int i = 0; i < n; i += 2 * k) {\n      for (int j = 0; j < k; j++) {\n\
    \        // complex<T> z = rt[j+k] * a[i+j+k]; // (25% faster if hand-rolled)\n\
    \        auto x = (T *)&rt[j + k], y = (T *)&a[i + j + k];\n        complex<T>\
    \ z(x[0] * y[0] - x[1] * y[1], x[0] * y[1] + x[1] * y[0]);\n        a[i + j +\
    \ k] = a[i + j] - z;\n        a[i + j] += z;\n      }\n    }\n  }\n}\ntemplate\
    \ <typename T>\n  requires is_arithmetic_v<T>\ninline vector<T> convolution(const\
    \ vector<T> &ta, const vector<T> &tb) {\n  int n = ta.size(), m = tb.size();\n\
    \  if (!n || !m) {\n    return {};\n  }\n  if (min(n, m) <= 60) {\n    return\
    \ convolution_naive(ta, tb);\n  }\n  vector<double> a(n), b(m);\n  for (int i\
    \ = 0; i < n; i++) {\n    a[i] = ta[i];\n  }\n  for (int i = 0; i < m; i++) {\n\
    \    b[i] = tb[i];\n  }\n  int z = bit_ceil((unsigned int)(n + m - 1));\n  vector<complex<double>>\
    \ in(z), out(z);\n  copy(a.begin(), a.end(), in.begin());\n  for (int i = 0; i\
    \ < m; i++) {\n    in[i].imag(b[i]);\n  }\n  fft(in);\n  for (complex<double>\
    \ &x : in) {\n    x *= x;\n  }\n  for (int i = 0; i < z; i++) {\n    out[i] =\
    \ in[-i & (z - 1)] - conj(in[i]);\n  }\n  fft(out);\n  vector<T> res(n + m - 1);\n\
    \  for (int i = 0; i < n + m - 1; i++) {\n    if constexpr (integral<T>) {\n \
    \     res[i] = imag(out[i]) / (4 * z) + 0.5;\n    } else {\n      res[i] = imag(out[i])\
    \ / (4 * z);\n    }\n  }\n  return res;\n}\ntemplate <concepts::broadly_integral\
    \ T>\ninline vector<T> convolution_sqrt(const vector<T> &a, const vector<T> &b)\
    \ {\n  int n = a.size(), m = b.size();\n  if (!n || !m) {\n    return {};\n  }\n\
    \  if (min(n, m) <= 60) {\n    return convolution_naive(a, b);\n  }\n  int z =\
    \ bit_ceil((unsigned int)(n + m - 1)), cut = 1 << 15;\n  vector<complex<double>>\
    \ L(z), R(z), outs(z), outl(z);\n  for (int i = 0; i < n; i++) {\n    L[i] = complex<double>(a[i]\
    \ >> 15, a[i] & ((1 << 15) - 1));\n  }\n  for (int i = 0; i < m; i++) {\n    R[i]\
    \ = complex<double>(b[i] >> 15, b[i] & ((1 << 15) - 1));\n  }\n  fft(L), fft(R);\n\
    \  for (int i = 0; i < z; i++) {\n    int j = -i & (z - 1);\n    outl[j] = (L[i]\
    \ + conj(L[j])) * R[i] / (2.0 * z);\n    outs[j] = (L[i] - conj(L[j])) * R[i]\
    \ / (2.0 * z) / 1i;\n  }\n  fft(outl), fft(outs);\n  vector<T> res(n + m - 1);\n\
    \  for (int i = 0; i < n + m - 1; i++) {\n    T av = (T)(real(outl[i]) + .5),\
    \ cv = (T)(imag(outs[i]) + .5);\n    T bv = (T)(imag(outl[i]) + .5) + (T)(real(outs[i])\
    \ + .5);\n    res[i] = (av * cut + bv) * cut + cv;\n  }\n  return res;\n}\ntemplate\
    \ <ModInt mint>\nvector<mint> convolution_arb_mod(const vector<mint> &a, const\
    \ vector<mint> &b) {\n  int n = a.size(), m = b.size();\n  if (!n || !m) {\n \
    \   return {};\n  }\n  if (min(n, m) <= 60) {\n    return convolution_naive(a,\
    \ b);\n  }\n  int z = bit_ceil((unsigned int)(n + m - 1)), cut = int(sqrt(mint::imod()));\n\
    \  vector<complex<double>> L(z), R(z), outs(z), outl(z);\n  for (int i = 0; i\
    \ < n; i++) {\n    L[i] = complex<double>(a[i].val() / cut, a[i].val() % cut);\n\
    \  }\n  for (int i = 0; i < m; i++) {\n    R[i] = complex<double>(b[i].val() /\
    \ cut, b[i].val() % cut);\n  }\n  fft(L), fft(R);\n  for (int i = 0; i < z; i++)\
    \ {\n    int j = -i & (z - 1);\n    outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0\
    \ * z);\n    outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * z) / 1i;\n  }\n  fft(outl),\
    \ fft(outs);\n  vector<mint> res(n + m - 1);\n  for (int i = 0; i < n + m - 1;\
    \ i++) {\n    long long av = (long long)(real(outl[i]) + .5),\n              cv\
    \ = (long long)(imag(outs[i]) + .5);\n    long long bv =\n        (long long)(imag(outl[i])\
    \ + .5) + (long long)(real(outs[i]) + .5);\n    res[i] = (av % mint::imod() *\
    \ cut + bv) % mint::imod() * cut + cv;\n  }\n  return res;\n}\ntemplate <typename\
    \ T>\n  requires is_floating_point_v<T>\nvector<complex<T>> convolution_complex(const\
    \ vector<complex<T>> &a,\n                                       const vector<complex<T>>\
    \ &b) {\n  int n = a.size(), m = b.size();\n  if (!a || !b) {\n    return {};\n\
    \  }\n  if (min(n, m) <= 60) {\n    return convolution_naive(a, b);\n  }\n  int\
    \ z = bit_ceil((unsigned int)(n + m - 1));\n  a.resize(z, 0);\n  b.resize(z, 0);\n\
    \  fft(a);\n  fft(b);\n  for (int i = 0; i < z; i++) {\n    a[i] *= b[i] / (T)z;\n\
    \  }\n  reverse(a.begin() + 1, a.end());\n  fft(a);\n  a.resize(n + m - 1);\n\
    \  return a;\n}\n} // namespace fft\n} // namespace internal::poly\nenum PolySetting\
    \ {\n  ntt,\n  fft,      // faster than fft_sqrt but less precision\n  fft_sqrt,\
    \ // uses sqrt to increase precision\n  fft_complex,\n};\ntemplate <typename T,\
    \ PolySetting poly_setting>\nstruct is_valid_setting : false_type {};\n// ntt\
    \ allowes ModInt or integral types\ntemplate <ModInt T> struct is_valid_setting<T,\
    \ PolySetting::ntt> : true_type {};\ntemplate <integral T>\n  requires internal::type_traits::is_64bit_or_less_v<T>\n\
    struct is_valid_setting<T, PolySetting::ntt> : true_type {};\n// fft_sqrt allow\
    \ broadly_integral types or ModInt\ntemplate <internal::concepts::broadly_integral\
    \ T>\nstruct is_valid_setting<T, PolySetting::fft_sqrt> : true_type {};\ntemplate\
    \ <ModInt T>\nstruct is_valid_setting<T, PolySetting::fft_sqrt> : true_type {};\n\
    // fft allow arithmetic types\ntemplate <typename T>\n  requires is_arithmetic_v<T>\n\
    struct is_valid_setting<T, PolySetting::fft> : true_type {};\n// fft_complex allows\
    \ complex numbers\ntemplate <typename T>\n  requires is_floating_point_v<T>\n\
    struct is_valid_setting<complex<T>, PolySetting::fft_complex> : true_type {};\n\
    \ntemplate <typename T, PolySetting poly_setting>\nconstexpr bool is_valid_setting_v\
    \ = is_valid_setting<T, poly_setting>::value;\n\ntemplate <typename T, PolySetting\
    \ poly_setting,\n          enable_if_t<is_valid_setting_v<T, poly_setting>, nullptr_t>\
    \ = nullptr>\nstruct Poly {\n  constexpr Poly() : _v(1, 0) {}\n  constexpr Poly(int\
    \ n) : _v(n) {}\n  constexpr Poly(vector<T> v) : _v(v) {}\n\n  constexpr int degree()\
    \ const { return _v.size() - 1; }\n  constexpr T operator[](int i) const { return\
    \ _v[i]; }\n  constexpr T &operator[](int i) { return _v[i]; }\n\n  constexpr\
    \ Poly &operator*=(const Poly &o) {\n    if constexpr (poly_setting == PolySetting::ntt)\
    \ {\n      if constexpr (ModInt<T>) {\n        int z = bit_ceil((unsigned int)(degree()\
    \ + o.degree() + 1));\n        if (StaticModInt<T> && T::is_prime_mod && (T::imod()\
    \ - 1) % z == 0) {\n          _v = internal::poly::ntt::convolution(_v, o._v);\n\
    \        } else {\n          _v = internal::poly::ntt::convolution_arb_mod(_v,\
    \ o._v);\n        }\n      } else { // integral\n        vector<long long> a(degree()\
    \ + 1), b(o.degree() + 1);\n        for (int i = 0; i <= degree(); i++) {\n  \
    \        a[i] = _v[i];\n        }\n        for (int i = 0; i <= o.degree(); i++)\
    \ {\n          b[i] = o._v[i];\n        }\n        vector<long long> res = internal::poly::ntt::convolution_ll(a,\
    \ b);\n        _v.resize(res.size());\n        for (size_t i = 0; i < res.size();\
    \ i++) {\n          _v[i] = res[i];\n        }\n      }\n    } else if constexpr\
    \ (poly_setting == PolySetting::fft_sqrt) {\n      if constexpr (ModInt<T>) {\n\
    \        _v = internal::poly::fft::convolution_arb_mod(_v, o._v);\n      } else\
    \ { // integral\n        _v = internal::poly::fft::convolution_sqrt(_v, o._v);\n\
    \      }\n    } else if constexpr (poly_setting == PolySetting::fft) {\n     \
    \ _v = internal::poly::fft::convolution(_v, o._v);\n    } else if constexpr (poly_setting\
    \ == PolySetting::fft_complex) {\n      _v = internal::poly::fft::convolution_complex(_v,\
    \ o._v);\n    }\n    return *this;\n  }\n  constexpr Poly operator*(const Poly\
    \ &o) const {\n    Poly res = *this;\n    return res *= o;\n  }\n\nprivate:\n\
    \  vector<T> _v;\n};\n} // namespace maomao90\n"
  code: "#pragma once\n\n#include <algorithm>\n#include <array>\n#include <bit>\n\
    #include <complex>\n#include <type_traits>\n#include <vector>\n\n#include \"library/internal/concepts.hpp\"\
    \n#include \"library/internal/math.hpp\"\n#include \"library/math/extended_gcd.hpp\"\
    \n#include \"library/math/modint.hpp\"\n\nnamespace maomao90 {\nusing namespace\
    \ std;\nnamespace internal::poly {\ntemplate <class T>\nvector<T> convolution_naive(const\
    \ vector<T> &a, const vector<T> &b) {\n  int n = a.size(), m = b.size();\n  vector<T>\
    \ ans(n + m - 1);\n  if (n < m) {\n    for (int j = 0; j < m; j++) {\n      for\
    \ (int i = 0; i < n; i++) {\n        ans[i + j] += a[i] * b[j];\n      }\n   \
    \ }\n  } else {\n    for (int i = 0; i < n; i++) {\n      for (int j = 0; j <\
    \ m; j++) {\n        ans[i + j] += a[i] * b[j];\n      }\n    }\n  }\n  return\
    \ ans;\n}\nnamespace ntt {\ntemplate <StaticModInt mint,\n          int g = internal::math::primitive_root<mint::imod()>>\n\
    struct ntt_info {\n  static constexpr int rank2 = __builtin_ctz(mint::umod() -\
    \ 1);\n  array<mint, rank2 + 1> root;  // root[i]^(2^i) == 1\n  array<mint, rank2\
    \ + 1> iroot; // root[i] * iroot[i] == 1\n\n  array<mint, max(0, rank2 - 2 + 1)>\
    \ rate2;\n  array<mint, max(0, rank2 - 2 + 1)> irate2;\n\n  array<mint, max(0,\
    \ rank2 - 3 + 1)> rate3;\n  array<mint, max(0, rank2 - 3 + 1)> irate3;\n\n  ntt_info()\
    \ {\n    root[rank2] = mint(g).pow((mint::imod() - 1) >> rank2);\n    iroot[rank2]\
    \ = root[rank2].inv();\n    for (int i = rank2 - 1; i >= 0; i--) {\n      root[i]\
    \ = root[i + 1] * root[i + 1];\n      iroot[i] = iroot[i + 1] * iroot[i + 1];\n\
    \    }\n\n    {\n      mint prod = 1, iprod = 1;\n      for (int i = 0; i <= rank2\
    \ - 2; i++) {\n        rate2[i] = root[i + 2] * prod;\n        irate2[i] = iroot[i\
    \ + 2] * iprod;\n        prod *= iroot[i + 2];\n        iprod *= root[i + 2];\n\
    \      }\n    }\n    {\n      mint prod = 1, iprod = 1;\n      for (int i = 0;\
    \ i <= rank2 - 3; i++) {\n        rate3[i] = root[i + 3] * prod;\n        irate3[i]\
    \ = iroot[i + 3] * iprod;\n        prod *= iroot[i + 3];\n        iprod *= root[i\
    \ + 3];\n      }\n    }\n  }\n};\n\ntemplate <StaticModInt mint> void butterfly(vector<mint>\
    \ &a) {\n  int n = a.size();\n  int h = __builtin_ctz((unsigned int)n);\n\n  static\
    \ const ntt_info<mint> info;\n\n  int len = 0; // a[i, i+(n>>len), i+2*(n>>len),\
    \ ..] is transformed\n  while (len < h) {\n    if (h - len == 1) {\n      int\
    \ p = 1 << (h - len - 1);\n      mint rot = 1;\n      for (int s = 0; s < (1 <<\
    \ len); s++) {\n        int offset = s << (h - len);\n        for (int i = 0;\
    \ i < p; i++) {\n          auto l = a[i + offset];\n          auto r = a[i + offset\
    \ + p] * rot;\n          a[i + offset] = l + r;\n          a[i + offset + p] =\
    \ l - r;\n        }\n        if (s + 1 != (1 << len)) {\n          rot *= info.rate2[countr_zero(~(unsigned\
    \ int)(s))];\n        }\n      }\n      len++;\n    } else {\n      // 4-base\n\
    \      int p = 1 << (h - len - 2);\n      mint rot = 1, imag = info.root[2];\n\
    \      for (int s = 0; s < (1 << len); s++) {\n        mint rot2 = rot * rot;\n\
    \        mint rot3 = rot2 * rot;\n        int offset = s << (h - len);\n     \
    \   for (int i = 0; i < p; i++) {\n          auto mod2 = 1ull * mint::imod() *\
    \ mint::imod();\n          auto a0 = 1ull * a[i + offset].val();\n          auto\
    \ a1 = 1ull * a[i + offset + p].val() * rot.val();\n          auto a2 = 1ull *\
    \ a[i + offset + 2 * p].val() * rot2.val();\n          auto a3 = 1ull * a[i +\
    \ offset + 3 * p].val() * rot3.val();\n          auto a1na3imag = 1ull * mint(a1\
    \ + mod2 - a3).val() * imag.val();\n          auto na2 = mod2 - a2;\n        \
    \  a[i + offset] = a0 + a2 + a1 + a3;\n          a[i + offset + 1 * p] = a0 +\
    \ a2 + (2 * mod2 - (a1 + a3));\n          a[i + offset + 2 * p] = a0 + na2 + a1na3imag;\n\
    \          a[i + offset + 3 * p] = a0 + na2 + (mod2 - a1na3imag);\n        }\n\
    \        if (s + 1 != (1 << len)) {\n          rot *= info.rate3[countr_zero(~(unsigned\
    \ int)(s))];\n        }\n      }\n      len += 2;\n    }\n  }\n}\n\ntemplate <StaticModInt\
    \ mint> void butterfly_inv(vector<mint> &a) {\n  int n = a.size();\n  int h =\
    \ __builtin_ctz((unsigned int)n);\n\n  static const ntt_info<mint> info;\n\n \
    \ int len = h; // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed\n  while (len)\
    \ {\n    if (len == 1) {\n      int p = 1 << (h - len);\n      mint irot = 1;\n\
    \      for (int s = 0; s < (1 << (len - 1)); s++) {\n        int offset = s <<\
    \ (h - len + 1);\n        for (int i = 0; i < p; i++) {\n          auto l = a[i\
    \ + offset];\n          auto r = a[i + offset + p];\n          a[i + offset] =\
    \ l + r;\n          a[i + offset + p] =\n              (unsigned long long)(mint::imod()\
    \ + l.val() - r.val()) *\n              irot.val();\n          ;\n        }\n\
    \        if (s + 1 != (1 << (len - 1))) {\n          irot *= info.irate2[countr_zero(~(unsigned\
    \ int)(s))];\n        }\n      }\n      len--;\n    } else {\n      // 4-base\n\
    \      int p = 1 << (h - len);\n      mint irot = 1, iimag = info.iroot[2];\n\
    \      for (int s = 0; s < (1 << (len - 2)); s++) {\n        mint irot2 = irot\
    \ * irot;\n        mint irot3 = irot2 * irot;\n        int offset = s << (h -\
    \ len + 2);\n        for (int i = 0; i < p; i++) {\n          auto a0 = 1ull *\
    \ a[i + offset + 0 * p].val();\n          auto a1 = 1ull * a[i + offset + 1 *\
    \ p].val();\n          auto a2 = 1ull * a[i + offset + 2 * p].val();\n       \
    \   auto a3 = 1ull * a[i + offset + 3 * p].val();\n\n          auto a2na3iimag\
    \ =\n              1ull * mint((mint::imod() + a2 - a3) * iimag.val()).val();\n\
    \n          a[i + offset] = a0 + a1 + a2 + a3;\n          a[i + offset + 1 * p]\
    \ =\n              (a0 + (mint::imod() - a1) + a2na3iimag) * irot.val();\n   \
    \       a[i + offset + 2 * p] =\n              (a0 + a1 + (mint::imod() - a2)\
    \ + (mint::imod() - a3)) *\n              irot2.val();\n          a[i + offset\
    \ + 3 * p] =\n              (a0 + (mint::imod() - a1) + (mint::imod() - a2na3iimag))\
    \ *\n              irot3.val();\n        }\n        if (s + 1 != (1 << (len -\
    \ 2))) {\n          irot *= info.irate3[countr_zero(~(unsigned int)(s))];\n  \
    \      }\n      }\n      len -= 2;\n    }\n  }\n}\ntemplate <StaticModInt mint>\n\
    vector<mint> convolution_ntt(vector<mint> a, vector<mint> b) {\n  int n = a.size(),\
    \ m = b.size();\n  int z = bit_ceil((unsigned int)(n + m - 1));\n  a.resize(z);\n\
    \  butterfly(a);\n  b.resize(z);\n  butterfly(b);\n  for (int i = 0; i < z; i++)\
    \ {\n    a[i] *= b[i];\n  }\n  butterfly_inv(a);\n  a.resize(n + m - 1);\n  mint\
    \ iz = mint(z).inv();\n  for (int i = 0; i < n + m - 1; i++)\n    a[i] *= iz;\n\
    \  return a;\n}\ntemplate <StaticModInt mint>\nvector<mint> convolution(const\
    \ vector<mint> &a, const vector<mint> &b) {\n  int n = a.size(), m = b.size();\n\
    \  if (!n || !m) {\n    return {};\n  }\n\n  int z = bit_ceil((unsigned int)(n\
    \ + m - 1));\n  assert((mint::imod() - 1) % z == 0);\n\n  if (min(n, m) <= 60)\
    \ {\n    return convolution_naive(a, b);\n  }\n  return convolution_ntt(a, b);\n\
    }\ntemplate <int mod, class T>\n  requires is_integral_v<T>\nvector<T> convolution(const\
    \ vector<T> &a, const vector<T> &b) {\n  int n = a.size(), m = b.size();\n  if\
    \ (!n || !m)\n    return {};\n\n  using mint = static_modint<mod>;\n\n  int z\
    \ = bit_ceil((unsigned int)(n + m - 1));\n  assert((mint::imod() - 1) % z == 0);\n\
    \n  vector<mint> a2(n), b2(m);\n  for (int i = 0; i < n; i++) {\n    a2[i] = mint(a[i]);\n\
    \  }\n  for (int i = 0; i < m; i++) {\n    b2[i] = mint(b[i]);\n  }\n  auto c2\
    \ = convolution(std::move(a2), std::move(b2));\n  vector<T> c(n + m - 1);\n  for\
    \ (int i = 0; i < n + m - 1; i++) {\n    c[i] = c2[i].val();\n  }\n  return c;\n\
    }\ninline vector<long long> convolution_ll(const vector<long long> &a,\n     \
    \                                   const vector<long long> &b) {\n  int n = a.size(),\
    \ m = b.size();\n  if (!n || !m) {\n    return {};\n  }\n\n  if (min(n, m) <=\
    \ 60) {\n    return convolution_naive(a, b);\n  }\n\n  static constexpr unsigned\
    \ long long MOD1 = 754974721; // 2^24\n  static constexpr unsigned long long MOD2\
    \ = 167772161; // 2^25\n  static constexpr unsigned long long MOD3 = 469762049;\
    \ // 2^26\n  static constexpr unsigned long long M2M3 = MOD2 * MOD3;\n  static\
    \ constexpr unsigned long long M1M3 = MOD1 * MOD3;\n  static constexpr unsigned\
    \ long long M1M2 = MOD1 * MOD2;\n  static constexpr unsigned long long M1M2M3\
    \ = MOD1 * MOD2 * MOD3;\n\n  static constexpr unsigned long long i1 =\n      inv_gcd<long\
    \ long>(MOD2 * MOD3, MOD1);\n  static constexpr unsigned long long i2 =\n    \
    \  inv_gcd<long long>(MOD1 * MOD3, MOD2);\n  static constexpr unsigned long long\
    \ i3 =\n      inv_gcd<long long>(MOD1 * MOD2, MOD3);\n\n  static constexpr int\
    \ MAX_AB_BIT = 24;\n  static_assert(MOD1 % (1ull << MAX_AB_BIT) == 1,\n      \
    \          \"MOD1 isn't enough to support an array length of 2^24.\");\n  static_assert(MOD2\
    \ % (1ull << MAX_AB_BIT) == 1,\n                \"MOD2 isn't enough to support\
    \ an array length of 2^24.\");\n  static_assert(MOD3 % (1ull << MAX_AB_BIT) ==\
    \ 1,\n                \"MOD3 isn't enough to support an array length of 2^24.\"\
    );\n  assert(n + m - 1 <= (1 << MAX_AB_BIT));\n\n  vector<long long> c1 = convolution<MOD1>(a,\
    \ b);\n  vector<long long> c2 = convolution<MOD2>(a, b);\n  vector<long long>\
    \ c3 = convolution<MOD3>(a, b);\n\n  vector<long long> c(n + m - 1);\n  for (int\
    \ i = 0; i < n + m - 1; i++) {\n    unsigned long long x = 0;\n    x += (c1[i]\
    \ * i1) % MOD1 * M2M3;\n    x += (c2[i] * i2) % MOD2 * M1M3;\n    x += (c3[i]\
    \ * i3) % MOD3 * M1M2;\n    // B = 2^63, -B <= x, r(real value) < B\n    // (x,\
    \ x - M, x - 2M, or x - 3M) = r (mod 2B)\n    // r = c1[i] (mod MOD1)\n    //\
    \ focus on MOD1\n    // r = x, x - M', x - 2M', x - 3M' (M' = M % 2^64) (mod 2B)\n\
    \    // r = x,\n    //     x - M' + (0 or 2B),\n    //     x - 2M' + (0, 2B or\
    \ 4B),\n    //     x - 3M' + (0, 2B, 4B or 6B) (without mod!)\n    // (r - x)\
    \ = 0, (0)\n    //           - M' + (0 or 2B), (1)\n    //           -2M' + (0\
    \ or 2B or 4B), (2)\n    //           -3M' + (0 or 2B or 4B or 6B) (3) (mod MOD1)\n\
    \    // we checked that\n    //   ((1) mod MOD1) mod 5 = 2\n    //   ((2) mod\
    \ MOD1) mod 5 = 3\n    //   ((3) mod MOD1) mod 5 = 4\n    long long _x = (long\
    \ long)x % (long long)MOD1;\n    if (_x < 0) {\n      _x += MOD1;\n    }\n   \
    \ long long diff = c1[i] - _x;\n    if (diff < 0)\n      diff += MOD1;\n    static\
    \ constexpr unsigned long long offset[5] = {0, 0, M1M2M3, 2 * M1M2M3,\n      \
    \                                               3 * M1M2M3};\n    x -= offset[diff\
    \ % 5];\n    c[i] = x;\n  }\n\n  return c;\n}\ntemplate <ModInt mint>\ninline\
    \ vector<mint> convolution_arb_mod(const vector<mint> &a,\n                  \
    \                      const vector<mint> &b) {\n  int n = a.size(), m = b.size();\n\
    \  if (!n || !m) {\n    return {};\n  }\n\n  if (min(n, m) <= 60) {\n    return\
    \ convolution_naive(a, b);\n  }\n\n  static constexpr long long MOD1 = 167772161;\
    \ // 2^25\n  static constexpr long long MOD2 = 469762049; // 2^26\n  static constexpr\
    \ long long MOD3 = 754974721; // 2^24\n\n  static constexpr long long INV12 =\
    \ inv_gcd(MOD1, MOD2);\n  static constexpr long long INV13 = inv_gcd(MOD1, MOD3);\n\
    \  static constexpr long long INV23 = inv_gcd(MOD2, MOD3);\n  static constexpr\
    \ long long INV13INV23 = INV13 * INV23 % MOD3;\n  static constexpr long long W1\
    \ = MOD1 % mint::imod();\n  static constexpr long long W2 = W1 * MOD2 % mint::imod();\n\
    \n  static constexpr int MAX_AB_BIT = 24;\n  static_assert(MOD1 % (1ull << MAX_AB_BIT)\
    \ == 1,\n                \"MOD1 isn't enough to support an array length of 2^24.\"\
    );\n  static_assert(MOD2 % (1ull << MAX_AB_BIT) == 1,\n                \"MOD2\
    \ isn't enough to support an array length of 2^24.\");\n  static_assert(MOD3 %\
    \ (1ull << MAX_AB_BIT) == 1,\n                \"MOD3 isn't enough to support an\
    \ array length of 2^24.\");\n  assert(n + m - 1 <= (1 << MAX_AB_BIT));\n\n  vector<long\
    \ long> _a(n), _b(m);\n  for (int i = 0; i < n; i++) {\n    _a[i] = a[i].val();\n\
    \  }\n  for (int i = 0; i < m; i++) {\n    _b[i] = b[i].val();\n  }\n\n  vector<long\
    \ long> c1 = convolution<MOD1>(_a, _b);\n  vector<long long> c2 = convolution<MOD2>(_a,\
    \ _b);\n  vector<long long> c3 = convolution<MOD3>(_a, _b);\n\n  vector<mint>\
    \ c(n + m - 1);\n  for (int i = 0; i < n + m - 1; i++) {\n    long long x = (c2[i]\
    \ + MOD2 - c1[i]) * INV12 % MOD2;\n    long long y =\n        ((c3[i] + MOD3 -\
    \ c1[i]) * INV13INV23 + (MOD3 - x) * INV23) % MOD3;\n    c[i] = c1[i] + x * W1\
    \ + y * W2;\n  }\n\n  return c;\n}\n} // namespace ntt\nnamespace fft {\ntemplate\
    \ <typename T>\n  requires is_floating_point_v<T>\ninline void fft(vector<complex<T>>\
    \ &a) {\n  int n = a.size(), L = 31 - __builtin_clz(n);\n  static vector<complex<long\
    \ double>> R(2, 1);\n  static vector<complex<T>> rt(2, 1); // (^ 10% faster if\
    \ double)\n  for (static int k = 2; k < n; k *= 2) {\n    R.resize(n);\n    rt.resize(n);\n\
    \    auto x = polar(1.0L, acos(-1.0L) / k);\n    for (int i = k; i < 2 * k; i++)\
    \ {\n      rt[i] = R[i] = i & 1 ? R[i / 2] * x : R[i / 2];\n    }\n  }\n  vector<int>\
    \ rev(n);\n  for (int i = 0; i < n; i++) {\n    rev[i] = (rev[i / 2] | (i & 1)\
    \ << L) / 2;\n  }\n  for (int i = 0; i < n; i++) {\n    if (i < rev[i]) {\n  \
    \    swap(a[i], a[rev[i]]);\n    }\n  }\n  for (int k = 1; k < n; k *= 2) {\n\
    \    for (int i = 0; i < n; i += 2 * k) {\n      for (int j = 0; j < k; j++) {\n\
    \        // complex<T> z = rt[j+k] * a[i+j+k]; // (25% faster if hand-rolled)\n\
    \        auto x = (T *)&rt[j + k], y = (T *)&a[i + j + k];\n        complex<T>\
    \ z(x[0] * y[0] - x[1] * y[1], x[0] * y[1] + x[1] * y[0]);\n        a[i + j +\
    \ k] = a[i + j] - z;\n        a[i + j] += z;\n      }\n    }\n  }\n}\ntemplate\
    \ <typename T>\n  requires is_arithmetic_v<T>\ninline vector<T> convolution(const\
    \ vector<T> &ta, const vector<T> &tb) {\n  int n = ta.size(), m = tb.size();\n\
    \  if (!n || !m) {\n    return {};\n  }\n  if (min(n, m) <= 60) {\n    return\
    \ convolution_naive(ta, tb);\n  }\n  vector<double> a(n), b(m);\n  for (int i\
    \ = 0; i < n; i++) {\n    a[i] = ta[i];\n  }\n  for (int i = 0; i < m; i++) {\n\
    \    b[i] = tb[i];\n  }\n  int z = bit_ceil((unsigned int)(n + m - 1));\n  vector<complex<double>>\
    \ in(z), out(z);\n  copy(a.begin(), a.end(), in.begin());\n  for (int i = 0; i\
    \ < m; i++) {\n    in[i].imag(b[i]);\n  }\n  fft(in);\n  for (complex<double>\
    \ &x : in) {\n    x *= x;\n  }\n  for (int i = 0; i < z; i++) {\n    out[i] =\
    \ in[-i & (z - 1)] - conj(in[i]);\n  }\n  fft(out);\n  vector<T> res(n + m - 1);\n\
    \  for (int i = 0; i < n + m - 1; i++) {\n    if constexpr (integral<T>) {\n \
    \     res[i] = imag(out[i]) / (4 * z) + 0.5;\n    } else {\n      res[i] = imag(out[i])\
    \ / (4 * z);\n    }\n  }\n  return res;\n}\ntemplate <concepts::broadly_integral\
    \ T>\ninline vector<T> convolution_sqrt(const vector<T> &a, const vector<T> &b)\
    \ {\n  int n = a.size(), m = b.size();\n  if (!n || !m) {\n    return {};\n  }\n\
    \  if (min(n, m) <= 60) {\n    return convolution_naive(a, b);\n  }\n  int z =\
    \ bit_ceil((unsigned int)(n + m - 1)), cut = 1 << 15;\n  vector<complex<double>>\
    \ L(z), R(z), outs(z), outl(z);\n  for (int i = 0; i < n; i++) {\n    L[i] = complex<double>(a[i]\
    \ >> 15, a[i] & ((1 << 15) - 1));\n  }\n  for (int i = 0; i < m; i++) {\n    R[i]\
    \ = complex<double>(b[i] >> 15, b[i] & ((1 << 15) - 1));\n  }\n  fft(L), fft(R);\n\
    \  for (int i = 0; i < z; i++) {\n    int j = -i & (z - 1);\n    outl[j] = (L[i]\
    \ + conj(L[j])) * R[i] / (2.0 * z);\n    outs[j] = (L[i] - conj(L[j])) * R[i]\
    \ / (2.0 * z) / 1i;\n  }\n  fft(outl), fft(outs);\n  vector<T> res(n + m - 1);\n\
    \  for (int i = 0; i < n + m - 1; i++) {\n    T av = (T)(real(outl[i]) + .5),\
    \ cv = (T)(imag(outs[i]) + .5);\n    T bv = (T)(imag(outl[i]) + .5) + (T)(real(outs[i])\
    \ + .5);\n    res[i] = (av * cut + bv) * cut + cv;\n  }\n  return res;\n}\ntemplate\
    \ <ModInt mint>\nvector<mint> convolution_arb_mod(const vector<mint> &a, const\
    \ vector<mint> &b) {\n  int n = a.size(), m = b.size();\n  if (!n || !m) {\n \
    \   return {};\n  }\n  if (min(n, m) <= 60) {\n    return convolution_naive(a,\
    \ b);\n  }\n  int z = bit_ceil((unsigned int)(n + m - 1)), cut = int(sqrt(mint::imod()));\n\
    \  vector<complex<double>> L(z), R(z), outs(z), outl(z);\n  for (int i = 0; i\
    \ < n; i++) {\n    L[i] = complex<double>(a[i].val() / cut, a[i].val() % cut);\n\
    \  }\n  for (int i = 0; i < m; i++) {\n    R[i] = complex<double>(b[i].val() /\
    \ cut, b[i].val() % cut);\n  }\n  fft(L), fft(R);\n  for (int i = 0; i < z; i++)\
    \ {\n    int j = -i & (z - 1);\n    outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0\
    \ * z);\n    outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * z) / 1i;\n  }\n  fft(outl),\
    \ fft(outs);\n  vector<mint> res(n + m - 1);\n  for (int i = 0; i < n + m - 1;\
    \ i++) {\n    long long av = (long long)(real(outl[i]) + .5),\n              cv\
    \ = (long long)(imag(outs[i]) + .5);\n    long long bv =\n        (long long)(imag(outl[i])\
    \ + .5) + (long long)(real(outs[i]) + .5);\n    res[i] = (av % mint::imod() *\
    \ cut + bv) % mint::imod() * cut + cv;\n  }\n  return res;\n}\ntemplate <typename\
    \ T>\n  requires is_floating_point_v<T>\nvector<complex<T>> convolution_complex(const\
    \ vector<complex<T>> &a,\n                                       const vector<complex<T>>\
    \ &b) {\n  int n = a.size(), m = b.size();\n  if (!a || !b) {\n    return {};\n\
    \  }\n  if (min(n, m) <= 60) {\n    return convolution_naive(a, b);\n  }\n  int\
    \ z = bit_ceil((unsigned int)(n + m - 1));\n  a.resize(z, 0);\n  b.resize(z, 0);\n\
    \  fft(a);\n  fft(b);\n  for (int i = 0; i < z; i++) {\n    a[i] *= b[i] / (T)z;\n\
    \  }\n  reverse(a.begin() + 1, a.end());\n  fft(a);\n  a.resize(n + m - 1);\n\
    \  return a;\n}\n} // namespace fft\n} // namespace internal::poly\nenum PolySetting\
    \ {\n  ntt,\n  fft,      // faster than fft_sqrt but less precision\n  fft_sqrt,\
    \ // uses sqrt to increase precision\n  fft_complex,\n};\ntemplate <typename T,\
    \ PolySetting poly_setting>\nstruct is_valid_setting : false_type {};\n// ntt\
    \ allowes ModInt or integral types\ntemplate <ModInt T> struct is_valid_setting<T,\
    \ PolySetting::ntt> : true_type {};\ntemplate <integral T>\n  requires internal::type_traits::is_64bit_or_less_v<T>\n\
    struct is_valid_setting<T, PolySetting::ntt> : true_type {};\n// fft_sqrt allow\
    \ broadly_integral types or ModInt\ntemplate <internal::concepts::broadly_integral\
    \ T>\nstruct is_valid_setting<T, PolySetting::fft_sqrt> : true_type {};\ntemplate\
    \ <ModInt T>\nstruct is_valid_setting<T, PolySetting::fft_sqrt> : true_type {};\n\
    // fft allow arithmetic types\ntemplate <typename T>\n  requires is_arithmetic_v<T>\n\
    struct is_valid_setting<T, PolySetting::fft> : true_type {};\n// fft_complex allows\
    \ complex numbers\ntemplate <typename T>\n  requires is_floating_point_v<T>\n\
    struct is_valid_setting<complex<T>, PolySetting::fft_complex> : true_type {};\n\
    \ntemplate <typename T, PolySetting poly_setting>\nconstexpr bool is_valid_setting_v\
    \ = is_valid_setting<T, poly_setting>::value;\n\ntemplate <typename T, PolySetting\
    \ poly_setting,\n          enable_if_t<is_valid_setting_v<T, poly_setting>, nullptr_t>\
    \ = nullptr>\nstruct Poly {\n  constexpr Poly() : _v(1, 0) {}\n  constexpr Poly(int\
    \ n) : _v(n) {}\n  constexpr Poly(vector<T> v) : _v(v) {}\n\n  constexpr int degree()\
    \ const { return _v.size() - 1; }\n  constexpr T operator[](int i) const { return\
    \ _v[i]; }\n  constexpr T &operator[](int i) { return _v[i]; }\n\n  constexpr\
    \ Poly &operator*=(const Poly &o) {\n    if constexpr (poly_setting == PolySetting::ntt)\
    \ {\n      if constexpr (ModInt<T>) {\n        int z = bit_ceil((unsigned int)(degree()\
    \ + o.degree() + 1));\n        if (StaticModInt<T> && T::is_prime_mod && (T::imod()\
    \ - 1) % z == 0) {\n          _v = internal::poly::ntt::convolution(_v, o._v);\n\
    \        } else {\n          _v = internal::poly::ntt::convolution_arb_mod(_v,\
    \ o._v);\n        }\n      } else { // integral\n        vector<long long> a(degree()\
    \ + 1), b(o.degree() + 1);\n        for (int i = 0; i <= degree(); i++) {\n  \
    \        a[i] = _v[i];\n        }\n        for (int i = 0; i <= o.degree(); i++)\
    \ {\n          b[i] = o._v[i];\n        }\n        vector<long long> res = internal::poly::ntt::convolution_ll(a,\
    \ b);\n        _v.resize(res.size());\n        for (size_t i = 0; i < res.size();\
    \ i++) {\n          _v[i] = res[i];\n        }\n      }\n    } else if constexpr\
    \ (poly_setting == PolySetting::fft_sqrt) {\n      if constexpr (ModInt<T>) {\n\
    \        _v = internal::poly::fft::convolution_arb_mod(_v, o._v);\n      } else\
    \ { // integral\n        _v = internal::poly::fft::convolution_sqrt(_v, o._v);\n\
    \      }\n    } else if constexpr (poly_setting == PolySetting::fft) {\n     \
    \ _v = internal::poly::fft::convolution(_v, o._v);\n    } else if constexpr (poly_setting\
    \ == PolySetting::fft_complex) {\n      _v = internal::poly::fft::convolution_complex(_v,\
    \ o._v);\n    }\n    return *this;\n  }\n  constexpr Poly operator*(const Poly\
    \ &o) const {\n    Poly res = *this;\n    return res *= o;\n  }\n\nprivate:\n\
    \  vector<T> _v;\n};\n} // namespace maomao90\n"
  dependsOn:
  - library/internal/concepts.hpp
  - library/internal/type_traits.hpp
  - library/internal/math.hpp
  - library/math/extended_gcd.hpp
  - library/math/modint.hpp
  - library/math/primality_test.hpp
  - library/math/pow_mod.hpp
  isVerificationFile: false
  path: library/math/poly.hpp
  requiredBy: []
  timestamp: '2024-12-13 23:21:45+08:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/convolution/convolution_1000000007.test.cpp
  - verify/yosupo/convolution/convolution_fft.test.cpp
  - verify/yosupo/convolution/convolution.test.cpp
  - verify/yosupo/convolution/convolution_fft_1000000007.test.cpp
documentation_of: library/math/poly.hpp
layout: document
title: Poly
---
