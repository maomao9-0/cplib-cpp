---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: library/internal/concepts.hpp
    title: library/internal/concepts.hpp
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
    path: library/math/multiplicative_function.hpp
    title: Multiplicative Function
  - icon: ':heavy_check_mark:'
    path: library/math/pow_mod.hpp
    title: Pow Mod
  - icon: ':heavy_check_mark:'
    path: library/math/primality_test.hpp
    title: Primality Test
  _extendedRequiredBy: []
  _extendedVerifiedWith: []
  _isVerificationFailed: false
  _pathExtension: cpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    '*NOT_SPECIAL_COMMENTS*': ''
    PROBLEM: https://judge.yosupo.jp/problem/sum_of_multiplicative_function_large
    links:
    - https://judge.yosupo.jp/problem/sum_of_multiplicative_function_large
  bundledCode: "#line 1 \"verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp\"\
    \n#define PROBLEM                                                            \
    \    \\\n  \"https://judge.yosupo.jp/problem/sum_of_multiplicative_function_large\"\
    \n\n#include <iostream>\n#include <vector>\nusing namespace std;\n\n#line 2 \"\
    library/math/modint.hpp\"\n\n#include <cassert>\n#include <concepts>\n#line 6\
    \ \"library/math/modint.hpp\"\n#include <type_traits>\n\n#line 2 \"library/internal/concepts.hpp\"\
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
    };\n} // namespace maomao90::internal::concepts\n#line 2 \"library/math/extended_gcd.hpp\"\
    \n\n#line 5 \"library/math/extended_gcd.hpp\"\n\n#line 7 \"library/math/extended_gcd.hpp\"\
    \n\nnamespace maomao90 {\nusing namespace std;\ntemplate <signed_integral T>\n\
    \  requires internal::type_traits::is_64bit_or_less_v<T>\nconstexpr T inv_gcd(T\
    \ x, T mod) {\n  using U = internal::type_traits::safely_multipliable_t<T>;\n\
    \  U a = mod, b = x, va = 0, vb = 1;\n  while (b) {\n    T k = a / b;\n    a -=\
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
    \ % 2 == 0)\n    d /= 2;\n  for (int i = 0; i < size; i++) {\n    T a = bases[i];\n\
    \    if (a % n == 0) {\n      continue;\n    }\n    T t = d, y = pow_mod<T, T>(a,\
    \ t, n);\n    while (t != n - 1 && y != 1 && y != n - 1) {\n      y = (U)y * y\
    \ % n;\n      t <<= 1;\n    }\n    if (y != n - 1 && t % 2 == 0) {\n      return\
    \ false;\n    }\n  }\n  return true;\n}\ntemplate <unsigned_integral T>\n  requires\
    \ internal::type_traits::is_64bit_or_less_v<T>\nconstexpr bool is_prime(T n) {\n\
    \  if constexpr (internal::type_traits::is_32bit_or_less_v<T>) {\n    T bases[3]\
    \ = {2, 7, 61};\n    return miller_rabin<T>(n, bases, 3);\n  } else {\n    T bases[7]\
    \ = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};\n    return miller_rabin<T>(n,\
    \ bases, 7);\n  }\n}\n} // namespace maomao90\n#line 12 \"library/math/modint.hpp\"\
    \n\nnamespace maomao90 {\nusing namespace std;\ntemplate <typename T>\nconcept\
    \ ModInt =\n    requires(T a, long long p) {\n      { T::imod() } -> same_as<typename\
    \ T::mod_type>;\n      { T::umod() } -> same_as<typename T::umod_type>;\n    \
    \  a++;\n      a--;\n      ++a;\n      --a;\n      +a;\n      -a;\n      a.pow(p);\n\
    \      a.inv();\n    } && internal::concepts::Addable<T> &&\n    internal::concepts::Subtractable<T>\
    \ &&\n    internal::concepts::Multipliable<T> && internal::concepts::Dividable<T>\
    \ &&\n    equality_comparable<T>;\n\ntemplate <auto mod = 998244353, enable_if_t<(mod\
    \ >= 1), nullptr_t> = nullptr>\n  requires signed_integral<decltype(mod)> &&\n\
    \           internal::type_traits::is_64bit_or_less_v<decltype(mod)>\nstruct static_modint\
    \ {\nprivate:\n  using M = decltype(mod);\n  using UM = make_unsigned_t<M>;\n\
    \  using BM = internal::type_traits::safely_multipliable_t<M>;\n\npublic:\n  using\
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
    \    return os << o._v;\n  }\n\nprivate:\n  UM _v;\n  static constexpr bool is_prime_mod\
    \ = is_prime(umod());\n};\n} // namespace maomao90\n#line 2 \"library/math/multiplicative_function.hpp\"\
    \n\n#line 6 \"library/math/multiplicative_function.hpp\"\n\n#line 8 \"library/math/multiplicative_function.hpp\"\
    \n\nnamespace maomao90 {\nusing namespace std;\n// sumg is a vector of size 2\
    \ * m where sumg[i] represents prefix sum of\n// g from 2 to i + 1 for i < m,\
    \ otherwise, sum from 2 to n / (2 * m - i)\n// g is a function that returns the\
    \ value of g(prime)\n// returns prefix sum of g at prime indices with the same\
    \ range as sumg\ntemplate <typename T, typename G>\n  requires requires(G g, long\
    \ long prime) {\n    { g(prime) } -> same_as<T>;\n  } && internal::concepts::Addable<T>\
    \ && internal::concepts::Subtractable<T> &&\n           internal::concepts::Multipliable<T>\n\
    vector<T> lucy_dp(long long n, vector<T> sumg, G g) {\n  assert(sumg.size() %\
    \ 2 == 0);\n  long long m = sumg.size() / 2;\n  assert(m * m <= n && (m + 1) *\
    \ (m + 1) > n);\n  vector<bool> is_prime(m + 1);\n  for (int i = 2; i <= m; i++)\
    \ {\n    is_prime[i] = 1;\n  }\n  for (int i = 2; i * i <= m; i++) {\n    if (!is_prime[i])\
    \ {\n      continue;\n    }\n    for (int j = i * i; j <= m; j += i) {\n     \
    \ is_prime[j] = 0;\n    }\n  }\n  vector<long long> div(m + 1);\n  vector<T> sumgp\
    \ = sumg;\n  for (int i = 1; i <= m; i++) {\n    div[i] = n / i;\n  }\n  for (int\
    \ prime = 2; prime <= m; prime++) {\n    if (!is_prime[prime]) {\n      continue;\n\
    \    }\n    long long prime_squared = (long long)prime * prime, iprime = prime;\n\
    \    for (int i = 1; i <= m; i++, iprime += prime) {\n      if (div[i] < prime_squared)\
    \ {\n        break;\n      }\n      T div_val =\n          iprime <= m ? sumgp[2\
    \ * m - iprime] : sumgp[div[i] / prime - 1];\n      sumgp[2 * m - i] -= g(prime)\
    \ * (div_val - sumgp[prime - 2]);\n    }\n    for (int i = m; i >= prime_squared;\
    \ i--) {\n      sumgp[i - 1] -= g(prime) * (sumgp[i / prime - 1] - sumgp[prime\
    \ - 2]);\n    }\n  }\n  return sumgp;\n}\ntemplate <typename T, typename F>\n\
    \  requires requires(F f, long long prime, int power) {\n    { f(prime, power)\
    \ } -> same_as<T>;\n  } && internal::concepts::Addable<T> && internal::concepts::Subtractable<T>\
    \ &&\n           internal::concepts::Multipliable<T>\nvector<T> min25_sieve(long\
    \ long n, vector<T> sumfp, F f) {\n  assert(sumfp.size() % 2 == 0);\n  long long\
    \ m = sumfp.size() / 2;\n  assert(m * m <= n && (m + 1) * (m + 1) > n);\n  vector<bool>\
    \ is_prime(m + 1);\n  for (int i = 2; i <= m; i++) {\n    is_prime[i] = 1;\n \
    \ }\n  for (int i = 2; i * i <= m; i++) {\n    if (!is_prime[i]) {\n      continue;\n\
    \    }\n    for (int j = i * i; j <= m; j += i) {\n      is_prime[j] = 0;\n  \
    \  }\n  }\n  vector<long long> div(m + 1);\n  vector<T> sumf = sumfp, _sumf =\
    \ sumf;\n  for (int i = 1; i <= m; i++) {\n    div[i] = n / i;\n  }\n  for (int\
    \ prime = m; prime >= 2; prime--) {\n    if (!is_prime[prime]) {\n      continue;\n\
    \    }\n    int pow = 1;\n    for (long long prime_pow = prime; div[prime] >=\
    \ prime_pow;\n         prime_pow *= prime, pow++) {\n      for (int i = 1; i <=\
    \ m; i++) {\n        long long divprime = div[i] / prime_pow;\n        if (divprime\
    \ < prime) {\n          break;\n        }\n        T div_val = i * prime_pow <=\
    \ m ? sumf[2 * m - i * prime_pow]\n                                       : sumf[divprime\
    \ - 1];\n        _sumf[2 * m - i] +=\n            f(prime, pow) * (div_val - sumfp[prime\
    \ - 1]) + f(prime, pow + 1);\n      }\n      for (int i = m; i >= 1; i--) {\n\
    \        long long divprime = i / prime_pow;\n        if (divprime < prime) {\n\
    \          break;\n        }\n        _sumf[i - 1] += f(prime, pow) * (sumf[divprime\
    \ - 1] - sumf[prime - 1]) +\n                        f(prime, pow + 1);\n    \
    \  }\n    }\n    long long prime_squared = (long long)prime * prime;\n    int\
    \ lim =\n        prime_squared <= m ? prime_squared - 1 : 2 * m - n / prime_squared;\n\
    \    for (int i = 2 * m - 1; i >= lim; i--) {\n      sumf[i] = _sumf[i];\n   \
    \ }\n  }\n  for (int i = 0; i < 2 * m; i++) {\n    sumf[i] += f(1, 0);\n  }\n\
    \  return sumf;\n}\n} // namespace maomao90\n#line 10 \"verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp\"\
    \nusing namespace maomao90;\nusing mint = static_modint<469762049>;\n\nint t;\n\
    long long n;\nint a, b;\n\nint main() {\n  cin >> t;\n  while (t--) {\n    cin\
    \ >> n >> a >> b;\n    long long m = 1;\n    while ((m + 1) * (m + 1) <= n) {\n\
    \      m++;\n    }\n    vector<mint> sumg_0(m * 2), sumg_1(m * 2);\n    for (int\
    \ i = 1; i <= m; i++) {\n      sumg_0[i - 1] = i - 1;\n      sumg_1[i - 1] = (long\
    \ long)i * (i + 1) / 2 - 1;\n      sumg_0[2 * m - i] = n / i - 1;\n      sumg_1[2\
    \ * m - i] = mint(n / i) * (n / i + 1) / 2 - 1;\n    }\n    vector<mint> sumgp_0\
    \ =\n        lucy_dp(n, sumg_0, [&](long long p) { return mint::raw(1); });\n\
    \    vector<mint> sumgp_1 =\n        lucy_dp(n, sumg_1, [&](long long p) { return\
    \ mint(p); });\n    vector<mint> sumfp(m * 2);\n    for (int i = 0; i < 2 * m;\
    \ i++) {\n      sumfp[i] = sumgp_0[i] * a + sumgp_1[i] * b;\n    }\n    auto f\
    \ = [&](long long p, int e) {\n      if (p == 1)\n        return mint(1);\n  \
    \    return mint(a) * e + mint(b) * p;\n    };\n    vector<mint> sumf = min25_sieve(n,\
    \ sumfp, f);\n    cout << sumf.back() << '\\n';\n  }\n  return 0;\n}\n"
  code: "#define PROBLEM                                                         \
    \       \\\n  \"https://judge.yosupo.jp/problem/sum_of_multiplicative_function_large\"\
    \n\n#include <iostream>\n#include <vector>\nusing namespace std;\n\n#include \"\
    library/math/modint.hpp\"\n#include \"library/math/multiplicative_function.hpp\"\
    \nusing namespace maomao90;\nusing mint = static_modint<469762049>;\n\nint t;\n\
    long long n;\nint a, b;\n\nint main() {\n  cin >> t;\n  while (t--) {\n    cin\
    \ >> n >> a >> b;\n    long long m = 1;\n    while ((m + 1) * (m + 1) <= n) {\n\
    \      m++;\n    }\n    vector<mint> sumg_0(m * 2), sumg_1(m * 2);\n    for (int\
    \ i = 1; i <= m; i++) {\n      sumg_0[i - 1] = i - 1;\n      sumg_1[i - 1] = (long\
    \ long)i * (i + 1) / 2 - 1;\n      sumg_0[2 * m - i] = n / i - 1;\n      sumg_1[2\
    \ * m - i] = mint(n / i) * (n / i + 1) / 2 - 1;\n    }\n    vector<mint> sumgp_0\
    \ =\n        lucy_dp(n, sumg_0, [&](long long p) { return mint::raw(1); });\n\
    \    vector<mint> sumgp_1 =\n        lucy_dp(n, sumg_1, [&](long long p) { return\
    \ mint(p); });\n    vector<mint> sumfp(m * 2);\n    for (int i = 0; i < 2 * m;\
    \ i++) {\n      sumfp[i] = sumgp_0[i] * a + sumgp_1[i] * b;\n    }\n    auto f\
    \ = [&](long long p, int e) {\n      if (p == 1)\n        return mint(1);\n  \
    \    return mint(a) * e + mint(b) * p;\n    };\n    vector<mint> sumf = min25_sieve(n,\
    \ sumfp, f);\n    cout << sumf.back() << '\\n';\n  }\n  return 0;\n}\n"
  dependsOn:
  - library/math/modint.hpp
  - library/internal/concepts.hpp
  - library/internal/type_traits.hpp
  - library/math/extended_gcd.hpp
  - library/math/primality_test.hpp
  - library/math/pow_mod.hpp
  - library/math/multiplicative_function.hpp
  isVerificationFile: true
  path: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
  requiredBy: []
  timestamp: '2024-11-18 20:19:33+08:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
layout: document
redirect_from:
- /verify/verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
- /verify/verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp.html
title: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
---
