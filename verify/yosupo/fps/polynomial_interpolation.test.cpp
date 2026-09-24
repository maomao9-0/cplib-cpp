#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_interpolation"

#include "library/math/poly.hpp"
using namespace maomao90;
using mint = static_modint<>;
using poly = Poly<mint>;

void print(const poly &a) {
  for (int i = 0; i < a.size(); ++i)
    cout << a[i] << ' ';
  cout << '\n';
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vector<mint> xs(n), ys(n);
  for (auto &x : xs)
    cin >> x;
  for (auto &y : ys)
    cin >> y;
  print(poly::interpolate(xs, ys));
}
