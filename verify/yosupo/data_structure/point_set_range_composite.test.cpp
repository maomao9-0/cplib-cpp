#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

#include <iostream>
using namespace std;

#include "library/data_structure/segtree.hpp"
#include "library/math/modint.hpp"
using namespace maomao90;
using mint = static_modint<>;

struct Node {
  mint m, c;
  static Node id() { return {1, 0}; }
  Node merge(const Node &o) const { return {o.m * m, o.m * c + o.c}; }
  mint eval(int x) const { return m * x + c; }
};

using ST = SegTree<Node>;

int n, q;

int main() {
  ios::sync_with_stdio(0), cin.tie(0);
  cin >> n >> q;
  vector<Node> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i].m >> a[i].c;
  }
  ST seg(a);
  while (q--) {
    int t;
    cin >> t;
    if (t == 0) {
      int p, c, d;
      cin >> p >> c >> d;
      seg.set(p, {c, d});
    } else {
      int l, r, x;
      cin >> l >> r >> x;
      r--;
      cout << seg.qry(l, r).eval(x) << '\n';
    }
  }
  return 0;
}
