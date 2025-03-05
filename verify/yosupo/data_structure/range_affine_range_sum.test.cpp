#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

#include <iostream>
using namespace std;

#include "library/data_structure/lazy_segtree.hpp"
#include "library/math/modint.hpp"
using namespace maomao90;
using mint = static_modint<>;

struct Node {
  mint sm;
  static Node id() { return {0}; }
  Node merge(const Node &o) const { return {sm + o.sm}; }
};

struct Lazy {
  mint m, c;
  static Lazy id() { return {1, 0}; }
  Lazy merge(const Lazy &o) const { return {m * o.m, m * o.c + c}; }
  Node apply(const Node &o, int len) const { return {o.sm * m + c * len}; }
};

using ST = LazySegTree<Node, Lazy>;

int n, q;

int main() {
  ios::sync_with_stdio(0), cin.tie(0);
  cin >> n >> q;
  vector<Node> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i].sm;
  }
  ST seg(a);
  while (q--) {
    int t;
    cin >> t;
    if (t == 0) {
      int l, r, b, c;
      cin >> l >> r >> b >> c;
      r--;
      seg.upd(l, r, {b, c});
    } else {
      int i;
      cin >> i;
      cout << seg.get(i).sm << '\n';
      /*
      int l, r;
      cin >> l >> r;
      r--;
      cout << seg.qry(l, r).sm << '\n';
      */
    }
  }
  return 0;
}
