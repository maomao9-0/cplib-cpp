#pragma once

#include <cassert>
#include <concepts>
#include <vector>

#include "library/internal/concepts.hpp"

namespace maomao90 {
using namespace std;
// sumg is a vector of size 2 * m where sumg[i] represents prefix sum of
// g from 2 to i + 1 for i < m, otherwise, sum from 2 to n / (2 * m - i)
// g is a function that returns the value of g(prime)
// returns prefix sum of g at prime indices with the same range as sumg
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
