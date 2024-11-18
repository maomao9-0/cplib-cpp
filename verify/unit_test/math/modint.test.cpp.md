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
    PROBLEM: https://judge.yosupo.jp/problem/aplusb
    links:
    - https://judge.yosupo.jp/problem/aplusb
  bundledCode: "#line 1 \"verify/unit_test/math/modint.test.cpp\"\n#define PROBLEM\
    \ \"https://judge.yosupo.jp/problem/aplusb\"\n\n#include <cassert>\n#include <chrono>\n\
    #include <climits>\n#include <iostream>\n#include <random>\nusing namespace std;\n\
    \n#line 2 \"library/math/modint.hpp\"\n\n#line 4 \"library/math/modint.hpp\"\n\
    #include <concepts>\n#line 6 \"library/math/modint.hpp\"\n#include <type_traits>\n\
    \n#line 2 \"library/internal/concepts.hpp\"\n\n#line 2 \"library/internal/type_traits.hpp\"\
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
    \ = is_prime(umod());\n};\n} // namespace maomao90\n#line 11 \"verify/unit_test/math/modint.test.cpp\"\
    \nusing namespace maomao90;\n\nmt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());\n\
    \ntemplate <class T> inline T gen_random_value(const T mod) {\n  T res = rng();\n\
    \  int r = rng() % 10;\n  if (r == 0) {\n    res = mod - 1;\n  } else if (r ==\
    \ 1) {\n    if (rng() % 2 == 0) {\n      res = numeric_limits<T>::max();\n   \
    \ } else {\n      res = numeric_limits<T>::min();\n    }\n  } else if (r == 2)\
    \ {\n    res = 0;\n  }\n  if (rng() % 10 != 0)\n    res %= mod;\n  return res;\n\
    }\n\ntemplate <typename mint> void test(int testcases) {\n  const auto mod = mint::imod();\n\
    \  static_assert(mod == mint::imod());\n  while (testcases--) {\n    long long\
    \ ta = gen_random_value(mod);\n    mint A = ta;\n    __int128 a = ta % mod;\n\
    \    if (a < 0) {\n      a += mod;\n    }\n    assert(A.val() == a);\n    assert(A\
    \ == mint((long long)a));\n\n    unsigned long long tb = gen_random_value(mod);\n\
    \    mint B = tb;\n    __int128 b = tb % mod;\n    assert(B.val() == b);\n   \
    \ assert(B == mint((long long)b));\n\n    assert((A == B) == (a == b));\n    assert((A\
    \ != B) == (a != b));\n\n    __int128 c = (a + b) % mod;\n    mint C = A + B;\n\
    \    assert(C.val() == c);\n\n    __int128 d = (a + mod - b) % mod;\n    mint\
    \ D = A - B;\n    assert(D.val() == d);\n\n    __int128 e = a * b % mod;\n   \
    \ mint E = A * B;\n    assert(E.val() == e);\n\n    long long f;\n    do {\n \
    \     f = rng() % mod;\n    } while (__gcd(f, (long long)mod) != 1);\n    assert(f\
    \ < mod);\n    mint F = f;\n    mint G = F.inv();\n    assert(F * G == 1);\n \
    \   assert((__int128)F.val() * G.val() % mod == 1 % mod);\n    long long g = G.val();\n\
    \n    __int128 h = e * g % mod;\n    mint H = E / F;\n    assert(H.val() == h);\n\
    \n    long long ti = gen_random_value(mod);\n    long long ex = rng() % LLONG_MAX;\n\
    \    if (rng() % 10 == 0)\n      ex = rng() % 3;\n    __int128 i = ti % mod, j\
    \ = 1 % mod;\n    if (i < 0) {\n      i += mod;\n    }\n    {\n      __int128\
    \ i2 = i;\n      long long e2 = ex;\n      while (e2) {\n        if (e2 & 1) {\n\
    \          j = j * i2 % mod;\n        }\n        i2 = i2 * i2 % mod;\n       \
    \ e2 >>= 1;\n      }\n    }\n    mint I = ti;\n    mint J = I.pow(ex);\n    assert(I.val()\
    \ == i);\n    assert(J.val() == j);\n\n    __int128 k = a == 0 ? 0 : mod - a;\n\
    \    mint K = -A;\n    assert(K.val() == k);\n    assert(A == +A);\n\n    __int128\
    \ l = a++;\n    if (a >= mod) {\n      a -= mod;\n    }\n    mint L = A++;\n \
    \   assert(A.val() == a);\n    assert(L.val() == l);\n\n    __int128 m = gen_random_value(mod)\
    \ % mod;\n    if (m < 0) {\n      m += mod;\n    }\n    mint M = mint::raw(m);\n\
    \    assert(M.val() == m);\n\n    __int128 n = ++m;\n    if (m >= mod) {\n   \
    \   m -= mod;\n    }\n    if (n >= mod) {\n      n -= mod;\n    }\n    mint N\
    \ = ++M;\n    assert(M.val() == m);\n    assert(N.val() == n);\n\n    __int128\
    \ o = b--;\n    if (b < 0) {\n      b += mod;\n    }\n    mint O = B--;\n    assert(B.val()\
    \ == b);\n    assert(O.val() == o);\n\n    __int128 p = gen_random_value(mod)\
    \ % mod;\n    if (p < 0) {\n      p += mod;\n    }\n    mint P = mint::raw(p);\n\
    \    assert(P.val() == p);\n\n    __int128 q = --p;\n    if (p < 0) {\n      p\
    \ += mod;\n    }\n    if (q < 0) {\n      q += mod;\n    }\n    mint Q = --P;\n\
    \    assert(P.val() == p);\n    assert(Q.val() == q);\n  }\n}\n\ntemplate <typename\
    \ M, M mod> void test_wrapper(int testcases = 10000) {\n  test<static_modint<mod>>(testcases);\n\
    }\n\nint main() {\n  test_wrapper<int, 998244353>(1'000'000);\n  test_wrapper<int,\
    \ 1'000'000'007>(1'000'000);\n  test_wrapper<long long, 998244353>();\n  test_wrapper<long\
    \ long, 1'000'000'007>();\n\n  test_wrapper<int, INT_MAX>();\n  test_wrapper<int,\
    \ INT_MAX - 1>();\n  test_wrapper<int, INT_MAX - 2>(1);\n  test_wrapper<long long,\
    \ LLONG_MAX>();\n  test_wrapper<long long, LLONG_MAX - 1>();\n  test_wrapper<long\
    \ long, LLONG_MAX - 2>();\n\n  test_wrapper<int, 1>();\n  test_wrapper<int, 2>();\n\
    \  test_wrapper<int, 3>();\n  test_wrapper<int, 4>();\n  test_wrapper<int, 5>();\n\
    \  test_wrapper<int, 6>();\n  test_wrapper<int, 7>();\n  test_wrapper<int, 8>();\n\
    \  test_wrapper<int, 9>();\n  test_wrapper<int, 10>();\n  test_wrapper<int, 100>();\n\
    \  test_wrapper<int, 1000>();\n  test_wrapper<int, 10000>();\n  test_wrapper<int,\
    \ 100000>();\n  test_wrapper<int, 1000000>();\n  test_wrapper<int, 10000000>();\n\
    \  test_wrapper<int, 100000000>();\n  test_wrapper<int, 1000000000>();\n  test_wrapper<long\
    \ long, 10000000000>();\n  test_wrapper<long long, 100000000000>();\n  test_wrapper<long\
    \ long, 1000000000000>();\n  test_wrapper<long long, 10000000000000>();\n  test_wrapper<long\
    \ long, 100000000000000>();\n  test_wrapper<long long, 1000000000000000>();\n\
    \  test_wrapper<long long, 10000000000000000>();\n  test_wrapper<long long, 100000000000000000>();\n\
    \  test_wrapper<long long, 1000000000000000000>();\n  test_wrapper<int, 11>();\n\
    \  test_wrapper<int, 101>();\n  test_wrapper<int, 1001>();\n  test_wrapper<int,\
    \ 10001>();\n  test_wrapper<int, 100001>();\n  test_wrapper<int, 1000001>();\n\
    \  test_wrapper<int, 10000001>();\n  test_wrapper<int, 100000001>();\n  test_wrapper<int,\
    \ 1000000001>();\n  test_wrapper<long long, 10000000001>();\n  test_wrapper<long\
    \ long, 100000000001>();\n  test_wrapper<long long, 1000000000001>();\n  test_wrapper<long\
    \ long, 10000000000001>();\n  test_wrapper<long long, 100000000000001>();\n  test_wrapper<long\
    \ long, 1000000000000001>();\n  test_wrapper<long long, 10000000000000001>();\n\
    \  test_wrapper<long long, 100000000000000001>();\n  test_wrapper<long long, 1000000000000000001>();\n\
    \n  test_wrapper<int, 180227696>();\n  test_wrapper<int, 302343120>();\n  test_wrapper<int,\
    \ 447813621>();\n  test_wrapper<int, 454763204>();\n  test_wrapper<int, 557418622>();\n\
    \  test_wrapper<int, 666012019>();\n  test_wrapper<int, 666121844>();\n  test_wrapper<int,\
    \ 726530008>();\n  test_wrapper<int, 728776078>();\n  test_wrapper<int, 750511963>();\n\
    \n  test_wrapper<long long, 180227696>();\n  test_wrapper<long long, 302343120>();\n\
    \  test_wrapper<long long, 447813621>();\n  test_wrapper<long long, 454763204>();\n\
    \  test_wrapper<long long, 557418622>();\n  test_wrapper<long long, 666012019>();\n\
    \  test_wrapper<long long, 666121844>();\n  test_wrapper<long long, 726530008>();\n\
    \  test_wrapper<long long, 728776078>();\n  test_wrapper<long long, 750511963>();\n\
    \n  test_wrapper<long long, 4759123141>();\n  test_wrapper<long long, 1012713396046690528>();\n\
    \  test_wrapper<long long, 1635737873707882727>();\n  test_wrapper<long long,\
    \ 2289963980915342821>();\n  test_wrapper<long long, 7323047834428073605>();\n\
    \  test_wrapper<long long, 7685722831103890333>();\n  test_wrapper<long long,\
    \ 8188732928308455248>();\n  test_wrapper<long long, 8350304091958478101>();\n\
    \  test_wrapper<long long, 8368310815106939055>();\n  test_wrapper<long long,\
    \ 8683117913169507171>();\n\n  int a, b;\n  cin >> a >> b;\n  cout << a + b <<\
    \ '\\n';\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/aplusb\"\n\n#include <cassert>\n\
    #include <chrono>\n#include <climits>\n#include <iostream>\n#include <random>\n\
    using namespace std;\n\n#include \"library/math/modint.hpp\"\nusing namespace\
    \ maomao90;\n\nmt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());\n\
    \ntemplate <class T> inline T gen_random_value(const T mod) {\n  T res = rng();\n\
    \  int r = rng() % 10;\n  if (r == 0) {\n    res = mod - 1;\n  } else if (r ==\
    \ 1) {\n    if (rng() % 2 == 0) {\n      res = numeric_limits<T>::max();\n   \
    \ } else {\n      res = numeric_limits<T>::min();\n    }\n  } else if (r == 2)\
    \ {\n    res = 0;\n  }\n  if (rng() % 10 != 0)\n    res %= mod;\n  return res;\n\
    }\n\ntemplate <typename mint> void test(int testcases) {\n  const auto mod = mint::imod();\n\
    \  static_assert(mod == mint::imod());\n  while (testcases--) {\n    long long\
    \ ta = gen_random_value(mod);\n    mint A = ta;\n    __int128 a = ta % mod;\n\
    \    if (a < 0) {\n      a += mod;\n    }\n    assert(A.val() == a);\n    assert(A\
    \ == mint((long long)a));\n\n    unsigned long long tb = gen_random_value(mod);\n\
    \    mint B = tb;\n    __int128 b = tb % mod;\n    assert(B.val() == b);\n   \
    \ assert(B == mint((long long)b));\n\n    assert((A == B) == (a == b));\n    assert((A\
    \ != B) == (a != b));\n\n    __int128 c = (a + b) % mod;\n    mint C = A + B;\n\
    \    assert(C.val() == c);\n\n    __int128 d = (a + mod - b) % mod;\n    mint\
    \ D = A - B;\n    assert(D.val() == d);\n\n    __int128 e = a * b % mod;\n   \
    \ mint E = A * B;\n    assert(E.val() == e);\n\n    long long f;\n    do {\n \
    \     f = rng() % mod;\n    } while (__gcd(f, (long long)mod) != 1);\n    assert(f\
    \ < mod);\n    mint F = f;\n    mint G = F.inv();\n    assert(F * G == 1);\n \
    \   assert((__int128)F.val() * G.val() % mod == 1 % mod);\n    long long g = G.val();\n\
    \n    __int128 h = e * g % mod;\n    mint H = E / F;\n    assert(H.val() == h);\n\
    \n    long long ti = gen_random_value(mod);\n    long long ex = rng() % LLONG_MAX;\n\
    \    if (rng() % 10 == 0)\n      ex = rng() % 3;\n    __int128 i = ti % mod, j\
    \ = 1 % mod;\n    if (i < 0) {\n      i += mod;\n    }\n    {\n      __int128\
    \ i2 = i;\n      long long e2 = ex;\n      while (e2) {\n        if (e2 & 1) {\n\
    \          j = j * i2 % mod;\n        }\n        i2 = i2 * i2 % mod;\n       \
    \ e2 >>= 1;\n      }\n    }\n    mint I = ti;\n    mint J = I.pow(ex);\n    assert(I.val()\
    \ == i);\n    assert(J.val() == j);\n\n    __int128 k = a == 0 ? 0 : mod - a;\n\
    \    mint K = -A;\n    assert(K.val() == k);\n    assert(A == +A);\n\n    __int128\
    \ l = a++;\n    if (a >= mod) {\n      a -= mod;\n    }\n    mint L = A++;\n \
    \   assert(A.val() == a);\n    assert(L.val() == l);\n\n    __int128 m = gen_random_value(mod)\
    \ % mod;\n    if (m < 0) {\n      m += mod;\n    }\n    mint M = mint::raw(m);\n\
    \    assert(M.val() == m);\n\n    __int128 n = ++m;\n    if (m >= mod) {\n   \
    \   m -= mod;\n    }\n    if (n >= mod) {\n      n -= mod;\n    }\n    mint N\
    \ = ++M;\n    assert(M.val() == m);\n    assert(N.val() == n);\n\n    __int128\
    \ o = b--;\n    if (b < 0) {\n      b += mod;\n    }\n    mint O = B--;\n    assert(B.val()\
    \ == b);\n    assert(O.val() == o);\n\n    __int128 p = gen_random_value(mod)\
    \ % mod;\n    if (p < 0) {\n      p += mod;\n    }\n    mint P = mint::raw(p);\n\
    \    assert(P.val() == p);\n\n    __int128 q = --p;\n    if (p < 0) {\n      p\
    \ += mod;\n    }\n    if (q < 0) {\n      q += mod;\n    }\n    mint Q = --P;\n\
    \    assert(P.val() == p);\n    assert(Q.val() == q);\n  }\n}\n\ntemplate <typename\
    \ M, M mod> void test_wrapper(int testcases = 10000) {\n  test<static_modint<mod>>(testcases);\n\
    }\n\nint main() {\n  test_wrapper<int, 998244353>(1'000'000);\n  test_wrapper<int,\
    \ 1'000'000'007>(1'000'000);\n  test_wrapper<long long, 998244353>();\n  test_wrapper<long\
    \ long, 1'000'000'007>();\n\n  test_wrapper<int, INT_MAX>();\n  test_wrapper<int,\
    \ INT_MAX - 1>();\n  test_wrapper<int, INT_MAX - 2>(1);\n  test_wrapper<long long,\
    \ LLONG_MAX>();\n  test_wrapper<long long, LLONG_MAX - 1>();\n  test_wrapper<long\
    \ long, LLONG_MAX - 2>();\n\n  test_wrapper<int, 1>();\n  test_wrapper<int, 2>();\n\
    \  test_wrapper<int, 3>();\n  test_wrapper<int, 4>();\n  test_wrapper<int, 5>();\n\
    \  test_wrapper<int, 6>();\n  test_wrapper<int, 7>();\n  test_wrapper<int, 8>();\n\
    \  test_wrapper<int, 9>();\n  test_wrapper<int, 10>();\n  test_wrapper<int, 100>();\n\
    \  test_wrapper<int, 1000>();\n  test_wrapper<int, 10000>();\n  test_wrapper<int,\
    \ 100000>();\n  test_wrapper<int, 1000000>();\n  test_wrapper<int, 10000000>();\n\
    \  test_wrapper<int, 100000000>();\n  test_wrapper<int, 1000000000>();\n  test_wrapper<long\
    \ long, 10000000000>();\n  test_wrapper<long long, 100000000000>();\n  test_wrapper<long\
    \ long, 1000000000000>();\n  test_wrapper<long long, 10000000000000>();\n  test_wrapper<long\
    \ long, 100000000000000>();\n  test_wrapper<long long, 1000000000000000>();\n\
    \  test_wrapper<long long, 10000000000000000>();\n  test_wrapper<long long, 100000000000000000>();\n\
    \  test_wrapper<long long, 1000000000000000000>();\n  test_wrapper<int, 11>();\n\
    \  test_wrapper<int, 101>();\n  test_wrapper<int, 1001>();\n  test_wrapper<int,\
    \ 10001>();\n  test_wrapper<int, 100001>();\n  test_wrapper<int, 1000001>();\n\
    \  test_wrapper<int, 10000001>();\n  test_wrapper<int, 100000001>();\n  test_wrapper<int,\
    \ 1000000001>();\n  test_wrapper<long long, 10000000001>();\n  test_wrapper<long\
    \ long, 100000000001>();\n  test_wrapper<long long, 1000000000001>();\n  test_wrapper<long\
    \ long, 10000000000001>();\n  test_wrapper<long long, 100000000000001>();\n  test_wrapper<long\
    \ long, 1000000000000001>();\n  test_wrapper<long long, 10000000000000001>();\n\
    \  test_wrapper<long long, 100000000000000001>();\n  test_wrapper<long long, 1000000000000000001>();\n\
    \n  test_wrapper<int, 180227696>();\n  test_wrapper<int, 302343120>();\n  test_wrapper<int,\
    \ 447813621>();\n  test_wrapper<int, 454763204>();\n  test_wrapper<int, 557418622>();\n\
    \  test_wrapper<int, 666012019>();\n  test_wrapper<int, 666121844>();\n  test_wrapper<int,\
    \ 726530008>();\n  test_wrapper<int, 728776078>();\n  test_wrapper<int, 750511963>();\n\
    \n  test_wrapper<long long, 180227696>();\n  test_wrapper<long long, 302343120>();\n\
    \  test_wrapper<long long, 447813621>();\n  test_wrapper<long long, 454763204>();\n\
    \  test_wrapper<long long, 557418622>();\n  test_wrapper<long long, 666012019>();\n\
    \  test_wrapper<long long, 666121844>();\n  test_wrapper<long long, 726530008>();\n\
    \  test_wrapper<long long, 728776078>();\n  test_wrapper<long long, 750511963>();\n\
    \n  test_wrapper<long long, 4759123141>();\n  test_wrapper<long long, 1012713396046690528>();\n\
    \  test_wrapper<long long, 1635737873707882727>();\n  test_wrapper<long long,\
    \ 2289963980915342821>();\n  test_wrapper<long long, 7323047834428073605>();\n\
    \  test_wrapper<long long, 7685722831103890333>();\n  test_wrapper<long long,\
    \ 8188732928308455248>();\n  test_wrapper<long long, 8350304091958478101>();\n\
    \  test_wrapper<long long, 8368310815106939055>();\n  test_wrapper<long long,\
    \ 8683117913169507171>();\n\n  int a, b;\n  cin >> a >> b;\n  cout << a + b <<\
    \ '\\n';\n}\n"
  dependsOn:
  - library/math/modint.hpp
  - library/internal/concepts.hpp
  - library/internal/type_traits.hpp
  - library/math/extended_gcd.hpp
  - library/math/primality_test.hpp
  - library/math/pow_mod.hpp
  isVerificationFile: true
  path: verify/unit_test/math/modint.test.cpp
  requiredBy: []
  timestamp: '2024-11-18 20:19:33+08:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/unit_test/math/modint.test.cpp
layout: document
redirect_from:
- /verify/verify/unit_test/math/modint.test.cpp
- /verify/verify/unit_test/math/modint.test.cpp.html
title: verify/unit_test/math/modint.test.cpp
---
