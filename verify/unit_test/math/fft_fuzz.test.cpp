#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "library/math/poly.hpp"
#include <cassert>
#include <iostream>
#include <random>
using namespace std;
using namespace maomao90;
namespace impl = maomao90::internal::poly;
int main() {
  mt19937_64 rng(0x93a182);
  for (int trial = 0; trial < 1200; ++trial) {
    int n = 1 + rng() % 400, m = 1 + rng() % 400;
    if (trial % 50 == 0) {
      n = 61;
      m = 32769 + trial;
    }
    vector<long long> a(n), b(m);
    for (auto &x : a)
      x = int(rng() % 2001) - 1000;
    for (auto &x : b)
      x = int(rng() % 2001) - 1000;
    auto expected = impl::convolution_naive(a, b);
    assert(impl::fft::convolution(a, b) == expected);
    vector<double> da(a.begin(), a.end()), db(b.begin(), b.end());
    int exponent = int(rng() % 1201) - 600;
    for (auto &x : da)
      x = ldexp(x, exponent);
    for (auto &x : db)
      x = ldexp(x, -exponent);
    auto c = impl::fft::convolution(da, db);
    for (int i = 0; i < (int)c.size(); ++i)
      assert(abs(c[i] - expected[i]) < 1e-5);
  }
  for (int trial = 0; trial < 200; ++trial) {
    using mint = dynamic_modint<902>;
    int mods[] = {2, 4, 17, 32768, 1000000007, 2147483647};
    int mod = trial < 6 ? mods[trial] : 2 + rng() % 2147483646;
    mint::set_mod(mod);
    int n = 61 + rng() % 400, m = 61 + rng() % 400;
    vector<mint> a(n), b(m);
    for (auto &x : a)
      x = rng();
    for (auto &x : b)
      x = rng();
    assert(impl::fft::convolution_arb_mod(a, b) ==
           impl::convolution_naive(a, b));
  }
  int a, b;
  cin >> a >> b;
  cout << a + b << '\n';
}
