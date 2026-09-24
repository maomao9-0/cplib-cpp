#define PROBLEM                                                                \
  "https://judge.yosupo.jp/problem/pow_of_formal_power_series_sparse"

#include "library/math/poly.hpp"
using namespace maomao90;
using mint = static_modint<>;
using poly = Poly<mint>;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, terms;
  long long exponent;
  cin >> n >> terms >> exponent;
  poly a(n);
  for (int j = 0; j < terms; ++j) {
    int i;
    cin >> i;
    cin >> a[i];
  }
  auto result = a.sparse_pow(exponent);
  for (int i = 0; i < n; ++i)
    cout << result[i] << ' ';
  cout << '\n';
}
