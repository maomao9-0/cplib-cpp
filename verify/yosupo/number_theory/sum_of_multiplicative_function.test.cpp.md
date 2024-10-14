---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: library/internal/internal_concepts.hpp
    title: library/internal/internal_concepts.hpp
  - icon: ':heavy_check_mark:'
    path: library/math/extended_gcd.hpp
    title: library/math/extended_gcd.hpp
  - icon: ':heavy_check_mark:'
    path: library/math/modint.hpp
    title: library/math/modint.hpp
  - icon: ':heavy_check_mark:'
    path: library/math/multiplicative_function.hpp
    title: library/math/multiplicative_function.hpp
  - icon: ':heavy_check_mark:'
    path: library/math/poly.hpp
    title: library/math/poly.hpp
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
    PROBLEM: https://judge.yosupo.jp/problem/sum_of_multiplicative_function_large
    links:
    - https://judge.yosupo.jp/problem/sum_of_multiplicative_function_large
  bundledCode: "#line 1 \"verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp\"\
    \n#define PROBLEM \"https://judge.yosupo.jp/problem/sum_of_multiplicative_function_large\"\
    \n\n#include <iostream>\n#include <vector>\n#include <utility>\nusing namespace\
    \ std;\n\n#line 2 \"library/math/multiplicative_function.hpp\"\n\n#line 2 \"library/math/poly.hpp\"\
    \n\n#line 4 \"library/math/poly.hpp\"\n\nnamespace maomao90 {\n    template <typename\
    \ T>\n    struct Poly {\n        constexpr Poly(): _v(1, 0) {}\n        constexpr\
    \ Poly(int n): _v(n) {}\n        constexpr Poly(vector<T> v): _v(v) {}\n\n   \
    \     constexpr int degree() const {\n            return _v.size() - 1;\n    \
    \    }\n        constexpr T& operator[](int i) {\n            return _v[i];\n\
    \        }\n    private:\n        vector<T> _v;\n    };\n}\n#line 2 \"library/internal/internal_concepts.hpp\"\
    \n\n#include <concepts>\n\nnamespace internal::concepts {\n    template <typename\
    \ T>\n    concept Addable = requires (T a) {\n        a + a;\n        a += a;\n\
    \    };\n    template <typename T>\n    concept Subtractable = requires (T a)\
    \ {\n        a - a;\n        a -= a;\n    };\n    template <typename T>\n    concept\
    \ Multipliable = requires (T a) {\n        a * a;\n        a *= a;\n    };\n \
    \   template <typename T>\n    concept Dividable =  requires (T a) {\n       \
    \ a / a;\n        a /= a;\n    };\n}\n#line 5 \"library/math/multiplicative_function.hpp\"\
    \n\n#line 9 \"library/math/multiplicative_function.hpp\"\n#include <cassert>\n\
    \nnamespace maomao90 {\n    // sumg is a vector of size 2 * m where sumg[i] represents\
    \ prefix sum of\n    // g from 2 to i + 1 for i < m, otherwise, sum from 2 to\
    \ n / (2 * m - i)\n    // g is a function that returns the value of g(prime)\n\
    \    // returns prefix sum of g at prime indices with the same range as sumg\n\
    \    template <typename T, typename G> requires\n        requires(G g, long long\
    \ prime) { { g(prime) } -> same_as<T>; } &&\n        internal::concepts::Addable<T>\
    \ &&\n        internal::concepts::Subtractable<T> &&\n        internal::concepts::Multipliable<T>\n\
    \    vector<T> lucy_dp(long long n, vector<T> sumg, G g) {\n        assert(sumg.size()\
    \ % 2 == 0);\n        long long m = sumg.size() / 2;\n        assert(m * m <=\
    \ n && (m + 1) * (m + 1) > n);\n        vector<bool> is_prime(m + 1);\n      \
    \  for (int i = 2; i <= m; i++) {\n            is_prime[i] = 1;\n        }\n \
    \       for (int i = 2; i * i <= m; i++) {\n            if (!is_prime[i]) {\n\
    \                continue;\n            }\n            for (int j = i * i; j <=\
    \ m; j += i) {\n                is_prime[j] = 0;\n            }\n        }\n \
    \       vector<long long> div(m + 1);\n        vector<T> sumgp = sumg;\n     \
    \   for (int i = 1; i <= m; i++) {\n            div[i] = n / i;\n        }\n \
    \       for (int prime = 2; prime <= m; prime++) {\n            if (!is_prime[prime])\
    \ {\n                continue;\n            }\n            long long prime_squared\
    \ = (long long) prime * prime,\n                 iprime = prime;\n           \
    \ for (int i = 1; i <= m; i++, iprime += prime) {\n                if (div[i]\
    \ < prime_squared) {\n                    break;\n                }\n        \
    \        T div_val = iprime <= m ? sumgp[2 * m - iprime] : sumgp[div[i] / prime\
    \ - 1];\n                sumgp[2 * m - i] -= g(prime) * (div_val - sumgp[prime\
    \ - 2]);\n            }\n            for (int i = m; i >= prime_squared; i--)\
    \ {\n                sumgp[i - 1] -= g(prime) * (sumgp[i / prime - 1] - sumgp[prime\
    \ - 2]);\n            }\n        }\n        return sumgp;\n    }\n    template\
    \ <typename T, typename F> requires \n        requires(F f, long long prime, int\
    \ power) { { f(prime, power) } -> same_as<T>; } &&\n        internal::concepts::Addable<T>\
    \ &&\n        internal::concepts::Subtractable<T> &&\n        internal::concepts::Multipliable<T>\n\
    \    vector<T> min25_sieve(long long n, F f, vector<T> sumfp) {\n        assert(sumfp.size()\
    \ % 2 == 0);\n        long long m = sumfp.size() / 2;\n        assert(m * m <=\
    \ n && (m + 1) * (m + 1) > n);\n        vector<bool> is_prime(m + 1);\n      \
    \  for (int i = 2; i <= m; i++) {\n            is_prime[i] = 1;\n        }\n \
    \       for (int i = 2; i * i <= m; i++) {\n            if (!is_prime[i]) {\n\
    \                continue;\n            }\n            for (int j = i * i; j <=\
    \ m; j += i) {\n                is_prime[j] = 0;\n            }\n        }\n \
    \       vector<long long> div(m + 1);\n        vector<T> sumf = sumfp, _sumf =\
    \ sumf, nsumf = sumfp;\n        for (int i = 1; i <= m; i++) {\n            div[i]\
    \ = n / i;\n        }\n        for (int prime = m; prime >= 2; prime--) {\n  \
    \          if (!is_prime[prime]) {\n                continue;\n            }\n\
    \            int pow = 1;\n            for (long long prime_pow = prime; div[prime]\
    \ >= prime_pow; prime_pow *= prime, pow++) {\n                for (int i = 1;\
    \ i <= m; i++) {\n                    long long divprime = div[i] / prime_pow;\n\
    \                    if (divprime < prime) {\n                        break;\n\
    \                    }\n                    T div_val = i * prime_pow <= m ? sumf[2\
    \ * m - i * prime_pow] : sumf[divprime - 1];\n                    _sumf[2 * m\
    \ - i] += f(prime, pow) * (div_val - sumfp[prime - 1]) + f(prime, pow + 1);\n\
    \                }\n                for (int i = m; i >= 1; i--) {\n         \
    \           long long divprime = i / prime_pow;\n                    if (divprime\
    \ < prime) {\n                        break;\n                    }\n        \
    \            _sumf[i - 1] += f(prime, pow) * (sumf[divprime - 1] - sumf[prime\
    \ - 1]) + f(prime, pow + 1);\n                }\n            }\n            long\
    \ long prime_squared = (long long) prime * prime;\n            int lim = prime_squared\
    \ <= m ? prime_squared - 1 :\n                2 * m - n / prime_squared;\n   \
    \         for (int i = 2 * m - 1; i >= lim; i--) {\n                sumf[i] =\
    \ _sumf[i];\n            }\n        }\n        for (int i = 0; i < 2 * m; i++)\
    \ {\n            sumf[i]++;\n        }\n        return sumf;\n    }\n}\n#line\
    \ 2 \"library/math/modint.hpp\"\n\n#line 2 \"library/math/extended_gcd.hpp\"\n\
    \n#line 4 \"library/math/extended_gcd.hpp\"\n#include <type_traits>\n#line 6 \"\
    library/math/extended_gcd.hpp\"\n#include <limits>\n\nnamespace maomao90 {\n \
    \   template <signed_integral T>\n    constexpr T inv_gcd(T x, T mod) {\n    \
    \    using U = conditional_t<numeric_limits<T>::max() <= numeric_limits<int>::max(),\
    \ long long, __int128>;\n        U a = mod, b = x, va = 0, vb = 1;\n        while\
    \ (b) {\n            T k = a / b;\n            a -= k * b; va -= k * vb;\n   \
    \         swap(a, b);\n            swap(va, vb);\n        }\n        assert(a\
    \ == 1); // gcd should be equal to 1\n        return (va % mod + mod) % mod;\n\
    \    }\n}\n#line 2 \"library/math/primality_test.hpp\"\n\n#line 2 \"library/math/pow_mod.hpp\"\
    \n\n#line 6 \"library/math/pow_mod.hpp\"\n\nnamespace maomao90 {\n    template\
    \ <integral T>\n    constexpr T pow_mod(T b, T p, T mod) {\n        using U =\
    \ conditional_t<numeric_limits<T>::max() <= numeric_limits<unsigned int>::max(),\
    \ unsigned long long, unsigned __int128>;\n        T res = 1;\n        b %= mod;\n\
    \        if (b < 0) {\n            b += mod;\n        }\n        while (p) {\n\
    \            if (p & 1) {\n                res = (U) res * b % mod;\n        \
    \    }\n            b = (U) b * b % mod;\n            p >>= 1;\n        }\n  \
    \      return res;\n    }\n}\n#line 4 \"library/math/primality_test.hpp\"\n\n\
    #line 8 \"library/math/primality_test.hpp\"\n\nnamespace maomao90 {\n    template\
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
    \ 6 \"library/math/modint.hpp\"\n\n#line 12 \"library/math/modint.hpp\"\n\nnamespace\
    \ maomao90 {\n    template <typename T>\n    concept Modint = requires (T a, long\
    \ long p) {\n        { T::imod() } -> same_as<typename T::mod_type>;\n       \
    \ { T::umod() } -> same_as<typename T::umod_type>;\n        a++; a--; ++a; --a;\n\
    \        +a; -a;\n        a.pow(p); a.inv();\n    } &&\n        internal::concepts::Addable<T>\
    \ &&\n        internal::concepts::Subtractable<T> &&\n        internal::concepts::Multipliable<T>\
    \ &&\n        internal::concepts::Dividable<T> &&\n        equality_comparable<T>;\n\
    \n    template <signed_integral M = int, M mod = 998244353, enable_if_t<(mod >=\
    \ 1), int> = 0>\n    struct static_modint {\n    private:\n        using UM =\
    \ make_unsigned_t<M>;\n        using BM = conditional_t<numeric_limits<M>::max()\
    \ <= numeric_limits<int>::max(),\n              long long, __int128>;\n    public:\n\
    \        using mod_type = M;\n        using umod_type = UM;\n        static constexpr\
    \ M imod() {\n            return mod;\n        }\n        static constexpr UM\
    \ umod() {\n            return mod;\n        }\n\n        static constexpr static_modint\
    \ raw(M v) {\n            static_modint res;\n            res._v = v;\n      \
    \      return res;\n        }\n\n        constexpr static_modint(): _v(0) {}\n\
    \n        template <signed_integral T>\n        constexpr static_modint(T v) {\n\
    \            M x = v % imod();\n            if (x < 0) {\n                x +=\
    \ imod();\n            }\n            _v = x;\n        }\n\n        template <unsigned_integral\
    \ T>\n        constexpr static_modint(T v): _v(v % umod()) {}\n\n        constexpr\
    \ UM val() const {\n            return _v;\n        }\n\n        constexpr static_modint\
    \ operator+() const {\n            return *this;\n        }\n        constexpr\
    \ static_modint operator-() const {\n            return raw(_v == 0 ? 0 : imod()\
    \ - _v);\n        }\n\n        constexpr static_modint& operator++() {\n     \
    \       ++_v;\n            if (_v == umod()) {\n                _v = 0;\n    \
    \        }\n            return *this;\n        }\n        constexpr static_modint&\
    \ operator--() {\n            if (_v == 0) {\n                _v = umod();\n \
    \           }\n            --_v;\n            return *this;\n        }\n     \
    \   constexpr static_modint operator++(int) {\n            static_modint x = *this;\n\
    \            ++*this;\n            return x;\n        }\n        constexpr static_modint\
    \ operator--(int) {\n            static_modint x = *this;\n            --*this;\n\
    \            return x;\n        }\n\n        constexpr static_modint& operator+=(const\
    \ static_modint &o) {\n            _v += o._v;\n            if (_v >= umod())\
    \ {\n                _v -= umod();\n            }\n            return *this;\n\
    \        }\n        constexpr static_modint& operator-=(const static_modint &o)\
    \ {\n            if (_v < o._v) {\n                _v += umod();\n           \
    \ }\n            _v -= o._v;\n            return *this;\n        }\n        constexpr\
    \ static_modint& operator*=(const static_modint &o) {\n            _v = (BM) _v\
    \ * o._v % umod();\n            return *this;\n        }\n        constexpr static_modint&\
    \ operator/=(const static_modint &o) {\n            return *this *= o.inv();\n\
    \        }\n\n        constexpr static_modint pow(long long p) const {\n     \
    \       assert(p >= 0);\n            static_modint res = 1, b = *this;\n     \
    \       while (p) {\n                if (p & 1) {\n                    res *=\
    \ b;\n                }\n                b *= b;\n                p >>= 1;\n \
    \           }\n            return res;\n        }\n        constexpr static_modint\
    \ inv() const {\n            if constexpr (is_prime_mod) {\n                return\
    \ pow(imod() - 2);\n            } else {\n                return raw(inv_gcd((M)\
    \ _v, imod()));\n            }\n        }\n\n        friend constexpr static_modint\
    \ operator+(const static_modint &l, const static_modint &r) {\n            static_modint\
    \ res = l;\n            return res += r;\n        }\n        friend constexpr\
    \ static_modint operator-(const static_modint &l, const static_modint &r) {\n\
    \            static_modint res = l;\n            return res -= r;\n        }\n\
    \        friend constexpr static_modint operator*(const static_modint &l, const\
    \ static_modint &r) {\n            static_modint res = l;\n            return\
    \ res *= r;\n        }\n        friend constexpr static_modint operator/(const\
    \ static_modint &l, const static_modint &r) {\n            static_modint res =\
    \ l;\n            return res /= r;\n        }\n\n        constexpr bool operator==(const\
    \ static_modint &o) const {\n            return _v == o._v;\n        }\n     \
    \   constexpr bool operator!=(const static_modint &o) const {\n            return\
    \ !(*this == o);\n        }\n\n        friend constexpr istream& operator>>(istream\
    \ &is, static_modint &o) {\n            M v; is >> v;\n            o = static_modint(v);\n\
    \            return is;\n        }\n        friend constexpr ostream& operator<<(ostream\
    \ &os, const static_modint &o) {\n            return os << o._v;\n        }\n\
    \    private:\n        UM _v;\n        static constexpr bool is_prime_mod = is_prime(umod());\n\
    \    };\n}\n#line 11 \"verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp\"\
    \nusing namespace maomao90;\nusing mint = static_modint<int, 469762049>;\n\nint\
    \ t;\nlong long n;\nint a, b;\n\nint main() {\n    cin >> t;\n    while (t--)\
    \ {\n        cin >> n >> a >> b;\n        long long m = 1;\n        while ((m\
    \ + 1) * (m + 1) <= n) {\n            m++;\n        }\n        vector<mint> sumg_0(m\
    \ * 2), sumg_1(m * 2);\n        for (int i = 1; i <= m; i++) {\n            sumg_0[i\
    \ - 1] = i - 1;\n            sumg_1[i - 1] = (long long) i * (i + 1) / 2 - 1;\n\
    \            sumg_0[2 * m - i] = n / i - 1;\n            sumg_1[2 * m - i] = mint(n\
    \ / i) * (n / i + 1) / 2 - 1;\n        }\n        vector<mint> sumgp_0 = lucy_dp(n,\
    \ sumg_0, [&] (long long p) {\n                return mint::raw(1);\n        \
    \        });\n        vector<mint> sumgp_1 = lucy_dp(n, sumg_1, [&] (long long\
    \ p) {\n                return mint(p);\n                });\n        vector<mint>\
    \ sumfp(m * 2);\n        for (int i = 0; i < 2 * m; i++) {\n            sumfp[i]\
    \ = sumgp_0[i] * a + sumgp_1[i] * b;\n        }\n        auto f = [&] (long long\
    \ p, int e) {\n            return mint(a) * e + mint(b) * p;\n        };\n   \
    \     vector<mint> sumf = min25_sieve(n, f, sumfp);\n        cout << sumf.back()\
    \ << '\\n';\n    }\n    return 0;\n}\n"
  code: "#define PROBLEM \"https://judge.yosupo.jp/problem/sum_of_multiplicative_function_large\"\
    \n\n#include <iostream>\n#include <vector>\n#include <utility>\nusing namespace\
    \ std;\n\n#include \"library/math/multiplicative_function.hpp\"\n#include \"library/math/poly.hpp\"\
    \n#include \"library/math/modint.hpp\"\nusing namespace maomao90;\nusing mint\
    \ = static_modint<int, 469762049>;\n\nint t;\nlong long n;\nint a, b;\n\nint main()\
    \ {\n    cin >> t;\n    while (t--) {\n        cin >> n >> a >> b;\n        long\
    \ long m = 1;\n        while ((m + 1) * (m + 1) <= n) {\n            m++;\n  \
    \      }\n        vector<mint> sumg_0(m * 2), sumg_1(m * 2);\n        for (int\
    \ i = 1; i <= m; i++) {\n            sumg_0[i - 1] = i - 1;\n            sumg_1[i\
    \ - 1] = (long long) i * (i + 1) / 2 - 1;\n            sumg_0[2 * m - i] = n /\
    \ i - 1;\n            sumg_1[2 * m - i] = mint(n / i) * (n / i + 1) / 2 - 1;\n\
    \        }\n        vector<mint> sumgp_0 = lucy_dp(n, sumg_0, [&] (long long p)\
    \ {\n                return mint::raw(1);\n                });\n        vector<mint>\
    \ sumgp_1 = lucy_dp(n, sumg_1, [&] (long long p) {\n                return mint(p);\n\
    \                });\n        vector<mint> sumfp(m * 2);\n        for (int i =\
    \ 0; i < 2 * m; i++) {\n            sumfp[i] = sumgp_0[i] * a + sumgp_1[i] * b;\n\
    \        }\n        auto f = [&] (long long p, int e) {\n            return mint(a)\
    \ * e + mint(b) * p;\n        };\n        vector<mint> sumf = min25_sieve(n, f,\
    \ sumfp);\n        cout << sumf.back() << '\\n';\n    }\n    return 0;\n}\n"
  dependsOn:
  - library/math/multiplicative_function.hpp
  - library/math/poly.hpp
  - library/internal/internal_concepts.hpp
  - library/math/modint.hpp
  - library/math/extended_gcd.hpp
  - library/math/primality_test.hpp
  - library/math/pow_mod.hpp
  isVerificationFile: true
  path: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
  requiredBy: []
  timestamp: '2024-10-14 14:00:38+08:00'
  verificationStatus: TEST_ACCEPTED
  verifiedWith: []
documentation_of: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
layout: document
redirect_from:
- /verify/verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
- /verify/verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp.html
title: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
---
