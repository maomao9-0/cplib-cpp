#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include <iostream>
using namespace std;

#include "library/data_structure/segtree.hpp"
#include "library/data_structure/standard_monoids.hpp"
using namespace maomao90;

using ll = long long;
using ST = SegTree<SumMonoid<ll>>;

int n, q;

int main() {
  ios::sync_with_stdio(0), cin.tie(0);
  cin >> n >> q;
  vector<SumMonoid<ll>> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i].v;
  }
  ST seg(a);
  while (q--) {
    int t;
    cin >> t;
    if (t == 0) {
      int p, x;
      cin >> p >> x;
      seg.set(p, {seg.get(p).v + x});
    } else {
      int l, r;
      cin >> l >> r;
      cout << seg.query(l, r).v << '\n';
    }
  }
  return 0;
}
