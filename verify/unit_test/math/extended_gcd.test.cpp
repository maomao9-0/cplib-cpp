#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
using namespace std;

#include "library/math/extended_gcd.hpp"
using namespace maomao90;

mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());

int main() {
  for (long long a = 0; a <= 100; a++) {
    for (long long b = 0; b <= 100; b++) {
      long long x, y;
      long long g = extended_gcd(a, b, x, y);
      assert(g == gcd(a, b));
      assert((__int128)a * x + (__int128)b * y == g);
    }
  }

  for (long long mod = 1; mod <= 200; mod++) {
    for (long long x = 0; x < mod; x++) {
      if (gcd(x, mod) != 1) {
        continue;
      }
      long long inv = inv_gcd(x, mod);
      assert(0 <= inv && inv < mod);
      assert((__int128)x * inv % mod == 1 % mod);
    }
  }

  for (int tc = 0; tc < 5000; tc++) {
    long long a = rng() % 1000000000000ll;
    long long b = rng() % 1000000000000ll;
    long long x, y;
    long long g = extended_gcd(a, b, x, y);
    assert(g == gcd(a, b));
    assert((__int128)a * x + (__int128)b * y == g);
  }

  for (int tc = 0; tc < 5000; tc++) {
    long long mod = rng() % 1000000 + 1;
    long long x;
    do {
      x = rng() % mod;
    } while (gcd(x, mod) != 1);
    long long inv = inv_gcd(x, mod);
    assert(0 <= inv && inv < mod);
    assert((__int128)x * inv % mod == 1 % mod);
  }

  int a, b;
  cin >> a >> b;
  cout << a + b << '\n';
  return 0;
}
