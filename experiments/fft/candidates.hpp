#pragma once
#include <algorithm>
#include <bit>
#include <cmath>
#include <complex>
#include <numbers>
#include <vector>

namespace fft_experiment {
using namespace std;
// Roots use the existing binary tree (mode 0), direct long-double trig (1),
// or direct double trig (2). Store w^3 separately for radix-4/split-radix.
template <class T, int mode> struct Roots {
  vector<complex<T>> w{1, 1}, w3{1, 1};
  vector<complex<long double>> wide{1, 1};
  void grow(int n) {
    int old = w.size();
    if (old >= n)
      return;
    w.resize(n);
    w3.resize(n);
    wide.resize(n);
    for (int k = old; k < n; k *= 2) {
      auto step = polar(1.0L, numbers::pi_v<long double> / k);
      for (int j = 0; j < k; ++j) {
        if constexpr (mode == 0)
          wide[k + j] = (j & 1) ? wide[(k + j) / 2] * step : wide[(k + j) / 2];
        else if constexpr (mode == 1)
          wide[k + j] = polar(1.0L, numbers::pi_v<long double> * j / k);
        else
          wide[k + j] = polar(1.0, numbers::pi * j / k);
        // Exact quadrantal roots avoid noise at +/-i.
        if (j == 0)
          wide[k + j] = {1, 0};
        if (j == k / 2)
          wide[k + j] = {0, 1};
        w[k + j] = complex<T>(wide[k + j]);
        w3[k + j] = complex<T>(wide[k + j] * wide[k + j] * wide[k + j]);
      }
    }
  }
};
template <class T, bool fused>
inline complex<T> mul(complex<T> a, complex<T> b) {
  if constexpr (fused)
    return {fma(a.real(), b.real(), -a.imag() * b.imag()),
            fma(a.real(), b.imag(), a.imag() * b.real())};
  else
    return {a.real() * b.real() - a.imag() * b.imag(),
            a.real() * b.imag() + a.imag() * b.real()};
}
template <class T> void permute(vector<complex<T>> &a) {
  int n = a.size();
  for (int i = 1, j = 0; i < n; ++i) {
    int bit = n >> 1;
    for (; j & bit; bit >>= 1)
      j ^= bit;
    j ^= bit;
    if (i < j)
      swap(a[i], a[j]);
  }
}
template <class T, bool fused, int mode>
void split(complex<T> *a, int n, Roots<T, mode> &r) {
  if (n == 1)
    return;
  if (n == 2) {
    auto x = a[0], y = a[1];
    a[0] = x + y;
    a[1] = x - y;
    return;
  }
  int k = n / 4;
  split<T, fused>(a, n / 2, r);
  split<T, fused>(a + n / 2, k, r);
  split<T, fused>(a + 3 * k, k, r);
  for (int j = 0; j < k; ++j) {
    auto x = a[j], y = a[j + k];
    auto z = mul<T, fused>(a[j + 2 * k], r.w[n / 2 + j]);
    auto t = mul<T, fused>(a[j + 3 * k], r.w3[n / 2 + j]);
    auto s = z + t, d = z - t;
    d = {-d.imag(), d.real()};
    a[j] = x + s;
    a[j + 2 * k] = x - s;
    a[j + k] = y + d;
    a[j + 3 * k] = y - d;
  }
}
// radix 2, 4, or split (3).
template <int radix, bool fused = false, int mode = 0, class T>
void fft(vector<complex<T>> &a) {
  int n = a.size();
  if (n <= 1)
    return;
  static Roots<T, mode> roots;
  roots.grow(n);
  permute(a);
  if constexpr (radix == 3) {
    split<T, fused>(a.data(), n, roots);
    return;
  }
  int k = 1;
  if constexpr (radix == 4) {
    if (countr_zero((unsigned)n) & 1) {
      for (int i = 0; i < n; i += 2) {
        auto x = a[i], y = a[i + 1];
        a[i] = x + y;
        a[i + 1] = x - y;
      }
      k = 2;
    }
    for (; k < n; k *= 4)
      for (int i = 0; i < n; i += 4 * k)
        for (int j = 0; j < k; ++j) {
          auto x = a[i + j];
          auto y = mul<T, fused>(a[i + j + k], roots.w[k + j]);
          auto z = mul<T, fused>(a[i + j + 2 * k], roots.w[2 * k + j]);
          auto t = mul<T, fused>(a[i + j + 3 * k], roots.w3[2 * k + j]);
          auto p = x + y, q = x - y, s = z + t, d = z - t;
          d = {-d.imag(), d.real()};
          a[i + j] = p + s;
          a[i + j + 2 * k] = p - s;
          a[i + j + k] = q + d;
          a[i + j + 3 * k] = q - d;
        }
  } else {
    for (; k < n; k *= 2)
      for (int i = 0; i < n; i += 2 * k)
        for (int j = 0; j < k; ++j) {
          auto x = a[i + j], y = mul<T, fused>(a[i + j + k], roots.w[k + j]);
          a[i + j] = x + y;
          a[i + j + k] = x - y;
        }
  }
}
} // namespace fft_experiment
