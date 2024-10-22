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
}
