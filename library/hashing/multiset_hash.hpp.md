---
data:
  _extendedDependsOn:
  - icon: ':warning:'
    path: library/hashing/splitmix.hpp
    title: Splitmix
  - icon: ':heavy_check_mark:'
    path: library/internal/concepts.hpp
    title: library/internal/concepts.hpp
  - icon: ':warning:'
    path: library/internal/hashing.hpp
    title: library/internal/hashing.hpp
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
  _pathExtension: hpp
  _verificationStatusIcon: ':warning:'
  attributes:
    links: []
  bundledCode: "#line 2 \"library/hashing/multiset_hash.hpp\"\n\n#line 2 \"library/math/modint.hpp\"\
    \n\n#line 2 \"library/math/extended_gcd.hpp\"\n\n#line 2 \"library/internal/type_traits.hpp\"\
    \n\n#include <type_traits>\n#include <limits>\n\nnamespace maomao90::internal::type_traits\
    \ {\n#define ENABLE_VALUE(x) \\\n    template <typename T> \\\n    constexpr bool\
    \ x##_v = x<T>::value\n#define ENABLE_TYPE(x) \\\n    template <typename T> \\\
    \n    using x##_t = typename x<T>::type\n\n    template <typename T>\n    using\
    \ is_32bit_or_less = bool_constant<numeric_limits<T>::max() <= numeric_limits<unsigned\
    \ int>::max()>;\n    ENABLE_VALUE(is_32bit_or_less);\n\n    template <typename\
    \ T>\n    using is_64bit_or_less = bool_constant<numeric_limits<T>::max() <= numeric_limits<unsigned\
    \ long long>::max()>;\n    ENABLE_VALUE(is_64bit_or_less);\n\n    template <typename\
    \ T>\n    using safely_multipliable_unsigned = conditional<is_32bit_or_less_v<T>,\
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
    \    };\n}\n#line 2 \"library/hashing/splitmix.hpp\"\n\nnamespace maomao90 {\n\
    \    constexpr unsigned long long splitmix64(unsigned long long x) {\n       \
    \ // http://xorshift.di.unimi.it/splitmix64.c\n        x += 0x9e3779b97f4a7c15;\n\
    \        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;\n        x = (x ^ (x >> 27))\
    \ * 0x94d049bb133111eb;\n        return x ^ (x >> 31);\n    }\n}\n#line 2 \"library/internal/hashing.hpp\"\
    \n\n#include <array>\n#include <random>\n#include <chrono>\n\nnamespace maomao90::internal::hashing\
    \ {\n    const int MIN_HASH_BASE = 128;\n    template <typename mint, size_t num_bases>\n\
    \    constexpr array<mint, num_bases> gen_bases() {\n        mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());\n\
    \        array<mint, num_bases> res;\n        for (int i = 0; i < num_bases; i++)\
    \ {\n            res[i] = mint::raw(rng() % (mint::umod() - MIN_HASH_BASE) + MIN_HASH_BASE);\n\
    \        }\n        return res;\n    }\n    template <typename mint, size_t num_bases>\n\
    \    constexpr array<mint, num_bases> gen_inverse(const array<mint, num_bases>\
    \ &bases) {\n        array<mint, num_bases> res;\n        for (int i = 0; i <\
    \ num_bases; i++) {\n            res[i] = bases[i].inv();\n        }\n       \
    \ return res;\n    }\n    template <typename mint, size_t num_bases, size_t CACHE>\n\
    \    constexpr array<array<mint, CACHE>, num_bases> init_power(const array<mint,\
    \ num_bases> &bases) {\n        array<array<mint, CACHE>, num_bases> res;\n  \
    \      for (int i = 0; i < num_bases; i++) {\n            res[i][0] = 1;\n   \
    \         for (int j = 1; j < CACHE; j++) {\n                res[i][j] = res[i][j\
    \ - 1] * bases[i];\n            }\n        }\n        return res;\n    }\n   \
    \ template <size_t num_bases>\n    constexpr array<unsigned long long, num_bases>\
    \ gen_fixed_random() {\n        mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());\n\
    \        array<unsigned long long, num_bases> res;\n        for (int i = 0; i\
    \ < num_bases; i++) {\n            res[i] = rng();\n        }\n        return\
    \ res;\n    }\n}\n#line 6 \"library/hashing/multiset_hash.hpp\"\n\n#line 8 \"\
    library/hashing/multiset_hash.hpp\"\n\nnamespace maomao90 {\n    template <Modint\
    \ modint = static_modint<(1ll << 61) - 1>, size_t num_bases = 1, size_t CACHE\
    \ = 1'000'000>\n    struct MultisetHash {\n        using mint = modint;\n    \
    \    static constexpr size_t get_num_bases() {\n            return num_bases;\n\
    \        }\n\n        constexpr array<unsigned long long, num_bases> get_v() const\
    \ {\n            array<unsigned long long, num_bases> res;\n            for (int\
    \ i = 0; i < num_bases; i++) {\n                res[i] = _v[i].val();\n      \
    \      }\n            return res;\n        }\n        // insert `cnt` copies of\
    \ `a` into multiset\n        constexpr MultisetHash insert(long long a, long long\
    \ cnt = 1) const {\n            MultisetHash res = *this;\n            for (int\
    \ i = 0; i < num_bases; i++) {\n                res._v[i] += get_pow(i, a) * cnt;\n\
    \            }\n            return res;\n        }\n        // erase `cnt` copies\
    \ of `a` into multiset\n        constexpr MultisetHash erase(long long a, long\
    \ long cnt = 1) const {\n            MultisetHash res = *this;\n            for\
    \ (int i = 0; i < num_bases; i++) {\n                res._v[i] -= get_pow(i, a)\
    \ * cnt;\n            }\n            return res;\n        }\n        // increases\
    \ all values in multiset by `delta` \n        // (`delta` can be negative)\n \
    \       constexpr MultisetHash offset(long long delta) const {\n            MultisetHash\
    \ res = *this;\n            for (int i = 0; i < num_bases; i++) {\n          \
    \      res._v[i] *= get_pow(i, delta);\n            }\n            return res;\n\
    \        }\n        constexpr MultisetHash operator+(const MultisetHash &o) const\
    \ {\n            MultisetHash res = *this;\n            return res += o;\n   \
    \     }\n        constexpr MultisetHash& operator+=(const MultisetHash &o) {\n\
    \            for (int i = 0; i < num_bases; i++) {\n                _v[i] += o._v[i];\n\
    \            }\n            return *this;\n        }\n        constexpr MultisetHash\
    \ operator-(const MultisetHash &o) const {\n            MultisetHash res = *this;\n\
    \            return res -= o;\n        }\n        constexpr MultisetHash& operator-=(const\
    \ MultisetHash &o) {\n            for (int i = 0; i < num_bases; i++) {\n    \
    \            _v[i] -= o._v[i];\n            }\n            return *this;\n   \
    \     }\n        constexpr bool operator==(const MultisetHash &o) const {\n  \
    \          for (int i = 0; i < num_bases; i++) {\n                if (_v[i] !=\
    \ o._v[i]) {\n                    return false;\n                }\n         \
    \   }\n            return true;\n        }\n        constexpr bool operator!=(const\
    \ MultisetHash &o) const {\n            return !(*this == o);\n        }\n   \
    \     private:\n        array<mint, num_bases> _v;\n        inline static const\
    \ array<mint, num_bases> \n            bases = internal::hashing::gen_bases<mint,\
    \ num_bases>(),\n                  inv_bases = internal::hashing::gen_inverse<mint,\
    \ num_bases>(bases);\n        inline static const array<array<mint, CACHE>, num_bases>\n\
    \            bases_pow = internal::hashing::init_power<mint, num_bases, CACHE>(bases),\n\
    \                      inv_bases_pow = internal::hashing::init_power<mint, num_bases,\
    \ CACHE>(inv_bases);\n        constexpr mint get_pow(int i, int p) const {\n \
    \           assert(i < num_bases);\n            if (abs(p) < CACHE) {\n      \
    \          return p >= 0 ? bases_pow[i][p] : inv_bases_pow[i][-p];\n         \
    \   } else {\n                return p >= 0 ? bases[i].pow(p) : inv_bases[i].pow(-p);\n\
    \            }\n        }\n    };\n}\n"
  code: "#pragma once\n\n#include \"library/math/modint.hpp\"\n#include \"library/hashing/splitmix.hpp\"\
    \n#include \"library/internal/hashing.hpp\"\n\n#include <array>\n\nnamespace maomao90\
    \ {\n    template <Modint modint = static_modint<(1ll << 61) - 1>, size_t num_bases\
    \ = 1, size_t CACHE = 1'000'000>\n    struct MultisetHash {\n        using mint\
    \ = modint;\n        static constexpr size_t get_num_bases() {\n            return\
    \ num_bases;\n        }\n\n        constexpr array<unsigned long long, num_bases>\
    \ get_v() const {\n            array<unsigned long long, num_bases> res;\n   \
    \         for (int i = 0; i < num_bases; i++) {\n                res[i] = _v[i].val();\n\
    \            }\n            return res;\n        }\n        // insert `cnt` copies\
    \ of `a` into multiset\n        constexpr MultisetHash insert(long long a, long\
    \ long cnt = 1) const {\n            MultisetHash res = *this;\n            for\
    \ (int i = 0; i < num_bases; i++) {\n                res._v[i] += get_pow(i, a)\
    \ * cnt;\n            }\n            return res;\n        }\n        // erase\
    \ `cnt` copies of `a` into multiset\n        constexpr MultisetHash erase(long\
    \ long a, long long cnt = 1) const {\n            MultisetHash res = *this;\n\
    \            for (int i = 0; i < num_bases; i++) {\n                res._v[i]\
    \ -= get_pow(i, a) * cnt;\n            }\n            return res;\n        }\n\
    \        // increases all values in multiset by `delta` \n        // (`delta`\
    \ can be negative)\n        constexpr MultisetHash offset(long long delta) const\
    \ {\n            MultisetHash res = *this;\n            for (int i = 0; i < num_bases;\
    \ i++) {\n                res._v[i] *= get_pow(i, delta);\n            }\n   \
    \         return res;\n        }\n        constexpr MultisetHash operator+(const\
    \ MultisetHash &o) const {\n            MultisetHash res = *this;\n          \
    \  return res += o;\n        }\n        constexpr MultisetHash& operator+=(const\
    \ MultisetHash &o) {\n            for (int i = 0; i < num_bases; i++) {\n    \
    \            _v[i] += o._v[i];\n            }\n            return *this;\n   \
    \     }\n        constexpr MultisetHash operator-(const MultisetHash &o) const\
    \ {\n            MultisetHash res = *this;\n            return res -= o;\n   \
    \     }\n        constexpr MultisetHash& operator-=(const MultisetHash &o) {\n\
    \            for (int i = 0; i < num_bases; i++) {\n                _v[i] -= o._v[i];\n\
    \            }\n            return *this;\n        }\n        constexpr bool operator==(const\
    \ MultisetHash &o) const {\n            for (int i = 0; i < num_bases; i++) {\n\
    \                if (_v[i] != o._v[i]) {\n                    return false;\n\
    \                }\n            }\n            return true;\n        }\n     \
    \   constexpr bool operator!=(const MultisetHash &o) const {\n            return\
    \ !(*this == o);\n        }\n        private:\n        array<mint, num_bases>\
    \ _v;\n        inline static const array<mint, num_bases> \n            bases\
    \ = internal::hashing::gen_bases<mint, num_bases>(),\n                  inv_bases\
    \ = internal::hashing::gen_inverse<mint, num_bases>(bases);\n        inline static\
    \ const array<array<mint, CACHE>, num_bases>\n            bases_pow = internal::hashing::init_power<mint,\
    \ num_bases, CACHE>(bases),\n                      inv_bases_pow = internal::hashing::init_power<mint,\
    \ num_bases, CACHE>(inv_bases);\n        constexpr mint get_pow(int i, int p)\
    \ const {\n            assert(i < num_bases);\n            if (abs(p) < CACHE)\
    \ {\n                return p >= 0 ? bases_pow[i][p] : inv_bases_pow[i][-p];\n\
    \            } else {\n                return p >= 0 ? bases[i].pow(p) : inv_bases[i].pow(-p);\n\
    \            }\n        }\n    };\n}\n"
  dependsOn:
  - library/math/modint.hpp
  - library/math/extended_gcd.hpp
  - library/internal/type_traits.hpp
  - library/math/primality_test.hpp
  - library/math/pow_mod.hpp
  - library/internal/concepts.hpp
  - library/hashing/splitmix.hpp
  - library/internal/hashing.hpp
  isVerificationFile: false
  path: library/hashing/multiset_hash.hpp
  requiredBy: []
  timestamp: '2024-10-22 15:04:08+08:00'
  verificationStatus: LIBRARY_NO_TESTS
  verifiedWith: []
documentation_of: library/hashing/multiset_hash.hpp
layout: document
title: Multiset Hash
---
