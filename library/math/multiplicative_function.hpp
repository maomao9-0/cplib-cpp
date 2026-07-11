/**
 * @file multiplicative_function.hpp
 * @brief Helpers for Min_25-style prefix sums of multiplicative functions.
 *
 * Given a multiplicative function `f`, `lucy_dp()` and `min25_sieve()` can be
 * combined to compute `sum_{i=1}^N f(i)` in roughly `O(N^{3/4} / log N)` time.
 * As a side effect, they also produce prefix sums on the distinct quotient set
 * `floor(N / k)`.
 *
 * Both helpers expect the caller to prepare the `2 * floor(sqrt(n))` layout
 * correctly; passing an inconsistent buffer shape or wrong prefix sums breaks
 * the formulas.
 */
#pragma once

#include <cassert>
#include <concepts>
#include <vector>

#include "library/internal/concepts.hpp"

namespace maomao90 {
using namespace std;
/**
 * @brief Lucy DP transform for prime-indexed contributions.
 *
 * Let `m = floor(sqrt(n))`. The input `sumg` has size `2 * m`.
 *
 * For `0 <= i < m`, `sumg[i]` represents:
 * `sum_{x=2}^{i+1} g(x)`.
 *
 * For `m <= i < 2 * m`, `sumg[i]` represents:
 * `sum_{x=2}^{floor(n / (2 * m - i))} g(x)`.
 *
 * In other words, `sumg` stores the prefix sum of `g` on the distinct values of
 * `floor(n / k)`, excluding `g(1)`.
 *
 * The callable `g` is only queried on primes. The result has the same layout as
 * `sumg`, but transformed so that only prime indices contribute:
 *
 * For `0 <= i < m`, the returned value is:
 * `sum_{x=2}^{i+1} g(x) * [is_prime(x)]`.
 *
 * For `m <= i < 2 * m`, the returned value is:
 * `sum_{x=2}^{floor(n / (2 * m - i))} g(x) * [is_prime(x)]`.
 *
 * @param n Target upper bound.
 * @param sumg Precomputed prefix sums in the standard Min_25 layout.
 * @param g Function evaluated at primes, where `g(p)` returns the value of `g`
 * at a prime `p`.
 * @return Prefix sums with non-prime contributions removed.
 * Complexity: `O(n^{3/4} / log n)`.
 * @warning Requires `sumg.size() == 2 * floor(sqrt(n))`.
 */
template <typename T, typename G>
  requires requires(G g, long long prime) {
    { g(prime) } -> same_as<T>;
  } && internal::concepts::Addable<T> && internal::concepts::Subtractable<T> &&
           internal::concepts::Multipliable<T>
vector<T> lucy_dp(long long n, vector<T> sumg, G g) {
  assert(sumg.size() % 2 == 0);
  long long m = sumg.size() / 2;
  assert(m * m <= n && (m + 1) * (m + 1) > n);
  vector<bool> is_prime(m + 1);
  for (int i = 2; i <= m; i++) {
    is_prime[i] = 1;
  }
  for (int i = 2; i * i <= m; i++) {
    if (!is_prime[i]) {
      continue;
    }
    for (int j = i * i; j <= m; j += i) {
      is_prime[j] = 0;
    }
  }
  vector<long long> div(m + 1);
  vector<T> sumgp = sumg;
  for (int i = 1; i <= m; i++) {
    div[i] = n / i;
  }
  for (int prime = 2; prime <= m; prime++) {
    if (!is_prime[prime]) {
      continue;
    }
    long long prime_squared = (long long)prime * prime, iprime = prime;
    for (int i = 1; i <= m; i++, iprime += prime) {
      if (div[i] < prime_squared) {
        break;
      }
      T div_val =
          iprime <= m ? sumgp[2 * m - iprime] : sumgp[div[i] / prime - 1];
      sumgp[2 * m - i] -= g(prime) * (div_val - sumgp[prime - 2]);
    }
    for (int i = m; i >= prime_squared; i--) {
      sumgp[i - 1] -= g(prime) * (sumgp[i / prime - 1] - sumgp[prime - 2]);
    }
  }
  return sumgp;
}
/**
 * @brief Min_25 sieve transition from prime contributions to full prefix sums.
 *
 * The input `sumfp` has the same `2 * floor(sqrt(n))` layout as `lucy_dp()`,
 * but stores prime-only prefix sums of `f`:
 *
 * For `0 <= i < m`, `sumfp[i]` is:
 * `sum_{x=2}^{i+1} f(x) * [is_prime(x)]`.
 *
 * For `m <= i < 2 * m`, `sumfp[i]` is:
 * `sum_{x=2}^{floor(n / (2 * m - i))} f(x) * [is_prime(x)]`.
 *
 * The callable `f` is queried as `f(p, k)` and should return the value of
 * `f(p^k)` for a prime `p`. This is enough to recover a multiplicative
 * function on all positive integers.
 *
 * The result again uses the same quotient layout, but now stores the full
 * prefix sum of `f`, including composite values and `f(1)`:
 *
 * For `0 <= i < m`, the returned value is:
 * `sum_{x=1}^{i+1} f(x)`.
 *
 * For `m <= i < 2 * m`, the returned value is:
 * `sum_{x=1}^{floor(n / (2 * m - i))} f(x)`.
 *
 * Complexity: `O(n^{3/4} / log n)`.
 *
 * Example:
 * Suppose `f(p^k) = p^2 + k p`. Then `f(p) = p^2 + p`, but directly feeding
 * `g(p) = p^2 + p` into `lucy_dp()` is invalid because `g` is not
 * multiplicative: `g(2) g(3) = 72`, while `g(6) = 42`.
 *
 * Instead, split the prime contribution into two multiplicative parts:
 * `g1(p) = p` and `g2(p) = p^2`. Their prefix sums are easy to precompute in
 * `O(1)` per queried value, and the prime-only sums produced by `lucy_dp()`
 * can be added together to form `sumfp`.
 *
 * @code{.cpp}
 * #include <cmath>
 * #include <iostream>
 * using namespace std;
 * #include "library/math/multiplicative_function.hpp"
 * using namespace maomao90;
 *
 * int main() {
 *   long long n;
 *   cin >> n;
 *   int s = floor(sqrt(n));
 *   auto g1 = [&](long long p) { return p; };
 *   auto g2 = [&](long long p) { return p * p; };
 *   vector<long long> sumg1(2 * s), sumg2(2 * s);
 *   for (int i = 1; i <= s; i++) {
 *     auto prefix_sum_g1 = [&](long long x) { return x * (x + 1) / 2 - 1; };
 *     sumg1[i - 1] = prefix_sum_g1(i);
 *     sumg1[2 * s - i] = prefix_sum_g1(n / i);
 *     auto prefix_sum_g2 = [&](long long x) {
 *       return x * (x + 1) * (2 * x + 1) / 6 - 1;
 *     };
 *     sumg2[i - 1] = prefix_sum_g2(i);
 *     sumg2[2 * s - i] = prefix_sum_g2(n / i);
 *   }
 *   vector<long long> sumgp1 = lucy_dp(n, sumg1, g1),
 *                     sumgp2 = lucy_dp(n, sumg2, g2);
 *   vector<long long> sumfp(2 * s);
 *   for (int i = 0; i < 2 * s; i++) {
 *     sumfp[i] = sumgp1[i] + sumgp2[i];
 *   }
 *   auto f = [&](long long p, int k) { return p * p + k * p; };
 *   vector<long long> sumf = min25_sieve(n, sumfp, f);
 *   cout << sumf.back() << '\n';
 * }
 * @endcode
 *
 * @param n Target upper bound.
 * @param sumfp Prime-only prefix sums in Min_25 layout.
 * @param f Multiplicative function on prime powers.
 * @return Prefix sums of `f` on the same distinct quotient set.
 * @warning Requires `sumfp.size() == 2 * floor(sqrt(n))`.
 */
template <typename T, typename F>
  requires requires(F f, long long prime, int power) {
    { f(prime, power) } -> same_as<T>;
  } && internal::concepts::Addable<T> && internal::concepts::Subtractable<T> &&
           internal::concepts::Multipliable<T>
vector<T> min25_sieve(long long n, vector<T> sumfp, F f) {
  assert(sumfp.size() % 2 == 0);
  long long m = sumfp.size() / 2;
  assert(m * m <= n && (m + 1) * (m + 1) > n);
  vector<bool> is_prime(m + 1);
  for (int i = 2; i <= m; i++) {
    is_prime[i] = 1;
  }
  for (int i = 2; i * i <= m; i++) {
    if (!is_prime[i]) {
      continue;
    }
    for (int j = i * i; j <= m; j += i) {
      is_prime[j] = 0;
    }
  }
  vector<long long> div(m + 1);
  vector<T> sumf = sumfp, _sumf = sumf;
  for (int i = 1; i <= m; i++) {
    div[i] = n / i;
  }
  for (int prime = m; prime >= 2; prime--) {
    if (!is_prime[prime]) {
      continue;
    }
    int pow = 1;
    for (long long prime_pow = prime; div[prime] >= prime_pow;
         prime_pow *= prime, pow++) {
      for (int i = 1; i <= m; i++) {
        long long divprime = div[i] / prime_pow;
        if (divprime < prime) {
          break;
        }
        T div_val = i * prime_pow <= m ? sumf[2 * m - i * prime_pow]
                                       : sumf[divprime - 1];
        _sumf[2 * m - i] +=
            f(prime, pow) * (div_val - sumfp[prime - 1]) + f(prime, pow + 1);
      }
      for (int i = m; i >= 1; i--) {
        long long divprime = i / prime_pow;
        if (divprime < prime) {
          break;
        }
        _sumf[i - 1] += f(prime, pow) * (sumf[divprime - 1] - sumf[prime - 1]) +
                        f(prime, pow + 1);
      }
    }
    long long prime_squared = (long long)prime * prime;
    int lim =
        prime_squared <= m ? prime_squared - 1 : 2 * m - n / prime_squared;
    for (int i = 2 * m - 1; i >= lim; i--) {
      sumf[i] = _sumf[i];
    }
  }
  for (int i = 0; i < 2 * m; i++) {
    sumf[i] += f(1, 0);
  }
  return sumf;
}
} // namespace maomao90
