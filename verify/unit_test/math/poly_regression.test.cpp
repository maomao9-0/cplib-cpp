#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <complex>
#include <iostream>
#include <vector>
using namespace std;

#include "library/math/poly.hpp"
using namespace maomao90;

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
