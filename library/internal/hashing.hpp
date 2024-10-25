#pragma once

#include <array>
#include <random>
#include <chrono>

namespace maomao90::internal::hashing {
    const int MIN_HASH_BASE = 128;
    template <typename mint, size_t num_bases>
    constexpr array<mint, num_bases> gen_bases() {
        mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());
        array<mint, num_bases> res;
        for (int i = 0; i < num_bases; i++) {
            res[i] = mint::raw(rng() % (mint::umod() - MIN_HASH_BASE) + MIN_HASH_BASE);
        }
        return res;
    }
    template <typename mint, size_t num_bases>
    constexpr array<mint, num_bases> gen_inverse(const array<mint, num_bases> &bases) {
        array<mint, num_bases> res;
        for (int i = 0; i < num_bases; i++) {
            res[i] = bases[i].inv();
        }
        return res;
    }
    template <typename mint, size_t num_bases, size_t CACHE>
    constexpr array<array<mint, CACHE>, num_bases> init_power(const array<mint, num_bases> &bases) {
        array<array<mint, CACHE>, num_bases> res;
        for (int i = 0; i < num_bases; i++) {
            res[i][0] = 1;
            for (int j = 1; j < CACHE; j++) {
                res[i][j] = res[i][j - 1] * bases[i];
            }
        }
        return res;
    }
    template <size_t num_bases>
    constexpr array<unsigned long long, num_bases> gen_fixed_random() {
        mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());
        array<unsigned long long, num_bases> res;
        for (int i = 0; i < num_bases; i++) {
            res[i] = rng();
        }
        return res;
    }

    template <typename T>
    constexpr unsigned long long hash_function(const T &x) {
        static unsigned long long r = chrono::high_resolution_clock::now().time_since_epoch().count();
        constexpr unsigned long long z1 = 11995408973635179863ULL;
        if constexpr (internal::type_traits::is_broadly_integral_v<T>) {
            return ((unsigned long long) x ^ r) * z1;
        } else if constexpr (internal::type_traits::is_pair<T>) {
            constexpr unsigned long long z2 = 10150724397891781847ULL;
            return hash_function(x.first) + hash_function(x.second) * z2;
        } else if constexpr (internal::concepts::Iterable<T>) {
            constexpr unsigned long long mod = (1LL << 61) - 1;
            constexpr unsigned long long base = 950699498548472943ULL;
            unsigned long long m = 0;
            for (auto& z : x) {
                unsigned long long w;
                if constexpr (is_broadly_integral_v<T>) {
                    w = (unsigned long long) z ^ r;
                } else {
                    w = hash_function(z);
                }
                unsigned __int128 y = (unsigned __int128) m * base + (w & mod);
                m = (y & mod) + (y >> 61);
                if (m >= mod) m -= mod;
            }
            m ^= m << 24, m ^= m >> 31, m ^= m << 35;
            return m;
        }
    }

    template <typename T>
    struct HashObject {
        constexpr size_t operator()(const T &o) const {
            return hash_function(o);
        }
    }
}
