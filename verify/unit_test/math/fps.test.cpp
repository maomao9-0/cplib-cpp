#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include "library/math/poly.hpp"
#include <cassert>
#include <climits>
#include <cstdlib>
#include <random>
#include <string>
using namespace maomao90;

template <class Mint> void check_field() {
  using P = Poly<Mint>;
  std::mt19937 rng(1729);
  auto random_poly = [&](int n) {
    P p(n);
    for (int i = 0; i < n; ++i)
      p[i] = rng() % Mint::imod();
    return p;
  };
  auto naive_product = [](const P &a, const P &b, int n) {
    P c(n);
    for (int i = 0; i < a.size(); ++i)
      for (int j = 0; j < b.size() && i + j < n; ++j)
        c[i + j] += a[i] * b[j];
    return c;
  };
  for (int n :
       {0, 1, 2, 3, 7, 16, 31, 60, 61, 63, 64, 65, 127, 128, 129, 257}) {
    if (n >= Mint::imod())
      continue;
    for (int trial = 0; trial < 5; ++trial) {
      P a = random_poly(n), b = random_poly(n / 2 + 1);
      assert((a * b).truncated(n) == naive_product(a, b, n));
      assert(a.integral().differ() == a);
      assert(a.pre(n + 10) == a);
      assert(a.truncated(n + 10).pre(n) == a);
      if (!n) {
        assert(a.inv().empty());
        assert(a.log().empty());
        assert(a.exp().empty());
        continue;
      }
      a[0] = 1;
      P identity(n);
      identity[0] = 1;
      P naive_inverse(n);
      naive_inverse[0] = 1;
      for (int i = 1; i < n; ++i)
        for (int j = 1; j <= i; ++j)
          naive_inverse[i] -= a[j] * naive_inverse[i - j];
      assert(a.inv() == naive_inverse);
      assert(naive_product(a, a.inv(), n) == identity);
      P logarithm(n),
          derivative = naive_product(a.differ(), naive_inverse, n - 1);
      for (int i = 1; i < n; ++i)
        logarithm[i] = derivative[i - 1] / Mint(i);
      assert(a.log() == logarithm);
      a[0] = 0;
      P exponential(n);
      exponential[0] = 1;
      for (int i = 1; i < n; ++i) {
        for (int j = 1; j <= i; ++j)
          exponential[i] += Mint(j) * a[j] * exponential[i - j];
        exponential[i] /= Mint(i);
      }
      assert(a.exp() == exponential);
      assert(a.exp().log() == a);
      for (int k : {0, 1, 2, 5}) {
        P expected = identity;
        for (int j = 0; j < k; ++j)
          expected = naive_product(expected, a, n);
        assert(a.pow(k) == expected);
        assert(a.sparse_pow(k) == expected);
      }
      a[0] = 3;
      assert(a.pow(7) == a.sparse_pow(7));
      assert(a.pow(LLONG_MAX) == a.sparse_pow(LLONG_MAX));
      auto root = naive_product(a, a, n).sqrt();
      assert(root && naive_product(*root, *root, n) == naive_product(a, a, n));
      if (n > 3) {
        a[0] = a[1] = 0;
        auto square = naive_product(a, a, n);
        root = square.sqrt();
        assert(root && naive_product(*root, *root, n) == square);
      }
      Mint shift = 7;
      P shifted(n), basis{1};
      for (int i = 0; i < n; ++i) {
        shifted += basis * a[i];
        basis.multiply(1, shift);
      }
      assert(a.taylor_shift(shift) == shifted);
      auto original = a;
      a.multiply(3, 0);
      a.divide(3, 0);
      assert(a == original);
      a.multiply(3, 7);
      a.divide(3, 7);
      assert(a == original);
      b[b.size() - 1] = 1;
      auto [q, r] = original.divmod(b);
      assert((q * b + r).truncated(n) == original);
      assert(r.size() < b.size());
      vector<Mint> xs(n), ys(n);
      for (int i = 0; i < n; ++i) {
        xs[i] = i;
        ys[i] = original.eval(xs[i]);
      }
      assert(original.multipoint_eval(xs) == ys);
      assert(P::interpolate(xs, ys) == original);
      if (n > 1) {
        xs[1] = xs[0];
        ys[1] = ys[0];
        assert(original.multipoint_eval(xs) == ys);
      }
    }
  }
  assert(P(0).degree() == -1);
  assert((P(0) * P{1}).empty());
  assert((P{1} * P(0)).empty());
  assert(P(0).exp(4) == P({1, 0, 0, 0}));
  assert(P{1}.inv(4) == P({1, 0, 0, 0}));
  assert(P{1}.log(4) == P(4));
  assert(P(0).integral() == P{0});
  assert(!P({0, 1}).sqrt(4));
  assert(P({0, 0, 0, 1}).sqrt(2) == P(2));
  assert(P({0, 1}).pow(LLONG_MAX, 4) == P(4));
  assert(P({0, 1}).sparse_pow(LLONG_MAX, 4) == P(4));
  P zeros{0, 0};
  zeros.shrink();
  assert(zeros.empty());
  auto division = P({1, 2, 3, 0}).divmod(P({1, 0}));
  assert(division.first == P({1, 2, 3}) && division.second.empty());
  P alias{1, 2, 3};
  alias.divide_in_place(alias, 5);
  assert(alias == P({1, 0, 0, 0, 0}));
  for (int trial = 0; trial < 50; ++trial) {
    P numerator = random_poly(5 + trial),
      denominator = random_poly(1 + trial % 13);
    denominator[0] = 1;
    P series(150);
    for (int i = 0; i < series.size(); ++i) {
      if (i < numerator.size())
        series[i] = numerator[i];
      for (int j = 1; j < denominator.size() && j <= i; ++j)
        series[i] -= denominator[j] * series[i - j];
    }
    for (int k : {0, 1, 2, 17, 149})
      assert(P::bostan_mori(k, numerator, denominator) == series[k]);
  }
  assert(P::bostan_mori(LLONG_MAX, P{1}, P{1, -1}) == Mint(1));
  P periodic_denominator(129);
  periodic_denominator[0] = 1;
  periodic_denominator[128] = -1;
  assert(P::bostan_mori(LLONG_MAX, P{1}, periodic_denominator) == Mint(0));
  assert(P::bostan_mori(LLONG_MAX - 127, P{1}, periodic_denominator) ==
         Mint(1));
  assert(P::interpolate({}, {}).empty());
  assert(P{1}.multipoint_eval({}).empty());
  assert(P(0).multipoint_eval({1, 2}) == vector<Mint>({0, 0}));
  assert(P::interpolate({1, 2}, {0, 0}) == P(2));
  for (int denominator_size : {65, 100, 128, 129}) {
    P numerator = random_poly(denominator_size + 17);
    P denominator = random_poly(denominator_size);
    denominator[0] = 1;
    P series(301);
    for (int i = 0; i < series.size(); ++i) {
      if (i < numerator.size())
        series[i] = numerator[i];
      for (int j = 1; j < denominator.size() && j <= i; ++j)
        series[i] -= denominator[j] * series[i - j];
    }
    for (int k : {0, 1, 2, 17, 149, 300})
      assert(P::bostan_mori(k, numerator, denominator) == series[k]);
  }
  Mint nonresidue = 2;
  while (nonresidue.pow((Mint::imod() - 1) / 2) == Mint(1))
    ++nonresidue;
  assert(!P{nonresidue}.sqrt(4));
  // Explicit precisions both shorter and longer than the stored input.
  P short_series{1, 2, 3};
  assert(naive_product(short_series, short_series.inv(35), 35) ==
         P{1}.truncated(35));
  assert(short_series.inv(1) == P{1});
  P short_exponent{0, 2, 3};
  int precision = min(35LL, (long long)Mint::imod());
  assert(short_exponent.exp(precision).log() ==
         short_exponent.truncated(precision));
  vector<Mint> recurrence{2, 3, 4}, initial{5, 6, 7}, sequence = initial;
  for (int i = 3; i < 100; ++i)
    sequence.push_back(2 * sequence[i - 1] + 3 * sequence[i - 2] +
                       4 * sequence[i - 3]);
  for (int i = 0; i < 100; ++i)
    assert(P::linear_recurrence(initial, recurrence, i) == sequence[i]);
}

void check_convolution_dispatch() {
  using N = static_modint<>;
  for (int n = 1; n <= 1024; n *= 2) {
    vector<N> a(n);
    for (int i = 0; i < n; ++i)
      a[i] = i * i + 13;
    vector<N> doubled = a;
    doubled.resize(2 * n);
    internal::poly::ntt::butterfly(doubled);
    internal::poly::ntt::butterfly(a);
    internal::poly::ntt::ntt_doubling(a);
    assert(a == doubled);
  }
  using M = dynamic_modint<2718>;
  for (int modulus : {998244353, 1000000007, 1000}) {
    M::set_mod(modulus);
    vector<M> a(80), b(90);
    for (int i = 0; i < 80; ++i)
      a[i] = i * i + 1;
    for (int i = 0; i < 90; ++i)
      b[i] = i * i - 3;
    auto result = Poly<M>(a) * Poly<M>(b);
    assert(result.coefficients() == internal::poly::convolution_naive(a, b));
  }
  using C = static_modint<1000>;
  vector<C> a(70, 999), b(75, 999);
  assert((Poly<C>(a) * Poly<C>(b)).coefficients() ==
         internal::poly::convolution_naive(a, b));
}

template <class Wide, PolySetting setting> void check_unsupported_modulus() {
  using P = Poly<Wide, setting>;
  // Empty and small inputs must not silently bypass the modulus restriction.
  for (int n : {0, 1, 65}) {
    P a(vector<Wide>(n, 2)), b(vector<Wide>(n, 3));
    auto original = a;
    bool rejected = false;
    try {
      a *= b;
    } catch (const std::invalid_argument &error) {
      rejected = std::string(error.what()).find("moduli greater than 2^31-1") !=
                 std::string::npos;
    }
    if (!rejected || a != original)
      std::abort();
    rejected = false;
    try {
      auto result = a * b;
      (void)result;
    } catch (const std::invalid_argument &) {
      rejected = true;
    }
    if (!rejected)
      std::abort();
  }
}

int main() {
  check_field<static_modint<>>();
  check_field<static_modint<1000000007>>();
  check_field<static_modint<17>>();
  check_convolution_dispatch();
  check_unsupported_modulus<static_modint<2147483648LL>, PolySetting::ntt>();
  check_unsupported_modulus<static_modint<2305843009213693951LL>,
                            PolySetting::ntt>();
  check_unsupported_modulus<static_modint<2147483648LL>,
                            PolySetting::fft_sqrt>();
  check_unsupported_modulus<static_modint<2305843009213693951LL>,
                            PolySetting::fft_sqrt>();
  long long a, b;
  cin >> a >> b;
  cout << a + b << '\n';
}
