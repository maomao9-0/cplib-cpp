#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_of_formal_power_series"

#include "library/math/poly.hpp"
using namespace maomao90;
using mint = static_modint<>;
using poly = Poly<mint>;

void print(const poly &a) {
  for (int i = 0; i < (int)a.size(); ++i)
    cout << a[i] << ' ';
  cout << '\n';
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  poly a(n);
  for (int i = 0; i < n; ++i)
    cin >> a[i];
  // sqrt() assumes the input has a formal square root; check the leading
  // nonzero coefficient first to detect the -1 case.
  int leading = 0;
  while (leading < n && a[leading] == mint(0))
    ++leading;
  bool exists = leading == n ||
                (!(leading & 1) &&
                 a[leading].pow((mint::imod() - 1) / 2) == mint(1));
  if (exists)
    print(a.sqrt());
  else
    cout << "-1\n";
}
