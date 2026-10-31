#include "baseline.hpp"
#include "candidates.hpp"
#include "library/math/poly.hpp"
#include <bits/stdc++.h>
using namespace std;
using cd = complex<double>;
using Clock = chrono::steady_clock;
volatile double sink;
// method: square, product, balanced, separate, half.
template <class F>
vector<double> conv(const vector<double> &a, const vector<double> &b,
                    string method, F fft) {
  if (method == "production")
    return maomao90::internal::poly::fft::convolution(a, b);
  int n = a.size(), m = b.size(), z = bit_ceil(unsigned(n + m - 1));
  vector<double> out(n + m - 1);
  if (method == "half") {
    int h = z / 2;
    vector<cd> x(h), y(h);
    for (int i = 0; i < n; ++i)
      (i & 1) ? x[i / 2].imag(a[i]) : x[i / 2].real(a[i]);
    for (int i = 0; i < m; ++i)
      (i & 1) ? y[i / 2].imag(b[i]) : y[i / 2].real(b[i]);
    fft(x);
    fft(y);
    static fft_experiment::Roots<double, 0> roots;
    roots.grow(z);
    vector<cd> v(h);
    for (int k = 0; k < h; ++k) {
      int j = -k & (h - 1);
      auto w = roots.w[h + k];
      auto ae = (x[k] + conj(x[j])) * 0.5,
           ao = (x[k] - conj(x[j])) * cd(0, -0.5);
      auto be = (y[k] + conj(y[j])) * 0.5,
           bo = (y[k] - conj(y[j])) * cd(0, -0.5);
      auto aw = ao * w, bw = bo * w;
      auto c = (ae + aw) * (be + bw), d = (ae - aw) * (be - bw);
      v[k] = ((c + d) + (c - d) * conj(w) * cd(0, 1)) * (0.5 / h);
    }
    reverse(v.begin() + 1, v.end());
    fft(v);
    for (int i = 0; i < n + m - 1; ++i)
      out[i] = (i & 1) ? v[i / 2].imag() : v[i / 2].real();
    return out;
  }
  vector<cd> x(z), y(z);
  if (method == "separate") {
    copy(a.begin(), a.end(), x.begin());
    copy(b.begin(), b.end(), y.begin());
    fft(x);
    fft(y);
    for (int k = 0; k < z; ++k)
      x[k] *= y[k] / double(z);
    reverse(x.begin() + 1, x.end());
    fft(x);
    for (int k = 0; k < n + m - 1; ++k)
      out[k] = x[k].real();
    return out;
  }
  double sa = 1, sb = 1;
  if (method == "balanced") {
    double ma = 0, mb = 0;
    for (double v : a)
      ma = max(ma, abs(v));
    for (double v : b)
      mb = max(mb, abs(v));
    if (ma == 0 || mb == 0)
      return out;
    sa = exp2(-ilogb(ma));
    sb = exp2(-ilogb(mb));
  }
  for (int i = 0; i < n; ++i)
    x[i].real(a[i] * sa);
  for (int i = 0; i < m; ++i)
    x[i].imag(b[i] * sb);
  fft(x);
  if (method == "square") {
    for (auto &v : x)
      v *= v;
    for (int k = 0; k < z; ++k)
      y[k] = (x[-k & (z - 1)] - conj(x[k])) / double(4 * z);
  } else {
    for (int k = 0; k < z; ++k) {
      auto u = x[-k & (z - 1)], v = conj(x[k]);
      y[k] = (u - v) * (u + v) / double(4 * z);
    }
  }
  fft(y);
  for (int k = 0; k < n + m - 1; ++k)
    out[k] = y[k].imag() / (sa * sb);
  return out;
}
int main(int argc, char **argv) {
  string kernel = argc > 1 ? argv[1] : "baseline";
  int maxlg = argc > 2 ? atoi(argv[2]) : 19;
  auto fft = [&](auto &a) {
    if (kernel == "baseline")
      fft_baseline::fft(a);
    else if (kernel == "production")
      maomao90::internal::poly::fft::fft(a);
    else if (kernel == "r2")
      fft_experiment::fft<2>(a);
    else if (kernel == "r4")
      fft_experiment::fft<4>(a);
    else if (kernel == "split")
      fft_experiment::fft<3>(a);
    else if (kernel == "r2-fma")
      fft_experiment::fft<2, true>(a);
    else if (kernel == "r4-fma")
      fft_experiment::fft<4, true>(a);
    else
      abort();
  };
  cout << setprecision(10);
  for (int lg : {7, 11, 15, maxlg})
    for (string pattern : {"random", "positive", "alternating", "unbalanced",
                           "sparse", "cancellation"}) {
      int n = 1 << lg, m = n - 17;
      vector<double> a(n), b(m);
      mt19937_64 rng(0x890123 + lg);
      for (auto &v : a)
        v = int(rng() % 2001) - 1000;
      for (auto &v : b)
        v = int(rng() % 2001) - 1000;
      if (pattern == "positive") {
        fill(a.begin(), a.end(), 999);
        fill(b.begin(), b.end(), 1000);
      }
      if (pattern == "alternating") {
        for (int i = 0; i < n; ++i)
          a[i] = (i & 1) ? -999 : 999;
        fill(b.begin(), b.end(), 1000);
      }
      if (pattern == "unbalanced") {
        for (auto &v : a)
          v *= double(1LL << 35);
        for (auto &v : b)
          v /= double(1LL << 35);
      }
      if (pattern == "sparse") {
        fill(b.begin(), b.end(), 0);
        b[17] = 1;
      }
      if (pattern == "cancellation") {
        for (int i = 0; i < n; ++i)
          a[i] = (i & 1) ? -1000000 : 1000000;
        fill(b.begin(), b.end(), 1);
      }
      vector<int> positions;
      for (int k = 0; k < min(64, n + m - 1); ++k)
        positions.push_back(rng() % (n + m - 1));
      for (int k : {0, 1, m - 1, n - 1, n, n + m - 3, n + m - 2})
        positions.push_back(k);
      vector<long double> expected;
      for (int k : positions) {
        long double s = 0;
        for (int i = max(0, k - m + 1); i <= min(k, n - 1); ++i)
          s += (long double)a[i] * b[k - i];
        expected.push_back(s);
      }
      for (string method :
           (kernel == "production"
                ? vector<string>{"production"}
                : vector<string>{"square", "product", "balanced", "separate",
                                 "half"})) {
        auto c = conv(a, b, method, fft);
        vector<double> times;
        for (int rep = 0; rep < 5; ++rep) {
          auto start = Clock::now();
          c = conv(a, b, method, fft);
          times.push_back(
              chrono::duration<double, milli>(Clock::now() - start).count());
          sink = c[n / 3];
        }
        sort(times.begin(), times.end());
        long double mx = 0, rm = 0;
        int bad = 0;
        for (int j = 0; j < (int)positions.size(); ++j) {
          auto e = abs((long double)c[positions[j]] - expected[j]);
          mx = max(mx, e);
          rm += e * e;
          bad += roundl(c[positions[j]]) != expected[j];
        }
        cout << kernel << ',' << method << ',' << lg << ',' << pattern << ','
             << times[2] << ',' << mx << ',' << sqrtl(rm / positions.size())
             << ',' << bad << '\n';
      }
    }
}
