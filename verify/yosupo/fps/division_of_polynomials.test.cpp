#define PROBLEM "https://judge.yosupo.jp/problem/division_of_polynomials"

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
  poly a(n), b(m);
  for (int i = 0; i < n; ++i)
    cin >> a[i];
  for (int i = 0; i < m; ++i)
    cin >> b[i];
  auto [q, r] = a.divmod(b);
  cout << q.size() << ' ' << r.size() << '\n';
  print(q);
  print(r);
}
