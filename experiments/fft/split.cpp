#include "baseline.hpp"
#include "library/math/poly.hpp"
#include <bits/stdc++.h>
using namespace std;
using cd = complex<double>;
using Clock = chrono::steady_clock;
volatile double sink;
struct Result {
  double error;
  int bad;
};
Result check(int n, int mod, string pattern, string mode, string kernel) {
  int m = n - 17, z = bit_ceil(unsigned(n + m - 1)), cut = int(sqrt(mod));
  mt19937_64 rng(0x904321 + n);
  vector<long long> a(n), b(m), ah(n), al(n), bh(m), bl(m);
  for (auto &v : a)
    v = rng() % mod;
  for (auto &v : b)
    v = rng() % mod;
  if (pattern == "max") {
    fill(a.begin(), a.end(), mod - 1);
    fill(b.begin(), b.end(), mod - 1);
  }
  if (pattern == "digits") {
    fill(a.begin(), a.end(), (mod / cut - 1) * cut + cut / 2 - 1);
    b.assign(m, a[0]);
  }
  vector<cd> L(z), R(z), outs(z), outl(z);
  auto pack = [&](auto &v, auto &hi, auto &lo, auto &out) {
    for (int i = 0; i < (int)v.size(); ++i) {
      long long x = v[i];
      if (mode != "original" && x > mod / 2)
        x -= mod;
      long long q = x / cut;
      if (mode == "centered-digits")
        q = (x + (x < 0 ? -cut / 2 : cut / 2)) / cut;
      hi[i] = q;
      lo[i] = x - q * cut;
      out[i] = {double(hi[i]), double(lo[i])};
    }
  };
  pack(a, ah, al, L);
  pack(b, bh, bl, R);
  auto fft = [&](auto &v) {
    if (kernel == "baseline")
      fft_baseline::fft(v);
    else
      maomao90::internal::poly::fft::fft(v);
  };
  fft(L);
  fft(R);
  for (int i = 0; i < z; ++i) {
    int j = -i & (z - 1);
    outl[j] = (L[i] + conj(L[j])) * R[i] / double(2 * z);
    outs[j] = (L[i] - conj(L[j])) * R[i] / double(2 * z) / cd(0, 1);
  }
  fft(outl);
  fft(outs);
  double error = 0;
  int bad = 0;
  vector<int> pos;
  for (int i = 0; i < 64; ++i)
    pos.push_back(rng() % (n + m - 1));
  for (int k : {0, 1, m - 1, n - 1, n, n + m - 3, n + m - 2})
    pos.push_back(k);
  for (int k : pos) {
    array<long long, 4> exact{};
    for (int i = max(0, k - m + 1); i <= min(k, n - 1); ++i) {
      exact[0] += ah[i] * bh[k - i];
      exact[1] += ah[i] * bl[k - i];
      exact[2] += al[i] * bh[k - i];
      exact[3] += al[i] * bl[k - i];
    }
    array<double, 4> values{outl[k].real(), outl[k].imag(), outs[k].real(),
                            outs[k].imag()};
    for (int i = 0; i < 4; ++i) {
      error = max(error, abs(values[i] - double(exact[i])));
      bad += llround(values[i]) != exact[i];
    }
  }
  sink = outl[n / 3].real();
  return {error, bad};
}
int main(int argc, char **argv) {
  string kernel = argc > 1 ? argv[1] : "production";
  cout << setprecision(10);
  for (int lg : {10, 16, 19, 20})
    for (int mod : {998244353, 1000000007, 2147483647})
      for (string pattern : {"random", "max", "digits"})
        for (string mode : {"original", "centered", "centered-digits"}) {
          auto start = Clock::now();
          auto r = check(1 << lg, mod, pattern, mode, kernel);
          cout << kernel << ',' << lg << ',' << mod << ',' << pattern << ','
               << mode << ',' << r.error << ',' << r.bad << ','
               << chrono::duration<double, milli>(Clock::now() - start).count()
               << '\n';
        }
}
