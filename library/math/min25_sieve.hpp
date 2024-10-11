#pragma once

#include "library/math/poly.hpp"

#include <concepts>
#include <utility>
#include <vector>

namespace maomao90 {
    template <typename T, typename F> requires 
        requires(F f, long long prime, int power) { { f(prime, power) } -> same_as<T>; }
    vector<pair<long long, T>> min25_sieve(long long n, F f, Poly<T> g) {
        long long sqrt = 1;
        while ((sqrt + 1) * (sqrt + 1) <= n) {
            sqrt++;
        }
        vector<bool> is_prime(sqrt + 1);
        for (int i = 2; i <= sqrt; i++) {
            is_prime[i] = 1;
        }
        for (int i = 2; i * i <= sqrt; i++) {
            if (!is_prime[i]) {
                continue;
            }
            for (int j = i * i; j <= sqrt; j += i) {
                is_prime[j] = 0;
            }
        }
        vector<T> small_fp(sqrt + 1), big_fp(sqrt + 1), small_sieve(sqrt + 1), big_sieve(sqrt + 1), pow(sqrt + 1);
        vector<long long> div(sqrt + 1);
        for (int i = 1; i <= sqrt; i++) {
            pow[i] = 1;
            div[i] = n / i;
        }
        auto interpolate = [&] (int d, long long x) {
            if (d == 0) {
                return T(x) - 1;
            } else if (d == 1) {
                return T(x) * (x + 1) / 2 - 1;
            } else {
                return T(0);
            }
        };
        for (int d = 0; d <= g.degree(); d++) {
            vector<T> small_gp(sqrt + 1), big_gp(sqrt + 1);
            for (int i = 1; i <= sqrt; i++) {
                // initialise small_gp and big_gp
                small_gp[i] = interpolate(d, i);
                big_gp[i] = interpolate(d, div[i]);
            }
            for (int prime = 2; prime <= sqrt; prime++) {
                if (!is_prime[prime]) {
                    continue;
                }
                for (int i = 1; i <= sqrt; i++) {
                    if (div[i] < (long long) prime * prime) {
                        break;
                    }
                    T div_gp = (long long) i * prime <= sqrt ? big_gp[i * prime] : small_gp[div[i] / prime];
                    big_gp[i] -= pow[prime] * (div_gp - small_gp[prime - 1]);
                }
                for (int i = sqrt; i >= (long long) prime * prime; i--) {
                    small_gp[i] -= pow[prime] * (small_gp[i / prime] - small_gp[prime - 1]);
                }
                pow[prime] *= prime;
            }
            for (int i = 1; i <= sqrt; i++) {
                small_fp[i] += small_gp[i] * g[d];
                big_fp[i] += big_gp[i] * g[d];
            }
        }
        for (int i = 1; i <= sqrt; i++) {
            small_sieve[i] = small_fp[i];
            big_sieve[i] = big_fp[i];
        }
        for (int prime = sqrt; prime >= 2; prime--) {
            if (!is_prime[prime]) {
                continue;
            }
            for (int i = 1; i <= sqrt; i++) {
                if (div[i] < (long long) prime * prime) {
                    break;
                }
                int pow = 1;
                for (long long prime_pow = prime; prime_pow <= div[i] / prime; prime_pow *= prime, pow++) {
                    T div_sieve = i * prime_pow <= sqrt ? big_sieve[i * prime_pow] : small_sieve[div[i] / prime_pow];
                    big_sieve[i] += f(prime, pow) * (div_sieve - small_fp[prime]) + f(prime, pow + 1);
                }
            }
            for (int i = sqrt; i >= (long long) prime * prime; i--) {
                int pow = 1;
                for (long long prime_pow = prime; prime_pow <= i / prime; prime_pow *= prime, pow++) {
                    small_sieve[i] += f(prime, pow) * (small_sieve[i / prime_pow] - small_fp[prime]) + f(prime, pow + 1);
                }
            }
        }
        vector<pair<long long, T>> res(2 * sqrt);
        for (int i = 1; i <= sqrt; i++) {
            res[i - 1] = {i, small_sieve[i] + 1};
            res[2 * sqrt - i] = {div[i], big_sieve[i] + 1};
        }
        return res;
    }
}
