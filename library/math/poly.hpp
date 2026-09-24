#pragma once

/**
 * @file poly.hpp
 * @brief Polynomials and formal power series, with NTT/CRT/FFT multiplication.
 *
 * Coefficients are stored in ascending degree order. Series operations preserve
 * the requested precision (including trailing zeroes); polynomial division
 * normalizes its results. See `Poly` for representation and backend contracts.
 */
#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <complex>
#include <concepts>
#include <initializer_list>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include "library/internal/concepts.hpp"
#include "library/internal/math.hpp"
#include "library/math/extended_gcd.hpp"
#include "library/math/modint.hpp"

namespace maomao90 {
using namespace std;
namespace internal::poly {
template <typename T> inline long long round_integral(T x) {
  return x < 0 ? (long long)(x - T(0.5)) : (long long)(x + T(0.5));
}

template <class T>
vector<T> convolution_naive(const vector<T> &a, const vector<T> &b) {
  int n = a.size(), m = b.size();
  if (!n || !m)
    return {};
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
  array<mint, max(3, rank2 + 1)> root{};  // root[i]^(2^i) == 1, i <= rank2
  array<mint, max(3, rank2 + 1)> iroot{}; // root[i] * iroot[i] == 1

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
  assert(n > 0 && has_single_bit((unsigned int)n) &&
         (mint::imod() - 1) % n == 0);
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
  assert(n > 0 && has_single_bit((unsigned int)n) &&
         (mint::imod() - 1) % n == 0);
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
/**
 * Extends a butterfly transform from \f$n\f$ to \f$2n\f$ evaluations.
 *
 * The result is the transform of the original coefficient vector padded to
 * \f$2n\f$ entries. Input and output use butterfly order, not coefficient
 * order.
 *
 * Runs in \f$O(n\log n)\f$ time.
 *
 * @tparam mint a static modular integer with a supported prime modulus.
 * @param a the transformed coefficients, extended in place.
 * @pre `a.size()` is a nonzero power of two.
 * @pre \f$2n \mid p-1\f$, where \f$n\f$ is `a.size()` and \f$p\f$ is
 * `mint::imod()`.
 */
template <StaticModInt mint> void ntt_doubling(vector<mint> &a) {
  int n = a.size();
  assert(n > 0 && has_single_bit((unsigned int)n) &&
         (mint::imod() - 1) % (2LL * n) == 0);
  vector<mint> odd = a;
  butterfly_inv(odd);
  mint scale = mint(n).inv();
  mint step = mint(internal::math::primitive_root<mint::imod()>)
                  .pow((mint::imod() - 1) / (2LL * n));
  for (mint &coefficient : odd) {
    coefficient *= scale;
    scale *= step;
  }
  butterfly(odd);
  a.insert(a.end(), odd.begin(), odd.end());
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

  if (min(n, m) <= 60) {
    return convolution_naive(a, b);
  }
  int z = bit_ceil((unsigned int)(n + m - 1));
  assert((mint::imod() - 1) % z == 0);
  return convolution_ntt(a, b);
}
template <int mod, class T>
  requires is_integral_v<T>
vector<T> convolution(const vector<T> &a, const vector<T> &b) {
  int n = a.size(), m = b.size();
  if (!n || !m)
    return {};

  using mint = static_modint<mod>;

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
  // These depend on the current modulus for dynamic_modint.
  const long long W1 = MOD1 % mint::imod();
  const long long W2 = W1 * MOD2 % mint::imod();

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
      res[i] = T(round_integral(imag(out[i]) / (4 * z)));
    } else {
      res[i] = imag(out[i]) / (4 * z);
    }
  }
  return res;
}
template <concepts::broadly_unsigned_integral T>
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
} // namespace fft
} // namespace internal::poly
/**
 * Selects the multiplication backend used by `Poly`.
 *
 * The NTT backend provides exact modular convolution. Floating-point backends
 * trade precision for speed and require coefficients small enough to avoid
 * rounding errors or overflow.
 */
enum class PolySetting {
  ntt,      ///< Exact NTT, with CRT for unsupported transform lengths/moduli.
  fft,      ///< Real/integral convolution using double-precision FFT.
  fft_sqrt, ///< Split-coefficient FFT for unsigned integers or modints.
  fft_complex, ///< Complex convolution using the coefficient's floating type.
};
/**
 * Checks whether a coefficient type is compatible with a multiplication
 * backend.
 *
 * NTT accepts modular integers or integral types of at most \f$64\f$ bits. The
 * real FFT accepts arithmetic types, the split FFT accepts modular or unsigned
 * integral types, and the complex FFT accepts `std::complex` of a floating
 * type. This is a compile-time type check; modulus, length, and precision
 * limits are separate operation contracts documented by `Poly`.
 *
 * @tparam T the coefficient type to check.
 * @tparam setting the requested multiplication backend.
 * @see PolySetting
 */
template <typename T, PolySetting setting>
concept ValidPolySetting =
    (setting == PolySetting::ntt &&
     (ModInt<T> ||
      (integral<T> && internal::type_traits::is_64bit_or_less_v<T>))) ||
    (setting == PolySetting::fft && is_arithmetic_v<T>) ||
    (setting == PolySetting::fft_sqrt &&
     (ModInt<T> || internal::concepts::broadly_unsigned_integral<T>)) ||
    (setting == PolySetting::fft_complex && requires {
      typename T::value_type;
      requires floating_point<typename T::value_type>;
      requires same_as<T, complex<typename T::value_type>>;
    });

/**
 * A polynomial with arithmetic, evaluation, and formal power series operations.
 *
 * @par Coefficient representation
 * Coefficient \f$i\f$ represents the coefficient of \f$x^i\f$. Operations
 * preserve trailing zeroes unless stated otherwise. Both an empty vector and
 * `{0}` represent zero, but equality compares stored vectors. The default
 * constructor stores `{0}`. Use `shrink()` to obtain an empty representation of
 * zero. `Poly(n)` creates \f$n\f$ zero coefficients; `Poly{n}` creates the
 * constant \f$n\f$.
 *
 * @par Series precision
 * Series operations with a precision \f$n\f$ return exactly \f$n\f$
 * coefficients, treating missing input terms as zero. The default \f$n=-1\f$
 * means the stored input size. Precision zero returns an empty result without
 * requiring a particular constant coefficient. Other negative precisions are
 * invalid. Polynomial division via `divmod()` differs from series division via
 * `divide()`.
 *
 * @par Coefficient types and multiplication
 * Formal series operations require a static prime modulus at most
 * \f$2^{31}-1\f$. Other modular multiplication supports static or dynamic
 * moduli up to \f$2^{31}-1\f$; wider moduli throw `std::invalid_argument`,
 * including for small/empty inputs. The dynamic modulus must remain fixed while
 * its coefficients are in use. Scalar arithmetic is not subject to this
 * convolution restriction.
 *
 * NTT multiplication uses a quadratic kernel when the shorter input has at most
 * \f$60\f$ terms, otherwise a radix-\f$4\f$ transform when its length divides
 * \f$p-1\f$, where \f$p\f$ is the modulus. Other \f$32\f$-bit modular products
 * use three-prime CRT, with output length at most
 * \f$2^{24}\f$. Direct transforms at modulus \f$998244353\f$ have length at
 * most \f$2^{23}\f$. Integral NTT products require every result to fit in
 * signed \f$64\f$-bit integers and every intermediate sum in the quadratic
 * kernel to avoid overflow. FFT backends retain floating-point precision
 * limitations.
 *
 * @par Complexity notation
 * Let \f$s\f$ be the stored input size, \f$p\f$ the modulus, and \f$M(n)\f$ the
 * cost of multiplying two polynomials of length \f$n\f$. Normally
 * \f$M(n)=O(n\log(n+1))\f$. Degree-based bounds treat \f$p\f$ as fixed.
 * Additional variables are defined by each operation.
 *
 * @code{.cpp}
 * using mint = maomao90::static_modint<>;
 * using poly = maomao90::Poly<mint>;
 * poly f{1, 2, 3};
 * auto inverse = f.inv(100);          // 1/f modulo x^100
 * auto recovered = f.log(100).exp();  // f padded to 100 coefficients
 * auto [q, r] = f.divmod(poly{1, 1});
 * @endcode
 *
 * @tparam T the coefficient type, compatible with the selected backend.
 * @tparam poly_setting the multiplication backend; defaults to exact NTT/CRT.
 * @see PolySetting
 */
template <typename T, PolySetting poly_setting = PolySetting::ntt>
  requires ValidPolySetting<T, poly_setting>
struct Poly {
  /// @name Construction and coefficient storage
  /// @{
  /**
   * Constructs the zero polynomial with one stored zero coefficient.
   *
   * Runs in \f$O(1)\f$ time.
   */
  constexpr Poly() : v(1, 0) {}
  /**
   * Constructs a polynomial with \f$n\f$ stored zero coefficients.
   *
   * Runs in \f$O(n)\f$ time.
   *
   * @param n the number of coefficients to allocate.
   * @pre \f$n \ge 0\f$.
   */
  explicit constexpr Poly(int n) { resize(n); }
  /**
   * Constructs a polynomial from coefficients in ascending degree order.
   *
   * Runs in \f$O(1)\f$ time after the by-value argument is constructed;
   * passing an lvalue vector additionally copies its coefficients.
   *
   * @param v the coefficient vector, moved into the polynomial.
   */
  constexpr Poly(vector<T> v) : v(std::move(v)) {}
  /**
   * Constructs a polynomial from an explicit coefficient list.
   *
   * Runs in \f$O(n)\f$ time for \f$n\f$ coefficients.
   *
   * @param coefficients the coefficients in ascending degree order.
   */
  constexpr Poly(initializer_list<T> coefficients) : v(coefficients) {}

  /**
   * Returns the highest stored exponent, including trailing zeroes.
   *
   * Runs in \f$O(1)\f$ time.
   *
   * @return \f$s-1\f$, or \f$-1\f$ for an empty polynomial.
   * @note Call `shrink()` first when the mathematical degree is required.
   */
  constexpr int degree() const { return int(v.size()) - 1; }
  /**
   * Returns the number of stored coefficients, including trailing zeroes.
   *
   * Runs in \f$O(1)\f$ time.
   */
  constexpr int size() const { return int(v.size()); }
  /**
   * Checks whether no coefficients are stored.
   *
   * Runs in \f$O(1)\f$ time.
   *
   * @return `true` exactly when `size()==0`; `{0}` is not empty.
   */
  constexpr bool empty() const { return v.empty(); }
  /**
   * Changes the stored size, discarding high terms or appending zeroes.
   *
   * Runs in \f$O(s+n)\f$ time in the worst case, including reallocation.
   *
   * @param n the new number of coefficients.
   * @pre \f$n \ge 0\f$.
   */
  constexpr void resize(int n) {
    assert(n >= 0);
    v.resize(n);
  }
  /**
   * Provides read-only access to the stored coefficient vector.
   *
   * Runs in \f$O(1)\f$ time.
   *
   * @return a reference valid until this polynomial is destroyed; references
   *     to individual coefficients may be invalidated by mutation.
   */
  constexpr const vector<T> &coefficients() const { return v; }
  /**
   * Compares stored coefficient vectors, without removing trailing zeroes.
   *
   * Runs in \f$O(s)\f$ time in the worst case.
   *
   * @return `true` when both sizes and all stored coefficients are equal.
   */
  constexpr bool operator==(const Poly &) const = default;
  /**
   * Reads the coefficient of \f$x^i\f$.
   *
   * Runs in \f$O(1)\f$ time.
   *
   * @param i the zero-based coefficient index.
   * @pre \f$0 \le i < s\f$; missing coefficients are not implicitly accessible.
   * @return the coefficient value.
   */
  constexpr T operator[](int i) const { return v[i]; }
  /**
   * Accesses the coefficient of \f$x^i\f$ for modification.
   *
   * Runs in \f$O(1)\f$ time.
   *
   * @param i the zero-based coefficient index.
   * @pre \f$0 \le i < s\f$.
   * @return a mutable reference to the coefficient.
   */
  constexpr T &operator[](int i) { return v[i]; }
  /// @}

  /// @name Arithmetic
  /// @{
  /**
   * Multiplies this polynomial by another polynomial in place.
   *
   * Returns an empty polynomial if either input is empty. Otherwise stores
   * exactly \f$s+t-1\f$ coefficients, including trailing zeroes.
   *
   * Runs in \f$O(M(s+t))\f$ time, where \f$t\f$ is the other input's size.
   *
   * @param o the other factor; may alias this polynomial.
   * @return a reference to this polynomial.
   * @throws std::invalid_argument if the coefficient modulus exceeds
   * \f$2^{31}-1\f$. This check precedes all shortcuts and leaves this
   * polynomial unchanged.
   * @pre Inputs satisfy the backend's length, precision, and overflow limits
   *     described in `Poly`.
   */
  constexpr Poly &operator*=(const Poly &o) {
    // Reject unsupported moduli before the empty and quadratic shortcuts.
    if constexpr (StaticModInt<T>) {
      if constexpr (T::imod() > 2147483647LL) {
        throw invalid_argument(
            "Poly multiplication does not support moduli greater than 2^31-1");
      }
    }
    if (empty() || o.empty()) {
      v.clear();
      return *this;
    }
    if constexpr (poly_setting == PolySetting::ntt) {
      if constexpr (ModInt<T>) {
        if (min(size(), o.size()) <= 60) {
          v = internal::poly::convolution_naive(v, o.v);
          return *this;
        }
        if constexpr (StaticModInt<T>) {
          // The radix-4 kernel and three-prime reconstruction use 32-bit
          // moduli.
          if constexpr (T::imod() <= 2147483647LL && T::is_prime_mod) {
            int z = bit_ceil((unsigned int)(size() + o.size() - 1));
            if ((T::imod() - 1) % z == 0) {
              v = internal::poly::ntt::convolution(v, o.v);
              return *this;
            }
          }
        }
        v = internal::poly::ntt::convolution_arb_mod(v, o.v);
      } else { // integral
        vector<long long> a(degree() + 1), b(o.degree() + 1);
        for (int i = 0; i <= degree(); i++) {
          a[i] = v[i];
        }
        for (int i = 0; i <= o.degree(); i++) {
          b[i] = o.v[i];
        }
        vector<long long> res = internal::poly::ntt::convolution_ll(a, b);
        v.resize(res.size());
        for (size_t i = 0; i < res.size(); i++) {
          v[i] = res[i];
        }
      }
    } else if constexpr (poly_setting == PolySetting::fft_sqrt) {
      if constexpr (ModInt<T>) {
        v = internal::poly::fft::convolution_arb_mod(v, o.v);
      } else { // integral
        v = internal::poly::fft::convolution_sqrt(v, o.v);
      }
    } else if constexpr (poly_setting == PolySetting::fft) {
      v = internal::poly::fft::convolution(v, o.v);
    } else if constexpr (poly_setting == PolySetting::fft_complex) {
      v = internal::poly::fft::convolution_complex(v, o.v);
    }
    return *this;
  }
  /**
   * Returns the polynomial product without changing either input.
   *
   * Runs in \f$O(M(s+t))\f$ time, where \f$t\f$ is the other input's size.
   *
   * @param o the other factor.
   * @return the product, retaining trailing zeroes; empty if either input is
   * empty.
   * @throws std::invalid_argument if the coefficient modulus exceeds
   * \f$2^{31}-1\f$.
   * @see operator*=(const Poly&)
   */
  constexpr Poly operator*(const Poly &o) const {
    Poly res = *this;
    res *= o;
    return res;
  }

  /**
   * Adds another polynomial in place, padding the shorter input with zeroes.
   *
   * Runs in \f$O(s+t)\f$ time, where \f$t\f$ is the other input's size.
   *
   * @param other the polynomial to add; may alias this polynomial.
   * @return a reference to this polynomial, with \f$\max(s,t)\f$ stored terms.
   */
  constexpr Poly &operator+=(const Poly &other) {
    if (size() < other.size())
      resize(other.size());
    for (int i = 0; i < other.size(); ++i)
      v[i] += other[i];
    return *this;
  }
  /**
   * Subtracts another polynomial in place, padding the shorter input with
   * zeroes.
   *
   * Runs in \f$O(s+t)\f$ time, where \f$t\f$ is the other input's size.
   *
   * @param other the polynomial to subtract; may alias this polynomial.
   * @return a reference to this polynomial, with \f$\max(s,t)\f$ stored terms.
   */
  constexpr Poly &operator-=(const Poly &other) {
    if (size() < other.size())
      resize(other.size());
    for (int i = 0; i < other.size(); ++i)
      v[i] -= other[i];
    return *this;
  }
  /**
   * Scales every stored coefficient in place, preserving the stored size.
   *
   * Runs in \f$O(s)\f$ time.
   *
   * @param scalar the multiplier.
   * @return a reference to this polynomial.
   */
  constexpr Poly &operator*=(T scalar) {
    for (T &x : v)
      x *= scalar;
    return *this;
  }
  /**
   * Divides every stored coefficient by a modular scalar in place.
   *
   * Runs in \f$O(s+\log p)\f$ time.
   *
   * @param scalar the divisor.
   * @pre `scalar` is invertible modulo \f$p\f$.
   * @return a reference to this polynomial, with its stored size preserved.
   */
  constexpr Poly &operator/=(T scalar)
    requires ModInt<T>
  {
    assert(scalar != T(0));
    return *this *= scalar.inv();
  }
  /**
   * Returns the sum, padding the shorter input with zeroes.
   *
   * Runs in \f$O(s+t)\f$ time, where \f$t\f$ is the other input's size.
   *
   * @param other the polynomial to add.
   * @return the sum with \f$\max(s,t)\f$ stored coefficients.
   */
  constexpr Poly operator+(const Poly &other) const {
    Poly result(*this);
    result += other;
    return result;
  }
  /**
   * Returns the difference, padding the shorter input with zeroes.
   *
   * Runs in \f$O(s+t)\f$ time, where \f$t\f$ is the other input's size.
   *
   * @param other the polynomial to subtract.
   * @return the difference with \f$\max(s,t)\f$ stored coefficients.
   */
  constexpr Poly operator-(const Poly &other) const {
    Poly result(*this);
    result -= other;
    return result;
  }
  /**
   * Negates every stored coefficient.
   *
   * Runs in \f$O(s)\f$ time.
   *
   * @return the negated polynomial with the same stored size.
   */
  constexpr Poly operator-() const {
    Poly result(*this);
    for (T &x : result.v)
      x = -x;
    return result;
  }
  /**
   * Returns a scalar multiple without changing this polynomial.
   *
   * Runs in \f$O(s)\f$ time.
   *
   * @param scalar the multiplier.
   * @return the scaled polynomial with the same stored size.
   */
  constexpr Poly operator*(T scalar) const {
    Poly result(*this);
    result *= scalar;
    return result;
  }
  /**
   * Returns a scalar multiple with the scalar on the left.
   *
   * Runs in \f$O(n)\f$ time, where \f$n\f$ is the polynomial's size.
   *
   * @param scalar the multiplier.
   * @param p the polynomial to scale.
   * @return the scaled polynomial with the same stored size.
   */
  friend constexpr Poly operator*(T scalar, Poly p) {
    p *= scalar;
    return p;
  }
  /**
   * Returns the quotient by an invertible modular scalar.
   *
   * Runs in \f$O(s+\log p)\f$ time.
   *
   * @param scalar the divisor.
   * @pre `scalar` is invertible modulo \f$p\f$.
   * @return the scaled polynomial with the same stored size.
   */
  constexpr Poly operator/(T scalar) const
    requires ModInt<T>
  {
    Poly result(*this);
    result /= scalar;
    return result;
  }

  /// @}

  /// @name Coefficient transformations and linear factors
  /// @{
  /**
   * Removes trailing zero coefficients in place.
   *
   * A zero polynomial becomes empty; otherwise the stored degree becomes the
   * mathematical degree. No other operation normalizes implicitly unless its
   * documentation says so.
   *
   * Runs in \f$O(s)\f$ time.
   */
  void shrink() {
    while (!empty() && v.back() == T(0))
      v.pop_back();
  }
  /**
   * Copies a prefix of the stored coefficients without padding.
   *
   * Runs in \f$O(\min(n,s))\f$ time.
   *
   * @param n the maximum number of coefficients to copy.
   * @pre \f$n \ge 0\f$.
   * @return the first \f$\min(n,s)\f$ coefficients.
   * @see truncated()
   */
  Poly pre(int n) const {
    assert(n >= 0);
    return vector<T>(v.begin(), v.begin() + min(n, size()));
  }
  /**
   * Copies the series to exactly \f$n\f$ coefficients, truncating or padding.
   *
   * Runs in \f$O(n)\f$ time.
   *
   * @param n the requested number of coefficients.
   * @pre \f$n \ge 0\f$.
   * @return the first \f$n\f$ coefficients, with missing terms set to zero.
   * @see pre()
   */
  Poly truncated(int n) const {
    Poly result = pre(n);
    result.resize(n);
    return result;
  }
  /**
   * Reverses the stored coefficient vector, including trailing zeroes.
   *
   * Runs in \f$O(s)\f$ time.
   *
   * @return a polynomial of the same stored size in reversed order.
   */
  Poly rev() const { return vector<T>(v.rbegin(), v.rend()); }
  /**
   * Evaluates the polynomial at one point using Horner's rule.
   *
   * Runs in \f$O(s)\f$ time.
   *
   * @param x the evaluation point.
   * @return the value \f$f(x)\f$; zero for an empty polynomial.
   */
  T eval(T x) const {
    T result = 0;
    for (int i = size(); i-- > 0;)
      result = result * x + v[i];
    return result;
  }
  /**
   * Multiplies by the linear factor \f$ax+b\f$ in place.
   *
   * The stored size increases by one, including when the input is zero.
   *
   * Runs in \f$O(s)\f$ time.
   *
   * @param a the coefficient of \f$x\f$ in the factor.
   * @param b the constant coefficient of the factor.
   * @pre \f$a \ne 0\f$.
   */
  void multiply(T a, T b) {
    assert(a != T(0));
    int n = size();
    v.push_back(0);
    for (int i = n; i-- > 0;) {
      v[i + 1] += v[i] * a;
      v[i] *= b;
    }
  }
  /**
   * Divides exactly by the linear factor \f$ax+b\f$ in place.
   *
   * The stored size decreases by one. This operation requires a zero remainder;
   * use `divmod()` when the factor need not divide exactly. \f$b=0\f$ is
   * allowed.
   *
   * Runs in \f$O(s+\log p)\f$ time.
   *
   * @param a the coefficient of \f$x\f$ in the divisor.
   * @param b the constant coefficient of the divisor.
   * @pre The input is nonempty and \f$a\f$ is invertible modulo \f$p\f$.
   * @pre \f$ax+b\f$ divides the input polynomial exactly.
   */
  void divide(T a, T b)
    requires ModInt<T>
  {
    assert(a != T(0) && !empty());
    // Work from the leading coefficient: b may be zero, without a special case.
    T inverse = a.inv(), carry = v.back() * inverse;
    for (int i = size() - 2; i >= 0; --i) {
      T next = (v[i] - b * carry) * inverse;
      v[i] = carry;
      carry = next;
    }
    assert(carry == T(0));
    v.pop_back();
  }
  /// @}

  /// @name Differentiation and integration
  /// @{
  /**
   * Computes the formal derivative.
   *
   * Runs in \f$O(s)\f$ time.
   *
   * @return \f$f'(x)\f$ with \f$\max(0,s-1)\f$ stored terms. Constants and
   * empty inputs both yield an empty polynomial.
   */
  Poly differ() const {
    Poly result(max(0, size() - 1));
    for (int i = 1; i < size(); ++i)
      result[i - 1] = v[i] * T(i);
    return result;
  }
  /**
   * Computes the formal integral with constant coefficient zero.
   *
   * Integer inverses are computed in a single linear pass.
   *
   * Runs in \f$O(s)\f$ time.
   *
   * @pre `T` is a static modular integer with prime modulus \f$p \le
   * 2^{31}-1\f$.
   * @pre \f$s < p\f$, so all required denominators are invertible.
   * @return the integral with \f$s+1\f$ stored terms; `{0}` for empty input.
   */
  Poly integral() const
    requires StaticModInt<T>
  {
    auto inverses = integer_inverses(size());
    Poly result(size() + 1);
    for (int i = 0; i < size(); ++i)
      result[i + 1] = v[i] * inverses[i + 1];
    return result;
  }

  /// @}

  /// @name Formal power series
  /// @{
  /**
   * Computes the reciprocal series modulo \f$x^n\f$ using Newton doubling.
   *
   * When a direct NTT is available, only the unknown high half is computed and
   * the inverse's transform is reused. Other moduli use convolution doubling.
   *
   * Runs in \f$O(M(n))\f$ time.
   *
   * @param n the number of output coefficients; \f$-1\f$ means `size()`.
   * @pre `T` is a static modular integer with prime modulus \f$p \le
   * 2^{31}-1\f$.
   * @pre \f$n \ge -1\f$; for positive resolved precision, the input is nonempty
   *     and its constant coefficient is nonzero.
   * @return exactly \f$n\f$ coefficients of \f$1/f\f$, or an empty polynomial
   * when \f$n=0\f$.
   */
  Poly inv(int n = -1) const
    requires StaticModInt<T>
  {
    n = precision(n);
    if (!n)
      return Poly(0);
    assert(!empty() && v[0] != T(0));
    Poly result{v[0].inv()};
    for (int m = 1; m < n; m *= 2) {
      int length = min(2 * m, n);
      if (can_transform(2 * m)) {
        // Cyclic aliasing only affects the already-known low half. Reuse the
        // transform of g in g_new = g - g*(f*g-1), computing only the high
        // half.
        vector<T> f = pre(length).v, g = result.v;
        f.resize(2 * m);
        g.resize(2 * m);
        transform(f);
        transform(g);
        for (int i = 0; i < 2 * m; ++i)
          f[i] *= g[i];
        inverse_transform(f);
        fill(f.begin(), f.begin() + m, T(0));
        transform(f);
        for (int i = 0; i < 2 * m; ++i)
          f[i] *= g[i];
        inverse_transform(f);
        result.resize(length);
        for (int i = m; i < length; ++i)
          result[i] = -f[i];
      } else {
        Poly error = (pre(length) * result).truncated(length);
        error = -error;
        error[0] += T(2);
        result = (result * error).truncated(length);
      }
    }
    return result;
  }
  /**
   * Divides as a formal power series modulo \f$x^n\f$.
   *
   * This computes a truncated series, not a polynomial quotient and remainder.
   *
   * Runs in \f$O(M(n))\f$ time.
   *
   * @param denominator the divisor series.
   * @param n the number of output coefficients; \f$-1\f$ means the numerator's
   * size.
   * @pre `T` is a static modular integer with prime modulus \f$p \le
   * 2^{31}-1\f$.
   * @pre \f$n \ge -1\f$; for positive resolved precision, `denominator` is
   * nonempty and `denominator[0] != 0`.
   * @return exactly \f$n\f$ coefficients of \f$f/g\f$, where \f$g\f$ is
   * `denominator`.
   * @see divmod()
   */
  Poly divide(const Poly &denominator, int n = -1) const
    requires StaticModInt<T>
  {
    n = precision(n);
    return (pre(n) * denominator.inv(n)).truncated(n);
  }
  /**
   * Replaces this polynomial with its truncated series quotient.
   *
   * Runs in \f$O(M(n))\f$ time.
   *
   * @param denominator the divisor series; may alias this polynomial.
   * @param n the output precision; \f$-1\f$ means this polynomial's original
   * size.
   * @pre The coefficient field, precision, and divisor satisfy `divide()`.
   * @return a reference to this polynomial with exactly \f$n\f$ stored
   * coefficients.
   * @see divide(const Poly&, int)
   */
  Poly &divide_in_place(const Poly &denominator, int n = -1)
    requires StaticModInt<T>
  {
    return *this = divide(denominator, n);
  }
  /**
   * Computes the formal logarithm modulo \f$x^n\f$.
   *
   * Uses the identity \f$(\log f)'=f'/f\f$ and sets the constant term to zero.
   *
   * Runs in \f$O(M(n))\f$ time.
   *
   * @param n the number of output coefficients; \f$-1\f$ means `size()`.
   * @pre `T` is a static modular integer with prime modulus \f$p \le
   * 2^{31}-1\f$.
   * @pre \f$n \ge -1\f$ and the resolved precision is at most \f$p\f$.
   * @pre For positive precision, the input is nonempty and \f$f(0)=1\f$.
   * @return exactly \f$n\f$ coefficients of \f$\log f\f$, with constant
   * coefficient zero.
   */
  Poly log(int n = -1) const
    requires StaticModInt<T>
  {
    n = precision(n);
    if (!n)
      return Poly(0);
    assert(!empty() && v[0] == T(1) && (long long)n <= T::imod());
    return (pre(n).differ() * inv(n - 1)).truncated(n - 1).integral();
  }
  /**
   * Computes the formal exponential modulo \f$x^n\f$.
   *
   * Empty input is treated as zero, whose exponential is one. The direct NTT
   * path maintains the exponential and its inverse, reusing their transforms.
   *
   * Runs in \f$O(M(n))\f$ time.
   *
   * @param n the number of output coefficients; \f$-1\f$ means `size()`.
   * @pre `T` is a static modular integer with prime modulus \f$p \le
   * 2^{31}-1\f$.
   * @pre \f$n \ge -1\f$ and the resolved precision is at most \f$p\f$.
   * @pre For positive precision, the input is empty or \f$f(0)=0\f$.
   * @return exactly \f$n\f$ coefficients of \f$\exp f\f$; the constant is one
   * when \f$n>0\f$.
   */
  Poly exp(int n = -1) const
    requires StaticModInt<T>
  {
    n = precision(n);
    if (!n)
      return Poly(0);
    assert((empty() || v[0] == T(0)) && (long long)n <= T::imod());
    if (!can_transform(bit_ceil((unsigned int)n))) {
      Poly result{1};
      for (int m = 1; m < n; m *= 2) {
        int length = min(2 * m, n);
        Poly correction = pre(length) - result.log(length);
        correction[0] += T(1);
        result = (result * correction).truncated(length);
      }
      return result;
    }
    // Maintain f=exp(input) mod x^m and g=1/f mod x^(m/2).
    // The previous transform of g is reused to extend its inverse precision.
    Poly f = truncated(n), g{1};
    f[0] = 1;
    vector<T> transformed_inverse;
    Poly derivative = pre(n).differ().truncated(n);
    auto inverses = integer_inverses(n - 1);
    for (int m = 1; m < n; m *= 2) {
      vector<T> transformed_f = f.pre(m).v;
      transformed_f.resize(2 * m);
      transform(transformed_f);
      if (m > 1) {
        vector<T> error(m);
        for (int i = 0; i < m; ++i)
          error[i] = transformed_f[i] * transformed_inverse[i];
        inverse_transform(error);
        copy(error.begin() + m / 2, error.end(), error.begin());
        fill(error.begin() + m / 2, error.end(), T(0));
        transform(error);
        for (int i = 0; i < m; ++i)
          error[i] *= transformed_inverse[i];
        inverse_transform(error);
        g.resize(m);
        for (int i = 0; i < m / 2; ++i)
          g[m / 2 + i] = -error[i];
      }
      vector<T> residual = derivative.pre(m - 1).v;
      residual.resize(m);
      transform(residual);
      for (int i = 0; i < m; ++i)
        residual[i] *= transformed_f[i];
      inverse_transform(residual);
      // x*(f' - input'*f), cyclically modulo x^m-1. Its low coefficients
      // vanish before wrapping, leaving precisely the error needed above m.
      Poly difference = f.pre(m).differ().truncated(m);
      for (int i = 0; i < m; ++i)
        difference[i] -= residual[i];
      rotate(difference.v.rbegin(), difference.v.rbegin() + 1,
             difference.v.rend());
      difference.resize(2 * m);
      transform(difference.v);
      transformed_inverse = g.v;
      transformed_inverse.resize(2 * m);
      transform(transformed_inverse);
      for (int i = 0; i < 2 * m; ++i)
        difference[i] *= transformed_inverse[i];
      inverse_transform(difference.v);
      vector<T> correction(2 * m);
      for (int i = 0; i < m && m + i < n; ++i)
        correction[i] = f[m + i] - difference[i] * inverses[m + i];
      transform(correction);
      for (int i = 0; i < 2 * m; ++i)
        correction[i] *= transformed_f[i];
      inverse_transform(correction);
      for (int i = 0; i < m && m + i < n; ++i)
        f[m + i] = correction[i];
    }
    return f;
  }
  /**
   * Raises the series to a nonnegative integer power modulo \f$x^n\f$.
   *
   * Leading zeroes are factored out before applying logarithm and exponential.
   * Defines \f$f^0=1\f$, including \f$0^0\f$, and supports signed \f$64\f$-bit
   * exponents without overflowing the degree shift.
   *
   * Runs in \f$O(s+M(n)+\log(k+1))\f$ time.
   *
   * @param k the exponent.
   * @param n the number of output coefficients; \f$-1\f$ means `size()`.
   * @pre `T` is a static modular integer with prime modulus \f$p \le
   * 2^{31}-1\f$.
   * @pre \f$k \ge 0\f$, \f$n \ge -1\f$, and the resolved precision is at most
   * \f$p\f$.
   * @return exactly \f$n\f$ coefficients of \f$f^k\f$.
   * @see sparse_pow()
   */
  Poly pow(long long k, int n = -1) const
    requires StaticModInt<T>
  {
    n = precision(n);
    assert(k >= 0 && (long long)n <= T::imod());
    Poly result(n);
    if (!n)
      return result;
    if (!k) {
      result[0] = 1;
      return result;
    }
    int leading = 0;
    while (leading < size() && v[leading] == T(0))
      ++leading;
    if (leading == size() || leading > (n - 1) / k)
      return result;
    int shift = int(leading * k), length = n - shift;
    Poly unit(vector<T>(v.begin() + leading,
                        v.begin() + min(size(), leading + length)));
    unit /= v[leading];
    unit = (unit.log(length) * T(k)).exp(length) * v[leading].pow(k);
    copy(unit.v.begin(), unit.v.end(), result.v.begin() + shift);
    return result;
  }
  /**
   * Raises a sparse series to a nonnegative integer power modulo \f$x^n\f$.
   *
   * Uses \f$fg'=kf'g\f$ to compute \f$g=f^k\f$ from the nonzero input
   * coefficients. Input and output still use dense vectors. Defines
   * \f$f^0=1\f$, including \f$0^0\f$.
   *
   * Runs in \f$O(s+n(t+1)+\log(k+1))\f$ time, where \f$t\f$ is the number of
   * relevant nonzero input coefficients after removing the leading zeroes.
   *
   * @param k the exponent.
   * @param n the number of output coefficients; \f$-1\f$ means `size()`.
   * @pre `T` is a static modular integer with prime modulus \f$p \le
   * 2^{31}-1\f$.
   * @pre \f$k \ge 0\f$, \f$n \ge -1\f$, and the resolved precision is at most
   * \f$p\f$.
   * @return exactly \f$n\f$ coefficients of \f$f^k\f$.
   * @see pow()
   */
  Poly sparse_pow(long long k, int n = -1) const
    requires StaticModInt<T>
  {
    n = precision(n);
    assert(k >= 0 && (long long)n <= T::imod());
    Poly result(n);
    if (!n)
      return result;
    if (!k) {
      result[0] = 1;
      return result;
    }
    int leading = 0;
    while (leading < size() && v[leading] == T(0))
      ++leading;
    if (leading == size() || leading > (n - 1) / k)
      return result;
    int shift = int(leading * k), length = n - shift;
    vector<pair<int, T>> terms;
    for (int i = leading + 1; i < size() && i - leading < length; ++i)
      if (v[i] != T(0))
        terms.emplace_back(i - leading, v[i]);
    auto inverses = integer_inverses(length - 1);
    T inverse_constant = v[leading].inv(), exponent = T(k);
    result[shift] = v[leading].pow(k);
    for (int i = 1; i < length; ++i) {
      T sum = 0;
      for (auto [d, coefficient] : terms) {
        if (d > i)
          break;
        sum += coefficient * ((exponent + T(1)) * T(d) - T(i)) *
               result[shift + i - d];
      }
      result[shift + i] = sum * inverse_constant * inverses[i];
    }
    return result;
  }
  /// @}

  /// @name Polynomial division, shifting, and evaluation
  /// @{
  /**
   * Computes the polynomial quotient and remainder.
   *
   * Removes trailing zeroes from the inputs before division. The results
   * satisfy \f$f=qg+r\f$, where \f$g\f$ is `divisor`, and either \f$r=0\f$ or
   * \f$\deg r < \deg g\f$.
   * Both results are normalized: zero is represented by an empty vector.
   *
   * Runs in \f$O(M(s+t))\f$ time, where \f$t\f$ is the divisor's stored size.
   *
   * @param divisor the polynomial divisor; trailing zeroes are allowed.
   * @pre `T` is a static modular integer with prime modulus \f$p \le
   * 2^{31}-1\f$.
   * @pre The divisor has at least one nonzero coefficient.
   * @return a pair containing the quotient and remainder, in that order.
   * @see divide(const Poly&, int)
   */
  pair<Poly, Poly> divmod(const Poly &divisor) const
    requires StaticModInt<T>
  {
    assert_field();
    Poly numerator(*this), denominator(divisor);
    numerator.shrink();
    denominator.shrink();
    assert(!denominator.empty());
    if (numerator.size() < denominator.size())
      return {Poly(0), numerator};
    int length = numerator.size() - denominator.size() + 1;
    Poly quotient;
    if (min(length, denominator.size()) <= 60) {
      quotient.resize(length);
      T inverse = denominator.v.back().inv();
      for (int i = length; i-- > 0;) {
        T coefficient = numerator[i + denominator.size() - 1] * inverse;
        quotient[i] = coefficient;
        for (int j = 0; j < denominator.size(); ++j)
          numerator[i + j] -= coefficient * denominator[j];
      }
    } else {
      quotient = (numerator.rev().pre(length) * denominator.rev().inv(length))
                     .truncated(length)
                     .rev();
      numerator -= quotient * denominator;
    }
    numerator.resize(denominator.size() - 1);
    numerator.shrink();
    quotient.shrink();
    return {std::move(quotient), std::move(numerator)};
  }
  /**
   * Computes the polynomial quotient, discarding the remainder.
   *
   * Runs in \f$O(M(s+t))\f$ time, where \f$t\f$ is the divisor's stored size.
   *
   * @param divisor the polynomial divisor.
   * @pre The coefficient field and nonzero divisor satisfy `divmod()`.
   * @return the normalized quotient; empty when its value is zero.
   * @see divmod()
   */
  Poly divide_as_poly(const Poly &divisor) const
    requires StaticModInt<T>
  {
    return divmod(divisor).first;
  }
  /**
   * Returns the polynomial quotient, not a truncated series quotient.
   *
   * Runs in \f$O(M(s+t))\f$ time, where \f$t\f$ is the divisor's stored size.
   *
   * @param divisor the polynomial divisor.
   * @pre The coefficient field and nonzero divisor satisfy `divmod()`.
   * @return the normalized quotient.
   * @see divide(const Poly&, int)
   */
  Poly operator/(const Poly &divisor) const
    requires StaticModInt<T>
  {
    return divmod(divisor).first;
  }
  /**
   * Returns the remainder of polynomial division.
   *
   * Runs in \f$O(M(s+t))\f$ time, where \f$t\f$ is the divisor's stored size.
   *
   * @param divisor the polynomial divisor.
   * @pre The coefficient field and nonzero divisor satisfy `divmod()`.
   * @return the normalized remainder, of degree less than the divisor.
   * @see divmod()
   */
  Poly operator%(const Poly &divisor) const
    requires StaticModInt<T>
  {
    return divmod(divisor).second;
  }
  /**
   * Shifts the polynomial's argument by a constant.
   *
   * Runs in \f$O(M(s))\f$ time.
   *
   * @param c the shift applied to \f$x\f$.
   * @pre `T` is a static modular integer with prime modulus \f$p \le
   * 2^{31}-1\f$.
   * @pre \f$s \le p\f$, so the required factorials are invertible.
   * @return \f$f(x+c)\f$ with the original stored size, including trailing
   * zeroes.
   */
  Poly taylor_shift(T c) const
    requires StaticModInt<T>
  {
    assert_field();
    int n = size();
    if (!n)
      return Poly(0);
    assert((long long)n <= T::imod());
    vector<T> factorial(n, 1), inverse_factorial(n, 1);
    for (int i = 1; i < n; ++i)
      factorial[i] = factorial[i - 1] * T(i);
    inverse_factorial.back() = factorial.back().inv();
    for (int i = n - 1; i > 0; --i)
      inverse_factorial[i - 1] = inverse_factorial[i] * T(i);
    Poly a(n), b(n);
    T power = 1;
    for (int i = 0; i < n; ++i) {
      a[n - 1 - i] = v[i] * factorial[i];
      b[i] = power * inverse_factorial[i];
      power *= c;
    }
    Poly product = a * b, result(n);
    for (int i = 0; i < n; ++i)
      result[i] = product[n - 1 - i] * inverse_factorial[i];
    return result;
  }
  /**
   * Evaluates the polynomial at multiple points using a product tree.
   *
   * Repeated evaluation points are allowed and retain their input order.
   *
   * Runs in \f$O(M(s)+M(m)\log(m+1))\f$ time for \f$m\f$ points, using
   * \f$O(s+m\log(m+1))\f$ auxiliary space.
   *
   * @param xs the evaluation points.
   * @pre `T` is a static modular integer with prime modulus \f$p \le
   * 2^{31}-1\f$.
   * @return the vector \f$(f(x_0),\ldots,f(x_{m-1}))\f$, where \f$x_i\f$ is
   * `xs[i]`; empty for no points.
   */
  vector<T> multipoint_eval(const vector<T> &xs) const
    requires StaticModInt<T>
  {
    assert_field();
    int m = xs.size();
    vector<T> result(m);
    if (!m || empty())
      return result;
    auto tree = product_tree(xs);
    int base = tree.size() / 2;
    auto descend = [&](auto &&self, int node, int left, int right,
                       const Poly &f) -> void {
      if (left >= m)
        return;
      if (right - left <= 32) {
        for (int i = left; i < min(right, m); ++i)
          result[i] = f.eval(xs[i]);
        return;
      }
      int middle = (left + right) / 2;
      self(self, 2 * node, left, middle, f % tree[2 * node]);
      self(self, 2 * node + 1, middle, right, f % tree[2 * node + 1]);
    };
    descend(descend, 1, 0, base, *this % tree[1]);
    return result;
  }
  /**
   * Interpolates the unique polynomial of degree less than the number of
   * points.
   *
   * Uses a product tree and batch inversion of the derivative evaluations.
   *
   * Runs in \f$O(M(m)\log(m+1))\f$ time for \f$m\f$ points, using
   * \f$O(m\log(m+1))\f$ auxiliary space.
   *
   * @param xs the interpolation points.
   * @param ys the polynomial values at the corresponding points.
   * @pre `T` is a static modular integer with prime modulus \f$p \le
   * 2^{31}-1\f$.
   * @pre `xs.size() == ys.size()` and the elements of `xs` are pairwise
   * distinct modulo \f$p\f$.
   * @return exactly \f$m\f$ coefficients satisfying \f$f(x_i)=y_i\f$, where
   *     \f$x_i\f$ is `xs[i]` and \f$y_i\f$ is `ys[i]`; empty
   *     when no points are supplied.
   * @see multipoint_eval()
   */
  static Poly interpolate(const vector<T> &xs, const vector<T> &ys)
    requires StaticModInt<T>
  {
    assert_field();
    assert(xs.size() == ys.size());
    int m = xs.size();
    if (!m)
      return Poly(0);
    auto tree = product_tree(xs);
    int base = tree.size() / 2;
    vector<T> weights = tree[1].differ().multipoint_eval(xs);
    // Batch inversion: one modular inverse instead of m exponentiations.
    vector<T> prefix(m + 1, 1);
    for (int i = 0; i < m; ++i) {
      assert(weights[i] != T(0));
      prefix[i + 1] = prefix[i] * weights[i];
    }
    T inverse = prefix[m].inv();
    vector<Poly> values(2 * base, Poly(0));
    for (int i = m; i-- > 0;) {
      values[base + i] = Poly{ys[i] * prefix[i] * inverse};
      inverse *= weights[i];
    }
    for (int i = base; --i > 0;)
      values[i] =
          values[2 * i] * tree[2 * i + 1] + values[2 * i + 1] * tree[2 * i];
    return values[1].truncated(m);
  }
  /// @}

  /// @name Square roots and rational series coefficients
  /// @{
  /**
   * Computes a formal square root modulo \f$x^n\f$, if one exists.
   *
   * Uses Tonelli-Shanks for the first nonzero coefficient and Newton doubling
   * for the remaining series. Either sign is valid. Any high coefficients left
   * undetermined by a leading power of \f$x\f$ are set to zero.
   *
   * Runs in \f$O(M(n))\f$ time plus one scalar Tonelli-Shanks square root.
   *
   * @param n the number of output coefficients; \f$-1\f$ means `size()`.
   * @pre `T` is a static modular integer with an odd prime modulus \f$p \le
   * 2^{31}-1\f$.
   * @pre \f$n \ge -1\f$ and the resolved precision is at most \f$p\f$.
   * @return a series \f$g\f$ with exactly \f$n\f$ terms satisfying
   * \f$g^2\equiv f\pmod{x^n}\f$, or `std::nullopt` if the first nonzero term of
   * degree below \f$n\f$ has odd degree or a nonsquare coefficient. A zero
   * input modulo \f$x^n\f$ yields \f$n\f$ zeroes.
   */
  optional<Poly> sqrt(int n = -1) const
    requires StaticModInt<T>
  {
    n = precision(n);
    assert(T::imod() != 2 && (long long)n <= T::imod());
    if (!n)
      return Poly(0);
    int leading = 0;
    while (leading < min(n, size()) && v[leading] == T(0))
      ++leading;
    if (leading == min(n, size()))
      return Poly(n);
    if (leading & 1)
      return nullopt;
    auto root = modular_sqrt(v[leading]);
    if (!root)
      return nullopt;
    int length = n - leading;
    Poly unit(vector<T>(v.begin() + leading, v.begin() + min(size(), n)));
    Poly result{*root};
    T inverse_two = T(2).inv();
    for (int m = 1; m < length; m *= 2) {
      int next = min(2 * m, length);
      result = (result + unit.divide(result, next)) * inverse_two;
    }
    result.v.insert(result.v.begin(), leading / 2, T(0));
    result.resize(n);
    return result;
  }

  /**
   * Extracts one coefficient of a rational series using Bostan-Mori.
   *
   * The numerator may have degree at least that of the denominator; its
   * polynomial part is handled first. For large inputs with a suitable NTT,
   * parity elimination and transform doubling reuse evaluations across steps.
   *
   * Runs in \f$O(M(d)\log(k+2))\f$ time, where \f$d\f$ is the larger stored
   * input size.
   *
   * @param k the zero-based index of the coefficient to extract.
   * @param numerator the numerator polynomial, consumed by value.
   * @param denominator the denominator polynomial, consumed by value.
   * @pre `T` is a static modular integer with prime modulus \f$p \le
   * 2^{31}-1\f$.
   * @pre \f$k \ge 0\f$; `denominator` is nonempty and `denominator[0] != 0`.
   * @return the coefficient \f$[x^k]\,P(x)/Q(x)\f$, where \f$P\f$ is
   * `numerator` and \f$Q\f$ is `denominator`.
   * @see linear_recurrence()
   */
  static T bostan_mori(long long k, Poly numerator, Poly denominator)
    requires StaticModInt<T>
  {
    assert_field();
    assert(k >= 0 && !denominator.empty() && denominator[0] != T(0));
    numerator.shrink();
    denominator.shrink();
    T polynomial_part = 0;
    if (numerator.size() >= denominator.size()) {
      auto [quotient, remainder] = numerator.divmod(denominator);
      if (k < quotient.size())
        polynomial_part = quotient[int(k)];
      numerator = std::move(remainder);
    }
    if (numerator.empty())
      return polynomial_part;
    int base = bit_ceil((unsigned int)denominator.size());
    if (base > 64 && can_transform(2 * base)) {
      // In butterfly order, adjacent evaluations are at z and -z.
      // Eliminate odd coefficients directly in transform space, then double
      // the transforms for the next iteration instead of multiplying anew.
      numerator.resize(2 * base);
      denominator.resize(2 * base);
      transform(numerator.v);
      transform(denominator.v);
      vector<int> reversed_bits(base);
      for (int i = 1; i < base; ++i)
        reversed_bits[i] = (reversed_bits[i / 2] / 2) + (i & 1) * (base / 2);
      T inverse_two = T(2).inv();
      T inverse_root = T(internal::math::primitive_root<T::imod()>)
                           .inv()
                           .pow((T::imod() - 1) / (2 * base));
      vector<T> next_numerator(base), next_denominator(base);
      while (k) {
        for (int i = 0; i < base; ++i)
          next_denominator[i] = denominator[2 * i] * denominator[2 * i + 1];
        if (k & 1) {
          // (p(z)q(-z)-p(-z)q(z))/(2z), visited in ordinary root order.
          T scale = inverse_two;
          for (int i : reversed_bits) {
            next_numerator[i] = (numerator[2 * i] * denominator[2 * i + 1] -
                                 numerator[2 * i + 1] * denominator[2 * i]) *
                                scale;
            scale *= inverse_root;
          }
        } else {
          for (int i = 0; i < base; ++i)
            next_numerator[i] = (numerator[2 * i] * denominator[2 * i + 1] +
                                 numerator[2 * i + 1] * denominator[2 * i]) *
                                inverse_two;
        }
        numerator.v = next_numerator;
        denominator.v = next_denominator;
        k >>= 1;
        if (k) {
          internal::poly::ntt::ntt_doubling(numerator.v);
          internal::poly::ntt::ntt_doubling(denominator.v);
        }
      }
      // The constant coefficient is the average of all evaluations.
      T numerator_sum = 0, denominator_sum = 0;
      for (T value : numerator.v)
        numerator_sum += value;
      for (T value : denominator.v)
        denominator_sum += value;
      return polynomial_part + numerator_sum / denominator_sum;
    }
    while (k && !numerator.empty()) {
      Poly reflected = denominator;
      for (int i = 1; i < reflected.size(); i += 2)
        reflected[i] = -reflected[i];
      Poly p = numerator * reflected, q = denominator * reflected;
      numerator.resize((p.size() + 1 - (k & 1)) / 2);
      for (int i = 0; i < numerator.size(); ++i)
        numerator[i] = p[2 * i + (k & 1)];
      denominator.resize((q.size() + 1) / 2);
      for (int i = 0; i < denominator.size(); ++i)
        denominator[i] = q[2 * i];
      numerator.shrink();
      k >>= 1;
    }
    return polynomial_part +
           (numerator.empty() ? T(0) : numerator[0] / denominator[0]);
  }
  /**
   * Computes a term of a sequence defined by a linear recurrence.
   *
   * With \f$d\f$ initial terms and \f$c_j\f$ equal to `recurrence[j]`, the
   * convention is
   * \f[
   *   a_i=\sum_{j=0}^{d-1}c_j a_{i-1-j},\qquad i\ge d.
   * \f]
   * For example, `linear_recurrence({0,1}, {1,1}, k)` returns \f$F_k\f$, the
   * \f$k\f$-th Fibonacci number.
   *
   * Runs in \f$O(M(d)\log(k+2))\f$ time; returns in \f$O(1)\f$ time
   * when \f$k\f$ refers to an initial term.
   *
   * @param initial the values \f$a_0,\ldots,a_{d-1}\f$.
   * @param recurrence the coefficients in order of increasing lag.
   * @param k the zero-based index of the requested term.
   * @pre `T` is a static modular integer with prime modulus \f$p \le
   * 2^{31}-1\f$.
   * @pre `initial.size() == recurrence.size() > 0` and \f$k \ge 0\f$.
   * @return \f$a_k\f$.
   * @see bostan_mori()
   */
  static T linear_recurrence(const vector<T> &initial,
                             const vector<T> &recurrence, long long k)
    requires StaticModInt<T>
  {
    assert_field();
    assert(k >= 0 && !initial.empty() && initial.size() == recurrence.size());
    if (k < (long long)initial.size())
      return initial[int(k)];
    Poly denominator(int(recurrence.size()) + 1);
    denominator[0] = 1;
    for (int i = 0; i < (int)recurrence.size(); ++i)
      denominator[i + 1] = -recurrence[i];
    Poly numerator = (Poly(initial) * denominator).pre(initial.size());
    return bostan_mori(k, std::move(numerator), std::move(denominator));
  }

  /// @}

private:
  static void assert_field()
    requires StaticModInt<T>
  {
    static_assert(T::is_prime_mod,
                  "Formal power series require a prime modulus");
    static_assert(T::imod() <= 2147483647LL,
                  "Formal power series require a 32-bit modulus");
  }
  int precision(int n) const
    requires StaticModInt<T>
  {
    assert_field();
    assert(n >= -1);
    return n == -1 ? size() : n;
  }
  static vector<T> integer_inverses(int n)
    requires StaticModInt<T>
  {
    assert_field();
    assert(n >= 0 && (long long)n < T::imod());
    vector<T> result(n + 1);
    if (n)
      result[1] = 1;
    for (int i = 2; i <= n; ++i)
      result[i] = -T(T::imod() / i) * result[T::imod() % i];
    return result;
  }
  static bool can_transform(unsigned int n)
    requires StaticModInt<T>
  {
    return poly_setting == PolySetting::ntt && (T::imod() - 1) % n == 0;
  }
  static void transform(vector<T> &a)
    requires StaticModInt<T>
  {
    assert(!a.empty() && has_single_bit(a.size()) && can_transform(a.size()));
    internal::poly::ntt::butterfly(a);
  }
  static void inverse_transform(vector<T> &a)
    requires StaticModInt<T>
  {
    internal::poly::ntt::butterfly_inv(a);
    T scale = T(a.size()).inv();
    for (T &x : a)
      x *= scale;
  }
  static vector<Poly> product_tree(const vector<T> &xs)
    requires StaticModInt<T>
  {
    int base = bit_ceil((unsigned int)xs.size());
    vector<Poly> tree(2 * base, Poly{1});
    for (int i = 0; i < (int)xs.size(); ++i)
      tree[base + i] = Poly{-xs[i], 1};
    for (int i = base; --i > 0;)
      tree[i] = tree[2 * i] * tree[2 * i + 1];
    return tree;
  }
  // Tonelli-Shanks, entirely in the coefficient field.
  static optional<T> modular_sqrt(T a)
    requires StaticModInt<T>
  {
    if (a == T(0))
      return T(0);
    if (a.pow((T::imod() - 1) / 2) != T(1))
      return nullopt;
    long long odd = T::imod() - 1;
    int exponent = 0;
    while (!(odd & 1)) {
      odd >>= 1;
      ++exponent;
    }
    T nonresidue = 2;
    while (nonresidue.pow((T::imod() - 1) / 2) == T(1))
      ++nonresidue;
    T c = nonresidue.pow(odd), root = a.pow((odd + 1) / 2),
      remainder = a.pow(odd);
    while (remainder != T(1)) {
      int i = 0;
      T squared = remainder;
      while (squared != T(1)) {
        squared *= squared;
        ++i;
      }
      T factor = c.pow(1LL << (exponent - i - 1));
      root *= factor;
      c = factor * factor;
      remainder *= c;
      exponent = i;
    }
    return root;
  }
  vector<T> v;
};
} // namespace maomao90
