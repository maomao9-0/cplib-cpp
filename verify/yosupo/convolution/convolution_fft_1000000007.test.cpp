#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_1000000007"

#include "library/math/modint.hpp"
#include "library/math/poly.hpp"
using namespace maomao90;
using mint = static_modint<1000000007>;
using poly = Poly<mint, PolySetting::fft_sqrt>;

int n, m;

int main() {
  ios::sync_with_stdio(0), cin.tie(0);
  cin >> n >> m;
  poly a(n), b(m);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  for (int i = 0; i < m; i++) {
    cin >> b[i];
  }
  poly c = a * b;
  for (int i = 0; i <= c.degree(); i++) {
    cout << c[i] << ' ';
  }
  cout << '\n';
  return 0;
}
