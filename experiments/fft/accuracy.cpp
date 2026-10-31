// Independent 113-bit reference: direct twiddles and radix-2 butterflies.
#include "baseline.hpp"
#include "candidates.hpp"
#include "library/math/poly.hpp"
#include <bits/stdc++.h>
#include <boost/math/constants/constants.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
using namespace std;
using Q = boost::multiprecision::cpp_bin_float_quad;
using CQ = complex<Q>;
void reference(vector<CQ> &a) {
  int n = a.size();
  // Deliberately use an independently constructed reversal permutation.
  for (unsigned i = 0; i < unsigned(n); ++i) {
    unsigned j = 0, v = i;
    for (int bits = countr_zero(unsigned(n)); bits; --bits) {
      j = 2 * j + (v & 1);
      v >>= 1;
    }
    if (i < j)
      swap(a[i], a[j]);
  }
  Q pi = boost::math::constants::pi<Q>();
  for (int k = 1; k < n; k *= 2) {
    vector<CQ> w(k);
    for (int j = 0; j < k; ++j) {
      Q angle = pi * j / k;
      w[j] = CQ(cos(angle), sin(angle));
    }
    for (int i = 0; i < n; i += 2 * k)
      for (int j = 0; j < k; ++j) {
        auto x = a[i + j], y = a[i + j + k] * w[j];
        a[i + j] = x + y;
        a[i + j + k] = x - y;
      }
  }
}
template <class T, class F>
void check(string name, const vector<CQ> &input, const vector<CQ> &expected,
           F fft, int lg, string pattern) {
  vector<complex<T>> a;
  for (auto x : input)
    a.emplace_back(T(x.real()), T(x.imag()));
  fft(a);
  Q sum = 0, norm = 0, mx = 0;
  for (int i = 0; i < (int)a.size(); ++i) {
    CQ d = CQ(Q(a[i].real()), Q(a[i].imag())) - expected[i];
    Q e = std::norm(d);
    sum += e;
    norm += std::norm(expected[i]);
    mx = max(mx, sqrt(e));
  }
  cout << name << ',' << lg << ',' << pattern << ',' << sqrt(sum / norm) << ','
       << mx << '\n';
}
int main() {
  cout << setprecision(12);
  for (int lg : {5, 10, 14})
    for (string pattern :
         {"random", "constant", "alternating", "impulse", "tone"}) {
      int n = 1 << lg;
      mt19937_64 rng(906789);
      vector<CQ> a(n);
      Q pi = boost::math::constants::pi<Q>();
      for (int i = 0; i < n; ++i) {
        // Dyadic values are identical in every tested scalar type.
        double re = (int(rng() % 2049) - 1024) / 1024.0,
               im = (int(rng() % 2049) - 1024) / 1024.0;
        if (pattern == "constant") {
          re = 1;
          im = -1;
        }
        if (pattern == "alternating") {
          re = i & 1 ? -1 : 1;
          im = 0;
        }
        if (pattern == "impulse") {
          re = i == n / 3;
          im = 0;
        }
        if (pattern == "tone") {
          re = double(cos(2 * pi * ((i * 17) % n) / n));
          im = double(sin(2 * pi * ((i * 17) % n) / n));
        }
        a[i] = CQ(Q(re), Q(im));
      }
      auto expected = a;
      reference(expected);
      check<double>(
          "baseline", a, expected, [](auto &v) { fft_baseline::fft(v); }, lg,
          pattern);
      check<double>(
          "production", a, expected,
          [](auto &v) { maomao90::internal::poly::fft::fft(v); }, lg, pattern);
      check<double>(
          "r4-fma", a, expected,
          [](auto &v) { fft_experiment::fft<4, true>(v); }, lg, pattern);
      check<long double>(
          "production-long-double", a, expected,
          [](auto &v) { maomao90::internal::poly::fft::fft(v); }, lg, pattern);
    }
}
