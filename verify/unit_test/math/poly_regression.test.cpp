#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <complex>
#include <iostream>
#include <vector>
using namespace std;

#include "library/math/poly.hpp"
using namespace maomao90;

// Check both the public concept and the class constraint, including failures
// that must be substitution-friendly rather than hard template errors.
template <class T, PolySetting setting>
concept HasPoly = requires { typename Poly<T, setting>; };

template <class T, bool ntt_ok, bool fft_ok, bool split_ok, bool complex_ok>
constexpr bool backend_matrix() {
  static_assert(ValidPolySetting<T, PolySetting::ntt> == ntt_ok);
  static_assert(ValidPolySetting<T, PolySetting::fft> == fft_ok);
  static_assert(ValidPolySetting<T, PolySetting::fft_sqrt> == split_ok);
  static_assert(ValidPolySetting<T, PolySetting::fft_complex> == complex_ok);
  static_assert(HasPoly<T, PolySetting::ntt> == ntt_ok);
  static_assert(HasPoly<T, PolySetting::fft> == fft_ok);
  static_assert(HasPoly<T, PolySetting::fft_sqrt> == split_ok);
  static_assert(HasPoly<T, PolySetting::fft_complex> == complex_ok);
  return true;
}

struct NotComplex {
  using value_type = double;
};
static_assert(backend_matrix<static_modint<>, true, false, true, false>());
static_assert(backend_matrix<dynamic_modint<>, true, false, true, false>());
static_assert(backend_matrix<long long, true, true, false, false>());
static_assert(backend_matrix<unsigned long long, true, true, true, false>());
static_assert(backend_matrix<double, false, true, false, false>());
static_assert(backend_matrix<complex<double>, false, false, false, true>());
static_assert(
    backend_matrix<complex<long double>, false, false, false, true>());
static_assert(backend_matrix<NotComplex, false, false, false, false>());
static_assert(backend_matrix<void, false, false, false, false>());
static_assert(!HasPoly<int, static_cast<PolySetting>(-1)>);
static_assert(!is_convertible_v<PolySetting, int>);
static_assert(
    same_as<Poly<static_modint<>>, Poly<static_modint<>, PolySetting::ntt>>);

void test_negative_fft_convolution() {
  vector<long long> a(61), b(61);
  a[0] = -1;
  b[0] = 1;

  auto c = internal::poly::fft::convolution(a, b);
  assert(c.size() == 121);
  assert(c[0] == -1);
  for (int i = 1; i < (int)c.size(); i++) {
    assert(c[i] == 0);
  }

  Poly<long long, PolySetting::fft> p(61), q(61);
  p[0] = -1;
  q[0] = 1;
  auto r = p * q;
  assert(r.degree() == 120);
  assert(r[0] == -1);
  for (int i = 1; i <= r.degree(); i++) {
    assert(r[i] == 0);
  }
}

void test_unsigned_fft_sqrt_convolution() {
  vector<unsigned long long> a(61), b(61);
  a[0] = 123456789;
  a[1] = 987654321;
  b[0] = 111111111;
  b[1] = 222222222;

  auto c = internal::poly::fft::convolution_sqrt(a, b);
  assert(c.size() == 121);
  assert(c[0] == a[0] * b[0]);
  assert(c[1] == a[0] * b[1] + a[1] * b[0]);
  assert(c[2] == a[1] * b[1]);
  for (int i = 3; i < (int)c.size(); i++) {
    assert(c[i] == 0);
  }
}

void test_fft_complex_poly() {
  using cd = complex<double>;
  Poly<cd, PolySetting::fft_complex> a(2), b(2);
  a[0] = cd(1.0, 2.0);
  a[1] = cd(-3.0, 1.0);
  b[0] = cd(2.0, -1.0);
  b[1] = cd(4.0, 3.0);

  auto c = a * b;
  assert(c.degree() == 2);
  assert(abs(c[0] - a[0] * b[0]) < 1e-9);
  assert(abs(c[1] - (a[0] * b[1] + a[1] * b[0])) < 1e-9);
  assert(abs(c[2] - a[1] * b[1]) < 1e-9);
}

int main() {
  test_negative_fft_convolution();
  test_unsigned_fft_sqrt_convolution();
  test_fft_complex_poly();

  int a, b;
  cin >> a >> b;
  cout << a + b << '\n';
  return 0;
}
