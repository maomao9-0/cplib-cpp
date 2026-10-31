// Convolution snapshot from 8c8de73; unchanged algorithms for comparison.
#pragma once
#include "library/math/poly.hpp"
#include "baseline.hpp"
namespace fft_baseline {
using namespace maomao90;
using maomao90::internal::poly::convolution_naive;
template <typename T> inline long long round_integral(T x) {
  return x < 0 ? (long long)(x - T(0.5)) : (long long)(x + T(0.5));
}
template <typename T>
  requires is_arithmetic_v<T>
inline vector<T> convolution(const vector<T> &ta, const vector<T> &tb) {
  int n = ta.size(), m = tb.size();
  if (!n || !m) {
    return {};
  }
  if (min(n, m) <= 60) {
    return convolution_naive(ta, tb);
  }
  vector<double> a(n), b(m);
  for (int i = 0; i < n; i++) {
    a[i] = ta[i];
  }
  for (int i = 0; i < m; i++) {
    b[i] = tb[i];
  }
  int z = bit_ceil((unsigned int)(n + m - 1));
  vector<complex<double>> in(z), out(z);
  copy(a.begin(), a.end(), in.begin());
  for (int i = 0; i < m; i++) {
    in[i].imag(b[i]);
  }
  fft(in);
  for (complex<double> &x : in) {
    x *= x;
  }
  for (int i = 0; i < z; i++) {
    out[i] = in[-i & (z - 1)] - conj(in[i]);
  }
  fft(out);
  vector<T> res(n + m - 1);
  for (int i = 0; i < n + m - 1; i++) {
    if constexpr (integral<T>) {
      res[i] = T(round_integral(imag(out[i]) / (4 * z)));
    } else {
      res[i] = imag(out[i]) / (4 * z);
    }
  }
  return res;
}
template <maomao90::internal::concepts::broadly_unsigned_integral T>
inline vector<T> convolution_sqrt(const vector<T> &a, const vector<T> &b) {
  int n = a.size(), m = b.size();
  if (!n || !m) {
    return {};
  }
  if (min(n, m) <= 60) {
    return convolution_naive(a, b);
  }
  int z = bit_ceil((unsigned int)(n + m - 1)), cut = 1 << 15;
  vector<complex<double>> L(z), R(z), outs(z), outl(z);
  for (int i = 0; i < n; i++) {
    L[i] = complex<double>(a[i] >> 15, a[i] & ((1 << 15) - 1));
  }
  for (int i = 0; i < m; i++) {
    R[i] = complex<double>(b[i] >> 15, b[i] & ((1 << 15) - 1));
  }
  fft(L), fft(R);
  for (int i = 0; i < z; i++) {
    int j = -i & (z - 1);
    outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * z);
    outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * z) / 1i;
  }
  fft(outl), fft(outs);
  vector<T> res(n + m - 1);
  for (int i = 0; i < n + m - 1; i++) {
    T av = (T)(real(outl[i]) + .5), cv = (T)(imag(outs[i]) + .5);
    T bv = (T)(imag(outl[i]) + .5) + (T)(real(outs[i]) + .5);
    res[i] = (av * cut + bv) * cut + cv;
  }
  return res;
}
template <ModInt mint>
vector<mint> convolution_arb_mod(const vector<mint> &a, const vector<mint> &b) {
  int n = a.size(), m = b.size();
  if (!n || !m) {
    return {};
  }
  if (min(n, m) <= 60) {
    return convolution_naive(a, b);
  }
  int z = bit_ceil((unsigned int)(n + m - 1)), cut = int(sqrt(mint::imod()));
  vector<complex<double>> L(z), R(z), outs(z), outl(z);
  for (int i = 0; i < n; i++) {
    L[i] = complex<double>(a[i].val() / cut, a[i].val() % cut);
  }
  for (int i = 0; i < m; i++) {
    R[i] = complex<double>(b[i].val() / cut, b[i].val() % cut);
  }
  fft(L), fft(R);
  for (int i = 0; i < z; i++) {
    int j = -i & (z - 1);
    outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * z);
    outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * z) / 1i;
  }
  fft(outl), fft(outs);
  vector<mint> res(n + m - 1);
  for (int i = 0; i < n + m - 1; i++) {
    long long av = (long long)(real(outl[i]) + .5),
              cv = (long long)(imag(outs[i]) + .5);
    long long bv =
        (long long)(imag(outl[i]) + .5) + (long long)(real(outs[i]) + .5);
    res[i] = (av % mint::imod() * cut + bv) % mint::imod() * cut + cv;
  }
  return res;
}
template <typename T>
  requires is_floating_point_v<T>
vector<complex<T>> convolution_complex(vector<complex<T>> a,
                                       vector<complex<T>> b) {
  int n = a.size(), m = b.size();
  if (!n || !m) {
    return {};
  }
  if (min(n, m) <= 60) {
    return convolution_naive(a, b);
  }
  int z = bit_ceil((unsigned int)(n + m - 1));
  a.resize(z, 0);
  b.resize(z, 0);
  fft(a);
  fft(b);
  for (int i = 0; i < z; i++) {
    a[i] *= b[i] / (T)z;
  }
  reverse(a.begin() + 1, a.end());
  fft(a);
  a.resize(n + m - 1);
  return a;
}
}
