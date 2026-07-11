#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
using namespace std;

#include "library/math/pow_mod.hpp"
using namespace maomao90;

template <typename T, typename P> T brute_pow_mod(T b, P p, T mod) {
  using U = __int128_t;
  T res = 1 % mod;
  b %= mod;
  if (b < 0) {
    b += mod;
  }
  while (p--) {
    res = (U)res * b % mod;
  }
  return res;
}

mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());

int main() {
  for (long long mod = 2; mod <= 20; mod++) {
    for (long long b = -20; b <= 20; b++) {
      for (unsigned int p = 0; p <= 10; p++) {
        assert((pow_mod<long long, unsigned int>(b, p, mod) ==
                brute_pow_mod<long long, unsigned int>(b, p, mod)));
      }
    }
  }

  assert((pow_mod<long long, unsigned int>(0, 5, 7) == 0));
  assert((pow_mod<long long, unsigned int>(-2, 3, 5) == 2));
  assert((pow_mod<long long, unsigned int>(-2, 4, 5) == 1));

  for (int tc = 0; tc < 10000; tc++) {
    long long mod = rng() % 999 + 2;
    long long b = (long long)(rng() % 2001) - 1000;
    unsigned int p = rng() % 30;
    assert((pow_mod<long long, unsigned int>(b, p, mod) ==
            brute_pow_mod<long long, unsigned int>(b, p, mod)));
  }

  int a, b;
  cin >> a >> b;
  cout << a + b << '\n';
  return 0;
}
