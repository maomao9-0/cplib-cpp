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
    path: library/math/pow_mod.hpp
    title: Pow Mod
  - icon: ':heavy_check_mark:'
    path: library/math/primality_test.hpp
    title: Primality Test
  _extendedRequiredBy:
  - icon: ':warning:'
    path: library/hashing/multiset_hash.hpp
    title: Multiset Hash
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
  bundledCode: "#line 2 \"library/math/modint.hpp\"\n\n#line 2 \"library/math/extended_gcd.hpp\"\
    \n\n#line 2 \"library/internal/type_traits.hpp\"\n\n#include <type_traits>\n#include\
    \ <limits>\n\nnamespace maomao90::internal::type_traits {\n#define ENABLE_VALUE(x)\
    \ \\\n    template <typename T> \\\n    constexpr bool x##_v = x<T>::value\n#define\
    \ ENABLE_TYPE(x) \\\n    template <typename T> \\\n    using x##_t = typename\
    \ x<T>::type\n\n    template <typename T>\n    using is_32bit_or_less = bool_constant<numeric_limits<T>::max()\
    \ <= numeric_limits<unsigned int>::max()>;\n    ENABLE_VALUE(is_32bit_or_less);\n\
    \n    template <typename T>\n    using is_64bit_or_less = bool_constant<numeric_limits<T>::max()\
    \ <= numeric_limits<unsigned long long>::max()>;\n    ENABLE_VALUE(is_64bit_or_less);\n\
    \n    template <typename T>\n    using safely_multipliable_unsigned = conditional<is_32bit_or_less_v<T>,\
    \ unsigned long long, unsigned __int128>;\n    ENABLE_TYPE(safely_multipliable_unsigned);\n\
    \n    template <typename T>\n    using safely_multipliable_signed = conditional<is_32bit_or_less_v<T>,\
    \ long long, __int128>;\n    ENABLE_TYPE(safely_multipliable_signed);\n\n    template\
    \ <typename T>\n    using safely_multipliable = conditional<is_signed_v<T>, safely_multipliable_signed_t<T>,\
    \ safely_multipliable_unsigned_t<T>>;\n    ENABLE_TYPE(safely_multipliable);\n\
    \n#undef ENABLE_VALUE\n}\n#line 4 \"library/math/extended_gcd.hpp\"\n\n#include\
    \ <cassert>\n#line 7 \"library/math/extended_gcd.hpp\"\n#include <concepts>\n\n\
    namespace maomao90 {\n    template <signed_integral T> requires\n        internal::type_traits::is_64bit_or_less_v<T>\n\
    \    constexpr T inv_gcd(T x, T mod) {\n        using U = internal::type_traits::safely_multipliable_t<T>;\n\
    \        U a = mod, b = x, va = 0, vb = 1;\n        while (b) {\n            T\
    \ k = a / b;\n            a -= k * b; va -= k * vb;\n            swap(a, b);\n\
    \            swap(va, vb);\n        }\n        assert(a == 1); // gcd should be\
    \ equal to 1\n        return (va % mod + mod) % mod;\n    }\n}\n#line 2 \"library/math/primality_test.hpp\"\
    \n\n#line 2 \"library/math/pow_mod.hpp\"\n\n#line 4 \"library/math/pow_mod.hpp\"\
    \n\n#line 7 \"library/math/pow_mod.hpp\"\n\nnamespace maomao90 {\n    template\
    \ <integral T, unsigned_integral P> requires\n        internal::type_traits::is_64bit_or_less_v<T>\n\
    \    constexpr T pow_mod(T b, P p, T mod) {\n        using U = internal::type_traits::safely_multipliable_t<T>;\n\
    \        T res = 1;\n        b %= mod;\n        if (b < 0) {\n            b +=\
    \ mod;\n        }\n        while (p) {\n            if (p & 1) {\n           \
    \     res = (U) res * b % mod;\n            }\n            b = (U) b * b % mod;\n\
    \            p >>= 1;\n        }\n        return res;\n    }\n}\n#line 5 \"library/math/primality_test.hpp\"\
    \n\n#line 8 \"library/math/primality_test.hpp\"\n\nnamespace maomao90 {\n    template\
    \ <unsigned_integral T>\n    constexpr bool miller_rabin(const T &n, const T *bases,\
    \ const size_t size) {\n        using U = internal::type_traits::safely_multipliable_t<T>;\n\
    \        if (n <= 1) {\n            return false;\n        }\n        if (n ==\
    \ 2) {\n            return true;\n        }\n        if (n % 2 == 0) {\n     \
    \       return false;\n        }\n        T d = n - 1;\n        while (d % 2 ==\
    \ 0) d /= 2;\n        for (int i = 0; i < size; i++) {\n            T a = bases[i];\n\
    \            if (a % n == 0) {\n                continue;\n            }\n   \
    \         T t = d, y = pow_mod<T, T>(a, t, n);\n            while (t != n - 1\
    \ && y != 1 && y != n - 1) {\n                y = (U) y * y % n;\n           \
    \     t <<= 1;\n            }\n            if (y != n - 1 && t % 2 == 0) {\n \
    \               return false;\n            }\n        }\n        return true;\n\
    \    }\n    template <unsigned_integral T>\n    constexpr bool is_prime(T n) {\n\
    \        if constexpr (internal::type_traits::is_32bit_or_less_v<T>) {\n     \
    \       T bases[3] = {2, 7, 61};\n            return miller_rabin<T>(n, bases,\
    \ 3);\n        } else {\n            T bases[7] = {2, 325, 9375, 28178, 450775,\
    \ 9780504, 1795265022};\n            return miller_rabin<T>(n, bases, 7);\n  \
    \      }\n    }\n}\n#line 2 \"library/internal/concepts.hpp\"\n\n#line 4 \"library/internal/concepts.hpp\"\
    \n\nnamespace maomao90::internal::concepts {\n    template <typename T>\n    concept\
    \ Addable = requires (T a) {\n        a + a;\n        a += a;\n    };\n    template\
    \ <typename T>\n    concept Subtractable = requires (T a) {\n        a - a;\n\
    \        a -= a;\n    };\n    template <typename T>\n    concept Multipliable\
    \ = requires (T a) {\n        a * a;\n        a *= a;\n    };\n    template <typename\
    \ T>\n    concept Dividable = requires (T a) {\n        a / a;\n        a /= a;\n\
    \    };\n}\n#line 7 \"library/math/modint.hpp\"\n\n#line 10 \"library/math/modint.hpp\"\
    \n#include <iostream>\n#line 12 \"library/math/modint.hpp\"\n\nnamespace maomao90\
    \ {\n    template <typename T>\n    concept Modint = requires (T a, long long\
    \ p) {\n        { T::imod() } -> same_as<typename T::mod_type>;\n        { T::umod()\
    \ } -> same_as<typename T::umod_type>;\n        a++; a--; ++a; --a;\n        +a;\
    \ -a;\n        a.pow(p); a.inv();\n    } &&\n        internal::concepts::Addable<T>\
    \ &&\n        internal::concepts::Subtractable<T> &&\n        internal::concepts::Multipliable<T>\
    \ &&\n        internal::concepts::Dividable<T> &&\n        equality_comparable<T>;\n\
    \n    template <auto mod = 998244353, enable_if_t<(mod >= 1), nullptr_t> = nullptr>\
    \ requires \n        signed_integral<decltype(mod)> && \n        internal::type_traits::is_64bit_or_less_v<decltype(mod)>\n\
    \    struct static_modint {\n    private:\n        using M = decltype(mod);\n\
    \        using UM = make_unsigned_t<M>;\n        using BM = internal::type_traits::safely_multipliable_t<M>;\n\
    \    public:\n        using mod_type = M;\n        using umod_type = UM;\n   \
    \     static constexpr M imod() {\n            return mod;\n        }\n      \
    \  static constexpr UM umod() {\n            return mod;\n        }\n\n      \
    \  static constexpr static_modint raw(M v) {\n            static_modint res;\n\
    \            res._v = v;\n            return res;\n        }\n\n        constexpr\
    \ static_modint(): _v(0) {}\n\n        template <signed_integral T>\n        constexpr\
    \ static_modint(T v) {\n            M x = v % imod();\n            if (x < 0)\
    \ {\n                x += imod();\n            }\n            _v = x;\n      \
    \  }\n\n        template <unsigned_integral T>\n        constexpr static_modint(T\
    \ v): _v(v % umod()) {}\n\n        constexpr UM val() const {\n            return\
    \ _v;\n        }\n\n        constexpr static_modint operator+() const {\n    \
    \        return *this;\n        }\n        constexpr static_modint operator-()\
    \ const {\n            return raw(_v == 0 ? 0 : imod() - _v);\n        }\n\n \
    \       constexpr static_modint& operator++() {\n            ++_v;\n         \
    \   if (_v == umod()) {\n                _v = 0;\n            }\n            return\
    \ *this;\n        }\n        constexpr static_modint& operator--() {\n       \
    \     if (_v == 0) {\n                _v = umod();\n            }\n          \
    \  --_v;\n            return *this;\n        }\n        constexpr static_modint\
    \ operator++(int) {\n            static_modint x = *this;\n            ++*this;\n\
    \            return x;\n        }\n        constexpr static_modint operator--(int)\
    \ {\n            static_modint x = *this;\n            --*this;\n            return\
    \ x;\n        }\n\n        constexpr static_modint& operator+=(const static_modint\
    \ &o) {\n            _v += o._v;\n            if (_v >= umod()) {\n          \
    \      _v -= umod();\n            }\n            return *this;\n        }\n  \
    \      constexpr static_modint& operator-=(const static_modint &o) {\n       \
    \     if (_v < o._v) {\n                _v += umod();\n            }\n       \
    \     _v -= o._v;\n            return *this;\n        }\n        constexpr static_modint&\
    \ operator*=(const static_modint &o) {\n            _v = (BM) _v * o._v % umod();\n\
    \            return *this;\n        }\n        constexpr static_modint& operator/=(const\
    \ static_modint &o) {\n            return *this *= o.inv();\n        }\n\n   \
    \     constexpr static_modint pow(long long p) const {\n            assert(p >=\
    \ 0);\n            static_modint res = 1, b = *this;\n            while (p) {\n\
    \                if (p & 1) {\n                    res *= b;\n               \
    \ }\n                b *= b;\n                p >>= 1;\n            }\n      \
    \      return res;\n        }\n        constexpr static_modint inv() const {\n\
    \            if constexpr (is_prime_mod) {\n                return pow(imod()\
    \ - 2);\n            } else {\n                return raw(inv_gcd((M) _v, imod()));\n\
    \            }\n        }\n\n        friend constexpr static_modint operator+(const\
    \ static_modint &l, const static_modint &r) {\n            static_modint res =\
    \ l;\n            return res += r;\n        }\n        friend constexpr static_modint\
    \ operator-(const static_modint &l, const static_modint &r) {\n            static_modint\
    \ res = l;\n            return res -= r;\n        }\n        friend constexpr\
    \ static_modint operator*(const static_modint &l, const static_modint &r) {\n\
    \            static_modint res = l;\n            return res *= r;\n        }\n\
    \        friend constexpr static_modint operator/(const static_modint &l, const\
    \ static_modint &r) {\n            static_modint res = l;\n            return\
    \ res /= r;\n        }\n\n        constexpr bool operator==(const static_modint\
    \ &o) const {\n            return _v == o._v;\n        }\n        constexpr bool\
    \ operator!=(const static_modint &o) const {\n            return !(*this == o);\n\
    \        }\n\n        friend constexpr istream& operator>>(istream &is, static_modint\
    \ &o) {\n            M v; is >> v;\n            o = static_modint(v);\n      \
    \      return is;\n        }\n        friend constexpr ostream& operator<<(ostream\
    \ &os, const static_modint &o) {\n            return os << o._v;\n        }\n\
    \    private:\n        UM _v;\n        static constexpr bool is_prime_mod = is_prime(umod());\n\
    \    };\n}\n"
  code: "#pragma once\n\n#include \"library/math/extended_gcd.hpp\"\n#include \"library/math/primality_test.hpp\"\
    \n#include \"library/internal/concepts.hpp\"\n#include \"library/internal/type_traits.hpp\"\
    \n\n#include <type_traits>\n#include <concepts>\n#include <iostream>\n#include\
    \ <cassert>\n\nnamespace maomao90 {\n    template <typename T>\n    concept Modint\
    \ = requires (T a, long long p) {\n        { T::imod() } -> same_as<typename T::mod_type>;\n\
    \        { T::umod() } -> same_as<typename T::umod_type>;\n        a++; a--; ++a;\
    \ --a;\n        +a; -a;\n        a.pow(p); a.inv();\n    } &&\n        internal::concepts::Addable<T>\
    \ &&\n        internal::concepts::Subtractable<T> &&\n        internal::concepts::Multipliable<T>\
    \ &&\n        internal::concepts::Dividable<T> &&\n        equality_comparable<T>;\n\
    \n    template <auto mod = 998244353, enable_if_t<(mod >= 1), nullptr_t> = nullptr>\
    \ requires \n        signed_integral<decltype(mod)> && \n        internal::type_traits::is_64bit_or_less_v<decltype(mod)>\n\
    \    struct static_modint {\n    private:\n        using M = decltype(mod);\n\
    \        using UM = make_unsigned_t<M>;\n        using BM = internal::type_traits::safely_multipliable_t<M>;\n\
    \    public:\n        using mod_type = M;\n        using umod_type = UM;\n   \
    \     static constexpr M imod() {\n            return mod;\n        }\n      \
    \  static constexpr UM umod() {\n            return mod;\n        }\n\n      \
    \  static constexpr static_modint raw(M v) {\n            static_modint res;\n\
    \            res._v = v;\n            return res;\n        }\n\n        constexpr\
    \ static_modint(): _v(0) {}\n\n        template <signed_integral T>\n        constexpr\
    \ static_modint(T v) {\n            M x = v % imod();\n            if (x < 0)\
    \ {\n                x += imod();\n            }\n            _v = x;\n      \
    \  }\n\n        template <unsigned_integral T>\n        constexpr static_modint(T\
    \ v): _v(v % umod()) {}\n\n        constexpr UM val() const {\n            return\
    \ _v;\n        }\n\n        constexpr static_modint operator+() const {\n    \
    \        return *this;\n        }\n        constexpr static_modint operator-()\
    \ const {\n            return raw(_v == 0 ? 0 : imod() - _v);\n        }\n\n \
    \       constexpr static_modint& operator++() {\n            ++_v;\n         \
    \   if (_v == umod()) {\n                _v = 0;\n            }\n            return\
    \ *this;\n        }\n        constexpr static_modint& operator--() {\n       \
    \     if (_v == 0) {\n                _v = umod();\n            }\n          \
    \  --_v;\n            return *this;\n        }\n        constexpr static_modint\
    \ operator++(int) {\n            static_modint x = *this;\n            ++*this;\n\
    \            return x;\n        }\n        constexpr static_modint operator--(int)\
    \ {\n            static_modint x = *this;\n            --*this;\n            return\
    \ x;\n        }\n\n        constexpr static_modint& operator+=(const static_modint\
    \ &o) {\n            _v += o._v;\n            if (_v >= umod()) {\n          \
    \      _v -= umod();\n            }\n            return *this;\n        }\n  \
    \      constexpr static_modint& operator-=(const static_modint &o) {\n       \
    \     if (_v < o._v) {\n                _v += umod();\n            }\n       \
    \     _v -= o._v;\n            return *this;\n        }\n        constexpr static_modint&\
    \ operator*=(const static_modint &o) {\n            _v = (BM) _v * o._v % umod();\n\
    \            return *this;\n        }\n        constexpr static_modint& operator/=(const\
    \ static_modint &o) {\n            return *this *= o.inv();\n        }\n\n   \
    \     constexpr static_modint pow(long long p) const {\n            assert(p >=\
    \ 0);\n            static_modint res = 1, b = *this;\n            while (p) {\n\
    \                if (p & 1) {\n                    res *= b;\n               \
    \ }\n                b *= b;\n                p >>= 1;\n            }\n      \
    \      return res;\n        }\n        constexpr static_modint inv() const {\n\
    \            if constexpr (is_prime_mod) {\n                return pow(imod()\
    \ - 2);\n            } else {\n                return raw(inv_gcd((M) _v, imod()));\n\
    \            }\n        }\n\n        friend constexpr static_modint operator+(const\
    \ static_modint &l, const static_modint &r) {\n            static_modint res =\
    \ l;\n            return res += r;\n        }\n        friend constexpr static_modint\
    \ operator-(const static_modint &l, const static_modint &r) {\n            static_modint\
    \ res = l;\n            return res -= r;\n        }\n        friend constexpr\
    \ static_modint operator*(const static_modint &l, const static_modint &r) {\n\
    \            static_modint res = l;\n            return res *= r;\n        }\n\
    \        friend constexpr static_modint operator/(const static_modint &l, const\
    \ static_modint &r) {\n            static_modint res = l;\n            return\
    \ res /= r;\n        }\n\n        constexpr bool operator==(const static_modint\
    \ &o) const {\n            return _v == o._v;\n        }\n        constexpr bool\
    \ operator!=(const static_modint &o) const {\n            return !(*this == o);\n\
    \        }\n\n        friend constexpr istream& operator>>(istream &is, static_modint\
    \ &o) {\n            M v; is >> v;\n            o = static_modint(v);\n      \
    \      return is;\n        }\n        friend constexpr ostream& operator<<(ostream\
    \ &os, const static_modint &o) {\n            return os << o._v;\n        }\n\
    \    private:\n        UM _v;\n        static constexpr bool is_prime_mod = is_prime(umod());\n\
    \    };\n}\n"
  dependsOn:
  - library/math/extended_gcd.hpp
  - library/internal/type_traits.hpp
  - library/math/primality_test.hpp
  - library/math/pow_mod.hpp
  - library/internal/concepts.hpp
  isVerificationFile: false
  path: library/math/modint.hpp
  requiredBy:
  - library/hashing/multiset_hash.hpp
  timestamp: '2024-10-22 15:04:08+08:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - verify/yosupo/number_theory/sum_of_multiplicative_function.test.cpp
  - verify/unit_test/math/modint.test.cpp
documentation_of: library/math/modint.hpp
layout: document
title: ModInt
---
