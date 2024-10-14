---
data:
  _extendedDependsOn:
  - icon: ':heavy_check_mark:'
    path: library/internal/internal_concepts.hpp
    title: library/internal/internal_concepts.hpp
  - icon: ':heavy_check_mark:'
    path: library/math/poly.hpp
    title: library/math/poly.hpp
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
    library/internal/internal_concepts.hpp\"\n\n#include <concepts>\n\nnamespace internal::concepts\
    \ {\n    template <typename T>\n    concept Addable = requires (T a) {\n     \
    \   a + a;\n        a += a;\n    };\n    template <typename T>\n    concept Subtractable\
    \ = requires (T a) {\n        a - a;\n        a -= a;\n    };\n    template <typename\
    \ T>\n    concept Multipliable = requires (T a) {\n        a * a;\n        a *=\
    \ a;\n    };\n    template <typename T>\n    concept Dividable =  requires (T\
    \ a) {\n        a / a;\n        a /= a;\n    };\n}\n#line 5 \"library/math/multiplicative_function.hpp\"\
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
    \ {\n            sumf[i]++;\n        }\n        return sumf;\n    }\n}\n"
  code: "#pragma once\n\n#include \"library/math/poly.hpp\"\n#include \"library/internal/internal_concepts.hpp\"\
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
    \ {\n            sumf[i]++;\n        }\n        return sumf;\n    }\n}\n"
  dependsOn:
  - library/math/poly.hpp
  - library/internal/internal_concepts.hpp
  isVerificationFile: false
  path: library/math/multiplicative_function.hpp
  requiredBy: []
  timestamp: '2024-10-14 14:00:38+08:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
documentation_of: library/math/multiplicative_function.hpp
layout: document
redirect_from:
- /library/library/math/multiplicative_function.hpp
- /library/library/math/multiplicative_function.hpp.html
title: library/math/multiplicative_function.hpp
---
