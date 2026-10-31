#include "library/math/poly.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>
using namespace std;
using namespace maomao90;
namespace impl = maomao90::internal::poly;
int main() {
  mt19937_64 rng(907890);
  size_t checked = 0;
  for (int n : {61, 127, 1025, 16385, 131073, 524289}) {
    int m = n / 2 + 31;
    for (int pattern = 0; pattern < 4; ++pattern) {
      vector<long long> a(n), b(m);
      for (auto &x : a)
        x = int(rng() % 200001) - 100000;
      for (auto &x : b)
        x = int(rng() % 2001) - 1000;
      if (pattern == 1) {
        fill(a.begin(), a.end(), 10000);
        fill(b.begin(), b.end(), 9999);
      } else if (pattern == 2) {
        for (int i = 0; i < n; ++i)
          a[i] = i & 1 ? -100000000 : 100000000;
        fill(b.begin(), b.end(), 1);
      } else if (pattern == 3) {
        fill(b.begin(), b.end(), 0);
        b[27] = -3;
      }
      auto expected = impl::ntt::convolution_ll(a, b);
      assert(impl::fft::convolution(a, b) == expected);
      vector<double> da(a.begin(), a.end()), db(b.begin(), b.end());
      for (auto &x : da)
        x *= exp2(35.0);
      for (auto &x : db)
        x *= exp2(-35.0);
      auto actual = impl::fft::convolution(da, db);
      for (int i = 0; i < (int)actual.size(); ++i)
        assert(abs((long double)actual[i] - expected[i]) < 0.25L);
      checked += 2 * expected.size();
    }
    cout << "real/integer " << n << " passed\n";
  }
  using mint = static_modint<2147483647>;
  for (int n : {61, 127, 1025, 16385, 131073, 524289}) {
    int m = n / 2 + 31;
    for (int pattern = 0; pattern < 4; ++pattern) {
      vector<mint> a(n), b(m);
      for (auto &x : a)
        x = rng();
      for (auto &x : b)
        x = rng();
      if (pattern == 1) {
        fill(a.begin(), a.end(), -1);
        fill(b.begin(), b.end(), -1);
      } else if (pattern == 2) {
        fill(a.begin(), a.end(), 1073741823);
        fill(b.begin(), b.end(), 1073741823);
      } else if (pattern == 3) {
        fill(b.begin(), b.end(), 0);
        b[27] = -1;
      }
      auto expected = impl::ntt::convolution_arb_mod(a, b);
      assert(impl::fft::convolution_arb_mod(a, b) == expected);
      checked += expected.size();
    }
    cout << "modular " << n << " passed\n";
  }
  cout << checked << " coefficients checked against exact NTT/CRT\n";
}
