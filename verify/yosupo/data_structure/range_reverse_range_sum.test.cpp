#define PROBLEM "https://judge.yosupo.jp/problem/range_reverse_range_sum"

#include <iostream>
using namespace std;

#include "library/data_structure/splaytree.hpp"
#include "library/data_structure/standard_monoids.hpp"
using namespace maomao90;

using ll = long long;
using Splay = SplayTree<SumMonoid<ll>, NoLazy<SumMonoid<ll>>>;

int n, q;

int main() {
  ios::sync_with_stdio(0), cin.tie(0);
  cin >> n >> q;
  vector<SumMonoid<ll>> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i].v;
  }
  Splay splay(a);
  while (q--) {
    int t;
    cin >> t;
    if (t == 0) {
      int l, r;
      cin >> l >> r;
      splay.reverse(l, r);
    } else {
      int l, r;
      cin >> l >> r;
      cout << splay.query(l, r).v << '\n';
    }
  }
  return 0;
}
