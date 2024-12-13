#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <complex>
#include <type_traits>
#include <vector>

#include "library/internal/concepts.hpp"
#include "library/internal/math.hpp"
#include "library/math/extended_gcd.hpp"
#include "library/math/modint.hpp"

namespace maomao90 {
using namespace std;
namespace internal::poly {
template <class T>
vector<T> convolution_naive(const vector<T> &a, const vector<T> &b) {
  int n = a.size(), m = b.size();
  vector<T> ans(n + m - 1);
  if (n < m) {
    for (int j = 0; j < m; j++) {
      for (int i = 0; i < n; i++) {
        ans[i + j] += a[i] * b[j];
      }
    }
  } else {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        ans[i + j] += a[i] * b[j];
      }
    }
  }
  return ans;
}
namespace ntt {
template <StaticModInt mint,
          int g = internal::math::primitive_root<mint::imod()>>
struct ntt_info {
  static constexpr int rank2 = __builtin_ctz(mint::umod() - 1);
  array<mint, rank2 + 1> root;  // root[i]^(2^i) == 1
  array<mint, rank2 + 1> iroot; // root[i] * iroot[i] == 1

  array<mint, max(0, rank2 - 2 + 1)> rate2;
  array<mint, max(0, rank2 - 2 + 1)> irate2;

  array<mint, max(0, rank2 - 3 + 1)> rate3;
  array<mint, max(0, rank2 - 3 + 1)> irate3;

  ntt_info() {
    root[rank2] = mint(g).pow((mint::imod() - 1) >> rank2);
    iroot[rank2] = root[rank2].inv();
    for (int i = rank2 - 1; i >= 0; i--) {
      root[i] = root[i + 1] * root[i + 1];
      iroot[i] = iroot[i + 1] * iroot[i + 1];
    }

    {
      mint prod = 1, iprod = 1;
      for (int i = 0; i <= rank2 - 2; i++) {
        rate2[i] = root[i + 2] * prod;
        irate2[i] = iroot[i + 2] * iprod;
        prod *= iroot[i + 2];
        iprod *= root[i + 2];
      }
    }
    {
      mint prod = 1, iprod = 1;
      for (int i = 0; i <= rank2 - 3; i++) {
        rate3[i] = root[i + 3] * prod;
        irate3[i] = iroot[i + 3] * iprod;
        prod *= iroot[i + 3];
        iprod *= root[i + 3];
      }
    }
  }
};

template <StaticModInt mint> void butterfly(vector<mint> &a) {
  int n = a.size();
  int h = __builtin_ctz((unsigned int)n);

  static const ntt_info<mint> info;

  int len = 0; // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed
  while (len < h) {
    if (h - len == 1) {
      int p = 1 << (h - len - 1);
      mint rot = 1;
      for (int s = 0; s < (1 << len); s++) {
        int offset = s << (h - len);
        for (int i = 0; i < p; i++) {
          auto l = a[i + offset];
          auto r = a[i + offset + p] * rot;
          a[i + offset] = l + r;
          a[i + offset + p] = l - r;
        }
        if (s + 1 != (1 << len)) {
          rot *= info.rate2[countr_zero(~(unsigned int)(s))];
        }
      }
      len++;
    } else {
      // 4-base
      int p = 1 << (h - len - 2);
      mint rot = 1, imag = info.root[2];
      for (int s = 0; s < (1 << len); s++) {
        mint rot2 = rot * rot;
        mint rot3 = rot2 * rot;
        int offset = s << (h - len);
        for (int i = 0; i < p; i++) {
          auto mod2 = 1ull * mint::imod() * mint::imod();
          auto a0 = 1ull * a[i + offset].val();
          auto a1 = 1ull * a[i + offset + p].val() * rot.val();
          auto a2 = 1ull * a[i + offset + 2 * p].val() * rot2.val();
          auto a3 = 1ull * a[i + offset + 3 * p].val() * rot3.val();
          auto a1na3imag = 1ull * mint(a1 + mod2 - a3).val() * imag.val();
          auto na2 = mod2 - a2;
          a[i + offset] = a0 + a2 + a1 + a3;
          a[i + offset + 1 * p] = a0 + a2 + (2 * mod2 - (a1 + a3));
          a[i + offset + 2 * p] = a0 + na2 + a1na3imag;
          a[i + offset + 3 * p] = a0 + na2 + (mod2 - a1na3imag);
        }
        if (s + 1 != (1 << len)) {
          rot *= info.rate3[countr_zero(~(unsigned int)(s))];
        }
      }
      len += 2;
    }
  }
}

template <StaticModInt mint> void butterfly_inv(vector<mint> &a) {
  int n = a.size();
  int h = __builtin_ctz((unsigned int)n);

  static const ntt_info<mint> info;

  int len = h; // a[i, i+(n>>len), i+2*(n>>len), ..] is transformed
  while (len) {
    if (len == 1) {
      int p = 1 << (h - len);
      mint irot = 1;
      for (int s = 0; s < (1 << (len - 1)); s++) {
        int offset = s << (h - len + 1);
        for (int i = 0; i < p; i++) {
          auto l = a[i + offset];
          auto r = a[i + offset + p];
          a[i + offset] = l + r;
          a[i + offset + p] =
              (unsigned long long)(mint::imod() + l.val() - r.val()) *
              irot.val();
          ;
        }
        if (s + 1 != (1 << (len - 1))) {
          irot *= info.irate2[countr_zero(~(unsigned int)(s))];
        }
      }
      len--;
    } else {
      // 4-base
      int p = 1 << (h - len);
      mint irot = 1, iimag = info.iroot[2];
      for (int s = 0; s < (1 << (len - 2)); s++) {
        mint irot2 = irot * irot;
        mint irot3 = irot2 * irot;
        int offset = s << (h - len + 2);
        for (int i = 0; i < p; i++) {
          auto a0 = 1ull * a[i + offset + 0 * p].val();
          auto a1 = 1ull * a[i + offset + 1 * p].val();
          auto a2 = 1ull * a[i + offset + 2 * p].val();
          auto a3 = 1ull * a[i + offset + 3 * p].val();

          auto a2na3iimag =
              1ull * mint((mint::imod() + a2 - a3) * iimag.val()).val();

          a[i + offset] = a0 + a1 + a2 + a3;
          a[i + offset + 1 * p] =
              (a0 + (mint::imod() - a1) + a2na3iimag) * irot.val();
          a[i + offset + 2 * p] =
              (a0 + a1 + (mint::imod() - a2) + (mint::imod() - a3)) *
              irot2.val();
          a[i + offset + 3 * p] =
              (a0 + (mint::imod() - a1) + (mint::imod() - a2na3iimag)) *
              irot3.val();
        }
        if (s + 1 != (1 << (len - 2))) {
          irot *= info.irate3[countr_zero(~(unsigned int)(s))];
        }
      }
      len -= 2;
    }
  }
}
template <StaticModInt mint>
vector<mint> convolution_ntt(vector<mint> a, vector<mint> b) {
  int n = a.size(), m = b.size();
  int z = bit_ceil((unsigned int)(n + m - 1));
  a.resize(z);
  butterfly(a);
  b.resize(z);
  butterfly(b);
  for (int i = 0; i < z; i++) {
    a[i] *= b[i];
  }
  butterfly_inv(a);
  a.resize(n + m - 1);
  mint iz = mint(z).inv();
  for (int i = 0; i < n + m - 1; i++)
    a[i] *= iz;
  return a;
}
template <StaticModInt mint>
vector<mint> convolution(const vector<mint> &a, const vector<mint> &b) {
  int n = a.size(), m = b.size();
  if (!n || !m) {
    return {};
  }

  int z = bit_ceil((unsigned int)(n + m - 1));
  assert((mint::imod() - 1) % z == 0);

  if (min(n, m) <= 60) {
    return convolution_naive(a, b);
  }
  return convolution_ntt(a, b);
}
template <int mod, class T>
  requires is_integral_v<T>
vector<T> convolution(const vector<T> &a, const vector<T> &b) {
  int n = a.size(), m = b.size();
  if (!n || !m)
    return {};

  using mint = static_modint<mod>;

  int z = bit_ceil((unsigned int)(n + m - 1));
  assert((mint::imod() - 1) % z == 0);

  vector<mint> a2(n), b2(m);
  for (int i = 0; i < n; i++) {
    a2[i] = mint(a[i]);
  }
  for (int i = 0; i < m; i++) {
    b2[i] = mint(b[i]);
  }
  auto c2 = convolution(std::move(a2), std::move(b2));
  vector<T> c(n + m - 1);
  for (int i = 0; i < n + m - 1; i++) {
    c[i] = c2[i].val();
  }
  return c;
}
inline vector<long long> convolution_ll(const vector<long long> &a,
                                        const vector<long long> &b) {
  int n = a.size(), m = b.size();
  if (!n || !m) {
    return {};
  }

  if (min(n, m) <= 60) {
    return convolution_naive(a, b);
  }

  static constexpr unsigned long long MOD1 = 754974721; // 2^24
  static constexpr unsigned long long MOD2 = 167772161; // 2^25
  static constexpr unsigned long long MOD3 = 469762049; // 2^26
  static constexpr unsigned long long M2M3 = MOD2 * MOD3;
  static constexpr unsigned long long M1M3 = MOD1 * MOD3;
  static constexpr unsigned long long M1M2 = MOD1 * MOD2;
  static constexpr unsigned long long M1M2M3 = MOD1 * MOD2 * MOD3;

  static constexpr unsigned long long i1 =
      inv_gcd<long long>(MOD2 * MOD3, MOD1);
  static constexpr unsigned long long i2 =
      inv_gcd<long long>(MOD1 * MOD3, MOD2);
  static constexpr unsigned long long i3 =
      inv_gcd<long long>(MOD1 * MOD2, MOD3);

  static constexpr int MAX_AB_BIT = 24;
  static_assert(MOD1 % (1ull << MAX_AB_BIT) == 1,
                "MOD1 isn't enough to support an array length of 2^24.");
  static_assert(MOD2 % (1ull << MAX_AB_BIT) == 1,
                "MOD2 isn't enough to support an array length of 2^24.");
  static_assert(MOD3 % (1ull << MAX_AB_BIT) == 1,
                "MOD3 isn't enough to support an array length of 2^24.");
  assert(n + m - 1 <= (1 << MAX_AB_BIT));

  vector<long long> c1 = convolution<MOD1>(a, b);
  vector<long long> c2 = convolution<MOD2>(a, b);
  vector<long long> c3 = convolution<MOD3>(a, b);

  vector<long long> c(n + m - 1);
  for (int i = 0; i < n + m - 1; i++) {
    unsigned long long x = 0;
    x += (c1[i] * i1) % MOD1 * M2M3;
    x += (c2[i] * i2) % MOD2 * M1M3;
    x += (c3[i] * i3) % MOD3 * M1M2;
    // B = 2^63, -B <= x, r(real value) < B
    // (x, x - M, x - 2M, or x - 3M) = r (mod 2B)
    // r = c1[i] (mod MOD1)
    // focus on MOD1
    // r = x, x - M', x - 2M', x - 3M' (M' = M % 2^64) (mod 2B)
    // r = x,
    //     x - M' + (0 or 2B),
    //     x - 2M' + (0, 2B or 4B),
    //     x - 3M' + (0, 2B, 4B or 6B) (without mod!)
    // (r - x) = 0, (0)
    //           - M' + (0 or 2B), (1)
    //           -2M' + (0 or 2B or 4B), (2)
    //           -3M' + (0 or 2B or 4B or 6B) (3) (mod MOD1)
    // we checked that
    //   ((1) mod MOD1) mod 5 = 2
    //   ((2) mod MOD1) mod 5 = 3
    //   ((3) mod MOD1) mod 5 = 4
    long long _x = (long long)x % (long long)MOD1;
    if (_x < 0) {
      _x += MOD1;
    }
    long long diff = c1[i] - _x;
    if (diff < 0)
      diff += MOD1;
    static constexpr unsigned long long offset[5] = {0, 0, M1M2M3, 2 * M1M2M3,
                                                     3 * M1M2M3};
    x -= offset[diff % 5];
    c[i] = x;
  }

  return c;
}
template <ModInt mint>
inline vector<mint> convolution_arb_mod(const vector<mint> &a,
                                        const vector<mint> &b) {
  int n = a.size(), m = b.size();
  if (!n || !m) {
    return {};
  }

  if (min(n, m) <= 60) {
    return convolution_naive(a, b);
  }

  static constexpr long long MOD1 = 167772161; // 2^25
  static constexpr long long MOD2 = 469762049; // 2^26
  static constexpr long long MOD3 = 754974721; // 2^24

  static constexpr long long INV12 = inv_gcd(MOD1, MOD2);
  static constexpr long long INV13 = inv_gcd(MOD1, MOD3);
  static constexpr long long INV23 = inv_gcd(MOD2, MOD3);
  static constexpr long long INV13INV23 = INV13 * INV23 % MOD3;
  static constexpr long long W1 = MOD1 % mint::imod();
  static constexpr long long W2 = W1 * MOD2 % mint::imod();

  static constexpr int MAX_AB_BIT = 24;
  static_assert(MOD1 % (1ull << MAX_AB_BIT) == 1,
                "MOD1 isn't enough to support an array length of 2^24.");
  static_assert(MOD2 % (1ull << MAX_AB_BIT) == 1,
                "MOD2 isn't enough to support an array length of 2^24.");
  static_assert(MOD3 % (1ull << MAX_AB_BIT) == 1,
                "MOD3 isn't enough to support an array length of 2^24.");
  assert(n + m - 1 <= (1 << MAX_AB_BIT));

  vector<long long> _a(n), _b(m);
  for (int i = 0; i < n; i++) {
    _a[i] = a[i].val();
  }
  for (int i = 0; i < m; i++) {
    _b[i] = b[i].val();
  }

  vector<long long> c1 = convolution<MOD1>(_a, _b);
  vector<long long> c2 = convolution<MOD2>(_a, _b);
  vector<long long> c3 = convolution<MOD3>(_a, _b);

  vector<mint> c(n + m - 1);
  for (int i = 0; i < n + m - 1; i++) {
    long long x = (c2[i] + MOD2 - c1[i]) * INV12 % MOD2;
    long long y =
        ((c3[i] + MOD3 - c1[i]) * INV13INV23 + (MOD3 - x) * INV23) % MOD3;
    c[i] = c1[i] + x * W1 + y * W2;
  }

  return c;
}
} // namespace ntt
namespace fft {
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
      res[i] = imag(out[i]) / (4 * z) + 0.5;
    } else {
      res[i] = imag(out[i]) / (4 * z);
    }
  }
  return res;
}
template <concepts::broadly_integral T>
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
vector<complex<T>> convolution_complex(const vector<complex<T>> &a,
                                       const vector<complex<T>> &b) {
  int n = a.size(), m = b.size();
  if (!a || !b) {
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
} // namespace fft
} // namespace internal::poly
enum PolySetting {
  ntt,
  fft,      // faster than fft_sqrt but less precision
  fft_sqrt, // uses sqrt to increase precision
  fft_complex,
};
template <typename T, PolySetting poly_setting>
struct is_valid_setting : false_type {};
// ntt allowes ModInt or integral types
template <ModInt T> struct is_valid_setting<T, PolySetting::ntt> : true_type {};
template <integral T>
  requires internal::type_traits::is_64bit_or_less_v<T>
struct is_valid_setting<T, PolySetting::ntt> : true_type {};
// fft_sqrt allow broadly_integral types or ModInt
template <internal::concepts::broadly_integral T>
struct is_valid_setting<T, PolySetting::fft_sqrt> : true_type {};
template <ModInt T>
struct is_valid_setting<T, PolySetting::fft_sqrt> : true_type {};
// fft allow arithmetic types
template <typename T>
  requires is_arithmetic_v<T>
struct is_valid_setting<T, PolySetting::fft> : true_type {};
// fft_complex allows complex numbers
template <typename T>
  requires is_floating_point_v<T>
struct is_valid_setting<complex<T>, PolySetting::fft_complex> : true_type {};

template <typename T, PolySetting poly_setting>
constexpr bool is_valid_setting_v = is_valid_setting<T, poly_setting>::value;

template <typename T, PolySetting poly_setting,
          enable_if_t<is_valid_setting_v<T, poly_setting>, nullptr_t> = nullptr>
struct Poly {
  constexpr Poly() : _v(1, 0) {}
  constexpr Poly(int n) : _v(n) {}
  constexpr Poly(vector<T> v) : _v(v) {}

  constexpr int degree() const { return _v.size() - 1; }
  constexpr T operator[](int i) const { return _v[i]; }
  constexpr T &operator[](int i) { return _v[i]; }

  constexpr Poly &operator*=(const Poly &o) {
    if constexpr (poly_setting == PolySetting::ntt) {
      if constexpr (ModInt<T>) {
        int z = bit_ceil((unsigned int)(degree() + o.degree() + 1));
        if (StaticModInt<T> && T::is_prime_mod && (T::imod() - 1) % z == 0) {
          _v = internal::poly::ntt::convolution(_v, o._v);
        } else {
          _v = internal::poly::ntt::convolution_arb_mod(_v, o._v);
        }
      } else { // integral
        vector<long long> a(degree() + 1), b(o.degree() + 1);
        for (int i = 0; i <= degree(); i++) {
          a[i] = _v[i];
        }
        for (int i = 0; i <= o.degree(); i++) {
          b[i] = o._v[i];
        }
        vector<long long> res = internal::poly::ntt::convolution_ll(a, b);
        _v.resize(res.size());
        for (size_t i = 0; i < res.size(); i++) {
          _v[i] = res[i];
        }
      }
    } else if constexpr (poly_setting == PolySetting::fft_sqrt) {
      if constexpr (ModInt<T>) {
        _v = internal::poly::fft::convolution_arb_mod(_v, o._v);
      } else { // integral
        _v = internal::poly::fft::convolution_sqrt(_v, o._v);
      }
    } else if constexpr (poly_setting == PolySetting::fft) {
      _v = internal::poly::fft::convolution(_v, o._v);
    } else if constexpr (poly_setting == PolySetting::fft_complex) {
      _v = internal::poly::fft::convolution_complex(_v, o._v);
    }
    return *this;
  }
  constexpr Poly operator*(const Poly &o) const {
    Poly res = *this;
    return res *= o;
  }

private:
  vector<T> _v;
};
} // namespace maomao90
