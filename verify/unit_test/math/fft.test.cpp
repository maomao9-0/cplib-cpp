#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <cstdint>
#include <iostream>
#include <limits>
#include <numbers>
#include <random>
#include <vector>

#include "library/math/poly.hpp"
using namespace std;
using namespace maomao90;
namespace fft_impl = maomao90::internal::poly::fft;

template <class T> void check_transform() {
  mt19937 rng(901234);
  // Exercise both radix parities, repeated sizes, and cache growth/shrinkage.
  for (int n : {0, 1, 2, 4, 8, 16, 32, 64, 128, 8, 256, 2}) {
    vector<complex<T>> a(n);
    for (auto &x : a)
      x = {T(int(rng() % 31) - 15), T(int(rng() % 31) - 15)};
    auto original = a;
    fft_impl::fft(a);
    for (int k = 0; k < n; ++k) {
      complex<long double> expected{};
      for (int j = 0; j < n; ++j) {
        auto w =
            polar(1.0L, 2 * numbers::pi_v<long double> * ((j * k) % n) / n);
        expected += complex<long double>(original[j]) * w;
      }
      long double tolerance = 256 * numeric_limits<T>::epsilon() * max(1, n);
      assert(abs(complex<long double>(a[k]) - expected) < tolerance);
    }
    if (n) {
      reverse(a.begin() + 1, a.end());
      fft_impl::fft(a);
      for (int i = 0; i < n; ++i) {
        auto error = abs(complex<long double>(a[i] / T(n) - original[i]));
        assert(error < 256 * numeric_limits<T>::epsilon() * max(1, n));
      }
    }
  }
}

void check_rounding() {
  for (double x : {-2.5, -1.5, -0.5, -0.25, 0.0, 0.25, 0.5, 1.5, 2.5})
    assert(internal::poly::round_integral(x) == llround(x));
  for (long long x : {(1LL << 52) - 1, (1LL << 52) + 1, (1LL << 53) - 1}) {
    assert(internal::poly::round_integral(double(x)) == x);
    assert(internal::poly::round_integral(-double(x)) == -x);
  }
  vector<long long> a(65), b(61);
  a[0] = (1LL << 52) + 1;
  b[0] = 1;
  auto c = fft_impl::convolution(a, b);
  assert(c[0] == a[0]);
  for (int i = 1; i < (int)c.size(); ++i)
    assert(c[i] == 0);
}

void check_integer_convolution() {
  mt19937_64 rng(902345);
  for (int n : {0, 1, 60, 61, 62, 63, 64, 65, 127, 128, 129, 511}) {
    for (int m : {0, 1, 60, 61, 65, 129}) {
      vector<long long> a(n), b(m);
      for (auto &x : a)
        x = int(rng() % 20001) - 10000;
      for (auto &x : b)
        x = int(rng() % 20001) - 10000;
      vector<long long> expected;
      if (n && m) {
        expected.resize(n + m - 1);
        for (int i = 0; i < n; ++i)
          for (int j = 0; j < m; ++j)
            expected[i + j] += a[i] * b[j];
      }
      assert(fft_impl::convolution(a, b) == expected);
    }
  }
  // Check every coefficient against an independent, exact NTT/CRT backend.
  for (int pattern = 0; pattern < 4; ++pattern) {
    int n = (1 << 16) + 1, m = (1 << 15) - 3;
    vector<long long> a(n), b(m);
    for (auto &x : a)
      x = int(rng() % 200001) - 100000;
    for (auto &x : b)
      x = int(rng() % 200001) - 100000;
    if (pattern == 1) {
      fill(a.begin(), a.end(), 10000);
      fill(b.begin(), b.end(), 9999);
    } else if (pattern == 2) {
      for (int i = 0; i < n; ++i)
        a[i] = (i & 1) ? -100000000 : 100000000;
      fill(b.begin(), b.end(), 1);
    } else if (pattern == 3) {
      fill(b.begin(), b.end(), 0);
      b[17] = -1;
    }
    assert(fft_impl::convolution(a, b) ==
           internal::poly::ntt::convolution_ll(a, b));
  }
}

void check_real_convolution() {
  mt19937 rng(903456);
  for (int pattern = 0; pattern < 5; ++pattern) {
    int n = 257, m = 133;
    vector<double> a(n), b(m);
    for (auto &x : a)
      x = int(rng() % 101) - 50;
    for (auto &x : b)
      x = int(rng() % 101) - 50;
    if (pattern == 1 || pattern == 2) {
      double scale = pattern == 1 ? exp2(35.0) : 1e200;
      for (auto &x : a)
        x *= scale;
      for (auto &x : b)
        x /= scale;
    } else if (pattern == 3) {
      fill(a.begin(), a.end(), 0);
    } else if (pattern == 4) {
      fill(b.begin(), b.end(), 0);
      b[65] = 1;
    }
    auto c = fft_impl::convolution(a, b);
    for (int k = 0; k < n + m - 1; ++k) {
      long double expected = 0;
      for (int i = max(0, k - m + 1); i <= min(k, n - 1); ++i)
        expected += (long double)a[i] * b[k - i];
      assert(isfinite(c[k]));
      assert(abs(c[k] - expected) < 1e-8);
    }
  }
  // Long-double real coefficients must not be narrowed to double internally.
  vector<long double> a(65), b(61);
  a[0] = 1 + numeric_limits<long double>::epsilon() * 8;
  b[0] = 1;
  auto c = fft_impl::convolution(a, b);
  assert(abs(c[0] - a[0]) <= numeric_limits<long double>::epsilon() * 2);
  for (int i = 1; i < (int)c.size(); ++i)
    assert(abs(c[i]) < numeric_limits<long double>::epsilon() * 8);
}

template <class T> void check_complex_convolution() {
  mt19937 rng(904567);
  vector<complex<T>> a(127), b(65);
  for (auto &x : a)
    x = {T(int(rng() % 31) - 15), T(int(rng() % 31) - 15)};
  for (auto &x : b)
    x = {T(int(rng() % 31) - 15), T(int(rng() % 31) - 15)};
  auto c = fft_impl::convolution_complex(a, b);
  for (int k = 0; k < (int)c.size(); ++k) {
    complex<long double> expected{};
    for (int i = max(0, k - int(b.size()) + 1); i <= min(k, int(a.size()) - 1);
         ++i)
      expected += complex<long double>(a[i]) * complex<long double>(b[k - i]);
    assert(abs(complex<long double>(c[k]) - expected) <
           100000 * numeric_limits<T>::epsilon());
  }
}

void check_even_odd_imbalance() {
  // Recover even and odd spectra without subtracting nearly equal products.
  for (int position : {0, 1, 2, 17, 64}) {
    for (bool swap_parity : {false, true}) {
      vector<double> a(65), b(61);
      a[swap_parity ? 1 : 0] = 1e16;
      a[swap_parity ? 0 : 1] = 1;
      b[position % 61] = 1;
      auto c = fft_impl::convolution(a, b);
      for (int i = 0; i < (int)c.size(); ++i) {
        int index = i - position % 61;
        double expected = index >= 0 && index < int(a.size()) ? a[index] : 0;
        // Nontrivial shifts have ordinary FFT error proportional to the large
        // coefficient. At shift zero, both packed components are exact.
        if (!position)
          assert(c[i] == expected);
        else
          assert(abs(c[i] - expected) < 8);
      }
    }
  }
}

template <class T> void check_narrow_unsigned(int n) {
  vector<T> a(n, numeric_limits<T>::max()), b(n - 1, numeric_limits<T>::max());
  auto c = fft_impl::convolution_sqrt(a, b);
  for (int k = 0; k < (int)c.size(); ++k) {
    // (-1)^2 == 1 modulo the coefficient type's power-of-two range.
    auto expected = T(min({k + 1, n - 1, 2 * n - 2 - k}));
    assert(c[k] == expected);
  }
}

void check_split_convolution() {
  using mint = static_modint<1000000007>;
  mt19937 rng(905678);
  for (int n : {61, 127, 4097, 65537}) {
    vector<mint> a(n), b(n - 1);
    for (auto &x : a)
      x = rng();
    for (auto &x : b)
      x = rng();
    assert(fft_impl::convolution_arb_mod(a, b) ==
           internal::poly::ntt::convolution_arb_mod(a, b));
    fill(a.begin(), a.end(), mint(-1));
    fill(b.begin(), b.end(), mint(-1));
    auto c = fft_impl::convolution_arb_mod(a, b);
    for (int k = 0; k < (int)c.size(); ++k)
      assert(c[k] == mint(min({k + 1, n - 1, 2 * n - 2 - k})));
  }
  // The old uncentered split exceeds the 0.5 rounding margin on this case.
  {
    using large_mint = static_modint<2147483647>;
    int n = 1 << 20, m = n - 17;
    vector<large_mint> a(n, -1), b(m, -1);
    auto c = fft_impl::convolution_arb_mod(a, b);
    for (int k = 0; k < (int)c.size(); ++k)
      assert(c[k] == large_mint(min({k + 1, m, n + m - 1 - k})));
  }
  for (int modulus : {2, 3, 17, 1000000007, 2147483647}) {
    using dynamic_mint = dynamic_modint<901>;
    dynamic_mint::set_mod(modulus);
    vector<dynamic_mint> a(65), b(61);
    for (auto &x : a)
      x = rng();
    for (auto &x : b)
      x = rng();
    auto expected = internal::poly::convolution_naive(a, b);
    assert(fft_impl::convolution_arb_mod(a, b) == expected);
  }
  {
    vector<unsigned long long> a(65), b(61);
    a[0] = b[0] = ((1ULL << 26) + 1) << 15;
    auto c = fft_impl::convolution_sqrt(a, b);
    assert(c[0] == a[0] * b[0]);
    for (int k = 1; k < (int)c.size(); ++k)
      assert(c[k] == 0);
  }
  check_narrow_unsigned<uint8_t>(257);
  check_narrow_unsigned<uint16_t>(2);
  check_narrow_unsigned<uint16_t>(60);
  check_narrow_unsigned<uint16_t>(61);
  check_narrow_unsigned<uint16_t>(65535);
  check_narrow_unsigned<uint32_t>(1025);
  {
    using u128 = unsigned __int128;
    vector<u128> a(65), b(61);
    a[0] = b[0] = (u128(1) << 41) + (1 << 15);
    auto c = fft_impl::convolution_sqrt(a, b);
    assert(c[0] == a[0] * b[0]);
    for (int k = 1; k < (int)c.size(); ++k)
      assert(c[k] == 0);
  }
  vector<unsigned long long> a(4097), b(2051);
  for (auto &x : a)
    x = rng() % 1000000000;
  for (auto &x : b)
    x = rng() % 1000000000;
  auto c = fft_impl::convolution_sqrt(a, b);
  for (int k = 0; k < (int)c.size(); ++k) {
    unsigned long long expected = 0;
    for (int i = max(0, k - int(b.size()) + 1); i <= min(k, int(a.size()) - 1);
         ++i)
      expected += a[i] * b[k - i];
    assert(c[k] ==
           expected); // Unsigned arithmetic is deliberately modulo 2^64.
  }
}

void check_public_aliasing() {
  Poly<long long, PolySetting::fft> a(65);
  for (int i = 0; i < 65; ++i)
    a[i] = i - 32;
  auto expected = internal::poly::convolution_naive<long long>(a, a);
  a *= a;
  assert(vector<long long>(a) == expected);
  Poly<uint16_t, PolySetting::fft_sqrt> b(65, uint16_t(65535));
  b *= b;
  for (int k = 0; k < (int)b.size(); ++k)
    assert(b[k] == uint16_t(min(k + 1, 129 - k)));
}

int main() {
  check_transform<float>();
  check_transform<double>();
  check_transform<long double>();
  check_rounding();
  check_integer_convolution();
  check_real_convolution();
  check_even_odd_imbalance();
  check_complex_convolution<float>();
  check_complex_convolution<double>();
  check_complex_convolution<long double>();
  check_split_convolution();
  check_public_aliasing();
  int a, b;
  cin >> a >> b;
  cout << a + b << '\n';
}
