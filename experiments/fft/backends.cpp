#include "baseline_convolution.hpp"
#include <bits/stdc++.h>
using namespace std;
using namespace maomao90;
using Clock = chrono::steady_clock;
volatile long long sink;
template <class T, class F, class G>
void bench(int lg, string type, F old, G updated) {
  int n = 1 << lg, m = n - 17;
  mt19937_64 rng(908901);
  vector<T> a(n), b(m);
  for (auto &x : a)
    x = T(rng() % 1000000000);
  for (auto &x : b)
    x = T(rng() % 1000000000);
  if constexpr (is_integral_v<T>) {
    for (auto &x : a)
      x %= 1000;
    for (auto &x : b)
      x %= 1000;
  }
  vector<double> ta, tb;
  auto expected = old(a, b), actual = updated(a, b);
  assert(expected == actual);
  for (int rep = 0; rep < 9; ++rep) {
    auto run = [&](auto fn, auto &times) {
      auto start = Clock::now();
      auto c = fn(a, b);
      times.push_back(
          chrono::duration<double, milli>(Clock::now() - start).count());
      if constexpr (is_integral_v<T>)
        sink = c[n / 3];
      else
        sink = c[n / 3].val();
    };
    // Alternate measurement order to reduce systematic warmup bias.
    if (rep & 1) {
      run(old, ta);
      run(updated, tb);
    } else {
      run(updated, tb);
      run(old, ta);
    }
  }
  sort(ta.begin(), ta.end());
  sort(tb.begin(), tb.end());
  cout << type << ',' << lg << ',' << ta[4] << ',' << tb[4] << ','
       << ta[4] / tb[4] << '\n';
}
int main() {
  cout << setprecision(10);
  for (int lg : {7, 11, 15, 19}) {
    bench<long long>(
        lg, "integer",
        [](const auto &a, const auto &b) {
          return fft_baseline::convolution(a, b);
        },
        [](const auto &a, const auto &b) {
          return internal::poly::fft::convolution(a, b);
        });
    bench<static_modint<1000000007>>(
        lg, "modular",
        [](const auto &a, const auto &b) {
          return fft_baseline::convolution_arb_mod(a, b);
        },
        [](const auto &a, const auto &b) {
          return internal::poly::fft::convolution_arb_mod(a, b);
        });
  }
}
