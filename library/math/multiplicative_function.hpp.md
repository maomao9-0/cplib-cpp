---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: library/internal/concepts.hpp
    title: library/internal/concepts.hpp
  - icon: ':heavy_check_mark:'
    path: library/math/poly.hpp
    title: Poly
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
    title: verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"library/math/multiplicative_function.hpp\"\n\n#line 2 \"\
    library/math/poly.hpp\"\n\n#include <vector>\n\nnamespace maomao90 {\n    template\
    \ <typename T>\n    struct Poly {\n        constexpr Poly(): _v(1, 0) {}\n   \
    \     constexpr Poly(int n): _v(n) {}\n        constexpr Poly(vector<T> v): _v(v)\
    \ {}\n\n        constexpr int degree() const {\n            return _v.size() -\
    \ 1;\n        }\n        constexpr T& operator[](int i) {\n            return\
    \ _v[i];\n        }\n    private:\n        vector<T> _v;\n    };\n}\n#line 2 \"\
    library/internal/concepts.hpp\"\n\n#include <concepts>\n\nnamespace maomao90::internal::concepts\
    \ {\n    template <typename T>\n    concept Addable = requires (T a) {\n     \
    \   a + a;\n        a += a;\n    };\n    template <typename T>\n    concept Subtractable\
    \ = requires (T a) {\n        a - a;\n        a -= a;\n    };\n    template <typename\
    \ T>\n    concept Multipliable = requires (T a) {\n        a * a;\n        a *=\
    \ a;\n    };\n    template <typename T>\n    concept Dividable = requires (T a)\
    \ {\n        a / a;\n        a /= a;\n    };\n}\n#line 5 \"library/math/multiplicative_function.hpp\"\
    \n\n#line 7 \"library/math/multiplicative_function.hpp\"\n#include <utility>\n\
    #line 9 \"library/math/multiplicative_function.hpp\"\n#include <cassert>\n\nnamespace\
    \ maomao90 {\n    // sumg is a vector of size 2 * m where sumg[i] represents prefix\
    \ sum of\n    // g from 2 to i + 1 for i < m, otherwise, sum from 2 to n / (2\
    \ * m - i)\n    // g is a function that returns the value of g(prime)\n    //\
    \ returns prefix sum of g at prime indices with the same range as sumg\n    template\
    \ <typename T, typename G> requires\n        requires(G g, long long prime) {\
    \ { g(prime) } -> same_as<T>; } &&\n        internal::concepts::Addable<T> &&\n\
    \        internal::concepts::Subtractable<T> &&\n        internal::concepts::Multipliable<T>\n\
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
    \    vector<T> min25_sieve(long long n, vector<T> sumfp, F f) {\n        assert(sumfp.size()\
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
    \ {\n            sumf[i]++;\n        }\n        return sumf;\n    }\n}\n"
  code: "#pragma once\n\n#include \"library/math/poly.hpp\"\n#include \"library/internal/concepts.hpp\"\
    \n\n#include <concepts>\n#include <utility>\n#include <vector>\n#include <cassert>\n\
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
    \    vector<T> min25_sieve(long long n, vector<T> sumfp, F f) {\n        assert(sumfp.size()\
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
    \ {\n            sumf[i]++;\n        }\n        return sumf;\n    }\n}\n"
  dependsOn:
  - library/math/poly.hpp
  - library/internal/concepts.hpp
  isVerificationFile: false
  path: library/math/multiplicative_function.hpp
  requiredBy: []
  timestamp: '2024-10-22 15:04:08+08:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
documentation_of: library/math/multiplicative_function.hpp
layout: document
title: Multiplicative Function
---

# Prefix Sum of Multiplicative Function

Given a multiplicative function $f$, we will use two functions, `lucy_dp` and `min25_sieve`, to calculate $\sum_{i=1}^N f(i)$ in $O\left(\frac{N^{3/4}}{\log N}\right)$ time.

By the nature of the algorithm, we can also obtain the prefix sum of the multiplicative function at the distinct positive integer values of $\left\lfloor\frac{N}{k}\right\rfloor$ as a side-product.

## Lucy DP

```cpp
vector<T> lucy_dp(long long n, vector<T> sumg, G g)
```

**Parameters**

- `T` is the data type that is returned by the function $g$.
- `n` is the upper limit of the prefix sum.
- `sumg` is a vertor of size $2\lfloor\sqrt n\rfloor$.
  
  $\text{sumg}\_k = \begin{cases}\sum_{i=2}^{k+1} g(i) & \text{ for } 0\le k< \lfloor\sqrt n\rfloor\\\\ \sum_{i=2}^{\left\lfloor \frac{N}{2\lfloor\sqrt N\rfloor - k}\right\rfloor} g(i) & \text{ for } \lfloor\sqrt n\rfloor\le k< 2\lfloor\sqrt n\rfloor\end{cases}$

  In simpler terms, `sumg` contains the prefix sum (excluding $g(1)$) of the function $g$ at the positive integer values of $\left\lfloor\frac{N}{k}\right\rfloor$.
- `g` is the multiplicative function $g$. `g(p)` should return the value of $g(p)$ when evaluated at a prime integer $p$.

`lucy_dp` returns a vector of size $2\lfloor\sqrt n\rfloor$ which is a transformation of `sumg` such that $g(p)$ is included in the prefix sums only if $p$ is prime. In other words, the $k$-th value in the vector returned by `lucy_dp` is equals to

\begin{cases}\sum_{i=2}^{k+1} g(i)[\texttt{is\_prime}(i)] & \text{ for } 0\le k< \lfloor\sqrt n\rfloor\\\\ \sum_{i=2}^{\left\lfloor \frac{N}{2\lfloor\sqrt N\rfloor - k}\right\rfloor} g(i)[\texttt{is\_prime}(i)] & \text{ for } \lfloor\sqrt n\rfloor\le k< 2\lfloor\sqrt n\rfloor\end{cases}


The time complexity of `lucy_dp` is $O\left(\frac{N^{3/4}}{\log N}\right)$.

## Min25 Sieve

```cpp
vector<T> min25_sieve(long long n, vector<T> sumfp, F f)
```

**Parameters**

- `T` is the data type that is returned by the function $f$.
- `n` is the upper limit of the prefix sum.
- `sumfp` is a vertor of size $2\lfloor\sqrt n\rfloor$. 

  $\text{sumfp}\_k = \begin{cases}\sum_{i=2}^{k+1} f(i)[\texttt{is\_prime}(i)] & \text{ for } 0\le k< \lfloor\sqrt n\rfloor\\\\ \sum_{i=2}^{\left\lfloor \frac{N}{2\lfloor\sqrt N\rfloor - k}\right\rfloor} f(i)[\texttt{is\_prime}(i)] & \text{ for } \lfloor\sqrt n\rfloor\le k< 2\lfloor\sqrt n\rfloor\end{cases}$

  In simpler terms, `sumfp` contains the prefix sum of the function $f$ at the positive integer values of $\left\lfloor\frac{N}{k}\right\rfloor$.
- `f` is the multiplicative function $f$. `f(p, k)` should return the value of $f(p^k)$ where $p$ is a prime. Note that this is sufficient to evaluate $f$ at any positive integer since $f$ is multiplicative.

`min25_sieve` returns a vector of size $2\sqrt n$ which is a transformation of `sumfp` to add back the contribution of $f(x)$ into the prefix sums at non-prime values of $x$ (including $f(1)$). In other words, the $k$-th value in the vector returned by `min25_sieve` is equals to

\begin{cases}\sum_{i=1}^{k+1} f(i) & \text{ for } 0\le k< \lfloor\sqrt n\rfloor\\\\ \sum_{i=1}^{\left\lfloor \frac{N}{2\lfloor\sqrt N\rfloor - k}\right\rfloor} f(i) & \text{ for } \lfloor\sqrt n\rfloor\le k< 2\lfloor\sqrt n\rfloor\end{cases}


The time complexity of `min25_sieve` is $O\left(\frac{N^{3/4}}{\log N}\right)$.

## Example

Let us try to find the prefix sum of the multiplicative function $f(p^k) = p^2+kp$.

When evaluated at prime values $p$, $f(p) = p^2 + p$. Using $g(p) = p^2 + p$ as the input for `lucy_dp` does not work as $g$ is not multiplicative ($g(2)g(3) = (2^2 + 2)(3^2 + 3) = 72 \neq g(6) = 42$.

Instead, what we can do is to use two different functions $g_1(p) = p$ and $g_2(p) = p^2$. Both $g_1$ and $g_2$ are multiplicative functions, and we can easily get their prefix sums in $O(1)$ time, so they are good candidates to use in `lucy_dp`. We can sum up their results to get the prefix sum of $(p^2 + p)[\texttt{is\_prime}(p)]$, which we can use as `sumfp` for the input of `min25_sieve`.

```c++
int s = floor(sqrt(n));
auto g1 = [&] (long long p) { return p; };
auto g2 = [&] (long long p) { return p * p; };
vector<long long> sumg1(2 * s), sumg2(2 * s);
for (int i = 1; i <= s; i++) {
	auto prefix_sum_g1 = [&] (long long x) { return x * (x + 1) / 2 - 1; };
	sumg1[i - 1] = prefix_sum_g1(i);
	sumg1[2 * s - i] = prefix_sum_g1(n / i);
	auto prefix_sum_g2 = [&] (long long x) { return x * (x + 1) * (2 * x + 1) / 6 - 1; }
	sumg2[i - 1] = prefix_sum_g2(i);
	sumg2[2 * s - i] = prefix_sum_g2(n / i);
}
vector<long long> sumgp1 = lucy_dp(n, sumg1, g1), sumgp2 = lucy_dp(n, sumg2, g2);
vector<long long> sumfp(2 * s);
for (int i = 0; i < 2 * s; i++) {
	sumfp[i] = sumgp1[i] + sumgp2[i];
}
auto f = [&] (long long p, int k) { return p * p + k * p; };
vector<long long> sumf = min25_sieve(n, sumfp, f);
cout << sumf.back() << '\n';
```
