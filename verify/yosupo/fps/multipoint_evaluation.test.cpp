#define PROBLEM "https://judge.yosupo.jp/problem/multipoint_evaluation"

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
  int n, m;
  cin >> n >> m;
  poly a(n);
  vector<mint> xs(m);
  for (int i = 0; i < n; ++i)
    cin >> a[i];
  for (auto &x : xs)
    cin >> x;
  print(poly(a.multipoint_eval(xs)));
}
