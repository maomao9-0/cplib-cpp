// Snapshot of the FFT before this experiment.
#pragma once
#include <bits/stdc++.h>
namespace fft_baseline {
using namespace std;
template <typename T>
  requires is_floating_point_v<T>
inline void fft(vector<complex<T>> &a) {
  int n = a.size(), L = 31 - __builtin_clz(n);
  static vector<complex<long double>> R(2, 1);
  static vector<complex<T>> rt(2, 1); // (^ 10% faster if double)
  for (static int k = 2; k < n; k *= 2) {
    R.resize(n);
    rt.resize(n);
    auto x = polar(1.0L, acos(-1.0L) / k);
    for (int i = k; i < 2 * k; i++) {
      rt[i] = R[i] = i & 1 ? R[i / 2] * x : R[i / 2];
    }
  }
  vector<int> rev(n);
  for (int i = 0; i < n; i++) {
    rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
  }
  for (int i = 0; i < n; i++) {
    if (i < rev[i]) {
      swap(a[i], a[rev[i]]);
    }
  }
  for (int k = 1; k < n; k *= 2) {
    for (int i = 0; i < n; i += 2 * k) {
      for (int j = 0; j < k; j++) {
        // complex<T> z = rt[j+k] * a[i+j+k]; // (25% faster if hand-rolled)
        auto x = (T *)&rt[j + k], y = (T *)&a[i + j + k];
        complex<T> z(x[0] * y[0] - x[1] * y[1], x[0] * y[1] + x[1] * y[0]);
        a[i + j + k] = a[i + j] - z;
        a[i + j] += z;
      }
    }
  }
}
}
