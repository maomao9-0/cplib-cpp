#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_taylor_shift"

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
  mint c;
  cin >> n >> c;
  poly a(n);
  for (int i = 0; i < n; ++i)
    cin >> a[i];
  print(a.taylor_shift(c));
}
