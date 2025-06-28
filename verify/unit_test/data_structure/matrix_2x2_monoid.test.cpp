#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <random>
using namespace std;

#include "library/data_structure/lazy_segtree.hpp"
#include "library/data_structure/segtree.hpp"
#include "library/data_structure/splaytree.hpp"
#include "library/math/modint.hpp"
using namespace maomao90;
using mint2 = static_modint<2>;
using mint3 = static_modint<3>;
using mint4 = static_modint<4>;
using mint5 = static_modint<5>;

mt19937 rng(chrono::high_resolution_clock::now().time_since_epoch().count());

template <typename T> struct MatSum {
  T a, b, c, d;
  static MatSum id() { return {0, 0, 0, 0}; }
  MatSum merge(const MatSum &o) const {
    return {a + o.a, b + o.b, c + o.c, d + o.d};
  }

  bool operator==(const MatSum &o) const = default;

  static MatSum random_gen() { return {rng(), rng(), rng(), rng()}; }
};
template <typename T> struct LazyMult {
  T a, b, c, d;
  static LazyMult id() { return {1, 0, 0, 1}; }
  LazyMult merge(const LazyMult &o) const {
    return {
        a * o.a + b * o.c,
        a * o.b + b * o.d,
        c * o.a + d * o.c,
        c * o.b + d * o.d,
    };
  }
  MatSum<T> apply(const MatSum<T> &o, const int len) const {
    return {
        a * o.a + b * o.c,
        a * o.b + b * o.d,
        c * o.a + d * o.c,
        c * o.b + d * o.d,
    };
  }

  bool operator==(const LazyMult &o) const = default;

  static LazyMult random_gen() { return {rng(), rng(), rng(), rng()}; }
};

template <typename T> struct MatMult {
  T a, b, c, d;
  static MatMult id() { return {1, 0, 0, 1}; }
  MatMult merge(const MatMult &o) const {
    return {
        a * o.a + b * o.c,
        a * o.b + b * o.d,
        c * o.a + d * o.c,
        c * o.b + d * o.d,
    };
  }

  bool operator==(const MatMult &o) const = default;

  static MatMult random_gen() { return {rng(), rng(), rng(), rng()}; }
};
template <typename T> struct LazyConstant {
  T x;
  static LazyConstant id() { return {1}; }
  LazyConstant merge(const LazyConstant &o) const { return {x * o.x}; }
  MatMult<T> apply(const MatMult<T> &o, const int len) const {
    return {
        o.a * x.pow(len),
        o.b * x.pow(len),
        o.c * x.pow(len),
        o.d * x.pow(len),
    };
  }

  bool operator==(const LazyConstant &o) const = default;

  static LazyConstant random_gen() {
    T v;
    do {
      v = rng();
    } while (v == 0 && rng() % 10 != 0);
    return {v};
  }
};

pair<int, int> gen_range(int n) {
  if (n == 0) {
    return {0, 0};
  }
  int l, r;
  do {
    int o = rng() % 10;
    l = rng() % (n + 1);
    r = rng() % (n + 1);
    if (o == 0) {
      l = 0;
    } else if (o == 1) {
      r = n;
    } else if (o == 2) {
      r = l + rng() % ((n + 9) / 10);
    } else if (o <= 4) {
      l = rng() % ((n + 9) / 10);
    } else if (o <= 6) {
      r = n - rng() % ((n + 9) / 10);
    }
  } while (!(0 <= l && l <= r && r <= n));
  return {l, r};
}

template <typename T, typename L>
void brute_update(vector<T> &a, int l, int r, L upd) {
  // cerr << "BEFORE: \n";
  // for (T &x : a) {
  //   cerr << ' ' << x.a << ' ' << x.b << ' ' << x.c << ' ' << x.d << '\n';
  // }
  // if constexpr (requires { upd.x; }) {
  //   cerr << l << ' ' << r << ": " << upd.x << '\n';
  // } else {
  //   cerr << l << ' ' << r << ": " << upd.a << ' ' << upd.b << ' ' << upd.c
  //        << ' ' << upd.d << '\n';
  // }
  for (int i = l; i < r; i++) {
    a[i] = upd.apply(a[i], 1);
  }
  // cerr << "AFTER: \n";
  // for (T &x : a) {
  //   cerr << ' ' << x.a << ' ' << x.b << ' ' << x.c << ' ' << x.d << '\n';
  // }
}
template <typename T> T brute_query(vector<T> &a, int l, int r) {
  // cerr << "? " << l << ' ' << r << '\n';
  T res = T::id();
  for (int i = l; i < r; i++) {
    res = res.merge(a[i]);
  }
  // cerr << ' ' << res.a << ' ' << res.b << ' ' << res.c << ' ' << res.d <<
  // '\n';
  return res;
}
template <typename T, class P>
int brute_max_right(vector<T> &a, int l, P pred) {
  T cur = T::id();
  bool prv_pred = true;
  int res = l;
  for (int r = l; r < (int)a.size(); r++) {
    cur = cur.merge(a[r]);
    if (pred(cur)) {
      assert(prv_pred);
      res = r + 1;
    } else {
      prv_pred = false;
    }
  }
  return res;
}
template <typename T, class P> int brute_min_left(vector<T> &a, int r, P pred) {
  T cur = T::id();
  bool prv_pred = true;
  int res = r;
  for (int l = r - 1; l >= 0; l--) {
    cur = a[l].merge(cur);
    if (pred(cur)) {
      assert(prv_pred);
      res = l;
    } else {
      prv_pred = false;
    }
  }
  return res;
}

template <typename T, typename DS>
void test_point_update_range_query(int testcases, int n_lim, int queries,
                                   bool binary_search) {
  while (testcases--) {
    int n = rng() % n_lim + 1;
    vector<T> a(n);
    for (T &x : a) {
      x = T::random_gen();
    }
    DS ds;
    if (rng() % 2) {
      ds = DS(a);
    } else {
      ds = DS(n);
      for (int i = 0; i < n; i++) {
        ds.set(i, a[i]);
      }
    }
    int q = queries;
    while (q--) {
      int o = rng() % (2 + binary_search);
      if (o == 0) {
        int p = rng() % n;
        T v = T::random_gen();
        ds.set(p, v);
        a[p] = v;
      } else if (o == 1) {
        auto [l, r] = gen_range(n);
        assert((ds.query(l, r) == brute_query<T>(a, l, r)));
      } else {
        int p = rng() % (n + 1);
        if (rng() % 2) {
          auto pred = [&](T x) { return x.a != 0 || x.b != 0; };
          assert((ds.max_right(p, pred) == brute_max_right<T>(a, p, pred)));
        } else {
          auto pred = [&](T x) { return x.a != 0 || x.c != 0; };
          assert((ds.min_left(p, pred) == brute_min_left<T>(a, p, pred)));
        }
      }
    }
  }
}

template <typename T, typename L, typename DS>
void test_range_update_range_query(int testcases, int n_lim, int queries,
                                   bool binary_search) {
  while (testcases--) {
    int n = rng() % n_lim + 1;
    vector<T> a(n);
    for (T &x : a) {
      x = T::random_gen();
    }
    DS ds;
    if (rng() % 2) {
      ds = DS(a);
    } else {
      ds = DS(n);
      for (int i = 0; i < n; i++) {
        ds.set(i, a[i]);
      }
    }
    int q = queries;
    while (q--) {
      int o = rng() % (2 + binary_search);
      if (o == 0) {
        auto [l, r] = gen_range(n);
        L upd = L::random_gen();
        ds.update(l, r, upd);
        brute_update<T, L>(a, l, r, upd);
      } else if (o == 1) {
        auto [l, r] = gen_range(n);
        assert((ds.query(l, r) == brute_query<T>(a, l, r)));
      } else {
        int p = rng() % (n + 1);
        if (rng() % 2) {
          auto pred = [&](T x) { return x.a != 0 || x.b != 0; };
          assert((ds.max_right(p, pred) == brute_max_right<T>(a, p, pred)));
        } else {
          auto pred = [&](T x) { return x.a != 0 || x.c != 0; };
          assert((ds.min_left(p, pred) == brute_min_left<T>(a, p, pred)));
        }
      }
    }
  }
}

template <typename T> void brute_reverse(vector<T> &a, int l, int r) {
  reverse(a.begin() + l, a.begin() + r);
}
template <typename T> void brute_insert(vector<T> &a, int k, T x) {
  a.insert(a.begin() + k, x);
}
template <typename T> void brute_erase(vector<T> &a, int k) {
  a.erase(a.begin() + k);
}
template <typename T>
void brute_split_merge(vector<T> &a1, int l1, int r1, vector<T> &a2, int l2,
                       int r2) {
  vector<T> res1, res2;
  for (int i = 0; i < l1; i++) {
    res1.push_back(a1[i]);
  }
  for (int i = 0; i < l2; i++) {
    res2.push_back(a2[i]);
  }
  for (int i = l1; i < r1; i++) {
    res2.push_back(a1[i]);
  }
  for (int i = l2; i < r2; i++) {
    res1.push_back(a2[i]);
  }
  for (int i = r1; i < (int)a1.size(); i++) {
    res1.push_back(a1[i]);
  }
  for (int i = r2; i < (int)a2.size(); i++) {
    res2.push_back(a2[i]);
  }
  a1 = res1;
  a2 = res2;
}

template <typename T, typename L, typename DS>
void test_bbst(int testcases, int n_lim, int queries, bool binary_search) {
  while (testcases--) {
    int n = rng() % n_lim + 1;
    int layer = rng() % 4 + 2;
    vector<vector<T>> a(layer, vector<T>(n));
    for (int k = 0; k < layer; k++) {
      for (T &x : a[k]) {
        x = T::random_gen();
      }
    }
    vector<DS> ds(layer);
    if (rng() % 2) {
      for (int k = 0; k < layer; k++) {
        ds[k] = DS(a[k]);
      }
    } else {
      for (int k = 0; k < layer; k++) {
        ds[k] = DS(n);
        for (int i = 0; i < n; i++) {
          ds[k].set(i, a[k][i]);
        }
      }
    }
    int q = queries;
    while (q--) {
      int o = rng() % (6 + binary_search);
      bool all_empty = 1;
      for (int k = 0; k < layer; k++) {
        all_empty &= a[k].empty();
      }
      if (all_empty) {
        o = 3;
      }
      if (o == 0) {
        // update
        int k = rng() % layer;
        auto [l, r] = gen_range(a[k].size());
        L upd = L::random_gen();
        ds[k].update(l, r, upd);
        brute_update<T, L>(a[k], l, r, upd);
      } else if (o == 1) {
        // query
        int k = rng() % layer;
        auto [l, r] = gen_range(a[k].size());
        assert((ds[k].query(l, r) == brute_query<T>(a[k], l, r)));
      } else if (o == 2) {
        // reverse
        int k = rng() % layer;
        auto [l, r] = gen_range(a[k].size());
        ds[k].reverse(l, r);
        brute_reverse<T>(a[k], l, r);
      } else if (o == 3) {
        // insert
        int k = rng() % layer;
        int p = rng() % (a[k].size() + 1);
        T v = T::random_gen();
        ds[k].insert(p, v);
        brute_insert<T>(a[k], p, v);
      } else if (o == 4) {
        // erase
        int k = rng() % layer;
        while (a[k].empty()) {
          k = rng() % layer;
        }
        int p = rng() % a[k].size();
        ds[k].erase(p);
        brute_erase<T>(a[k], p);
      } else if (o == 5) {
        // split and merge
        int k[2], l[2], r[2];
        do {
          k[0] = rng() % layer;
          k[1] = rng() % layer;
        } while (k[0] == k[1]);
        DS ds_m[2], ds_r[2];
        for (int b : {0, 1}) {
          tie(l[b], r[b]) = gen_range(a[k[b]].size());
          int o_ = rng() % 3;
          if (o_ == 0) {
            tie(ds_m[b], ds_r[b]) = ds[k[b]].split(l[b], r[b]);
          } else if (o_ == 1) {
            ds_r[b] = ds[k[b]].split(r[b]);
            ds_m[b] = ds[k[b]].split(l[b]);
          } else {
            ds_m[b] = ds[k[b]].split(l[b]);
            ds_r[b] = ds_m[b].split(r[b] - l[b]);
          }
        }
        for (int b : {0, 1}) {
          int o_ = rng() % 3;
          if (o_ == 0) {
            ds[k[b]].merge(ds_m[b ^ 1], ds_r[b]);
          } else if (o_ == 1) {
            ds[k[b]].merge(ds_m[b ^ 1]);
            ds[k[b]].merge(ds_r[b]);
          } else {
            ds_m[b ^ 1].merge(ds_r[b]);
            ds[k[b]].merge(ds_m[b ^ 1]);
          }
        }
        brute_split_merge<T>(a[k[0]], l[0], r[0], a[k[1]], l[1], r[1]);
      } else {
        // binary search
        int k = rng() % layer;
        int p = rng() % (a[k].size() + 1);
        if (rng() % 2) {
          auto pred = [&](T x) { return x.a != 0 || x.b != 0; };
          assert(
              (ds[k].max_right(p, pred) == brute_max_right<T>(a[k], p, pred)));
        } else {
          auto pred = [&](T x) { return x.a != 0 || x.c != 0; };
          assert((ds[k].min_left(p, pred) == brute_min_left<T>(a[k], p, pred)));
        }
      }
    }
  }
}

template <typename Monoid, typename Lazy, bool commutative>
void test_wrapper(int testcases, int n_lim, int queries, bool binary_search) {
  test_point_update_range_query<Monoid, SegTree<Monoid>>(
      testcases, n_lim, queries, binary_search);
  test_range_update_range_query<Monoid, Lazy, LazySegTree<Monoid, Lazy>>(
      testcases, n_lim, queries, binary_search);
  test_range_update_range_query<Monoid, Lazy, SplayTree<Monoid, Lazy>>(
      testcases, n_lim, queries, binary_search);

  test_bbst<Monoid, Lazy, SplayTree<Monoid, Lazy, !commutative>>(
      testcases, n_lim, queries, binary_search);
}

// use MatSum and LazyMult
template <typename T>
void test_non_commutative_update_wrapper(int testcases, int n_lim,
                                         int queries) {
  test_wrapper<MatSum<T>, LazyMult<T>, true>(testcases, n_lim, queries, false);
}
// Use MatMult and LazyConstant
template <typename T>
void test_non_commutative_query_wrapper(int testcases, int n_lim, int queries) {
  test_wrapper<MatMult<T>, LazyConstant<T>, false>(testcases, n_lim, queries,
                                                   true);
}

int main() {
  for (auto [testcases, n_lim, queries] : vector<tuple<int, int, int>>(
           {{100, 10, 10000}, {10, 100, 1000}, {5, 1000, 1000}})) {
    test_non_commutative_update_wrapper<mint2>(testcases, n_lim, queries);
    test_non_commutative_update_wrapper<mint3>(testcases, n_lim, queries);
    test_non_commutative_update_wrapper<mint4>(testcases, n_lim, queries);
    test_non_commutative_update_wrapper<mint5>(testcases, n_lim, queries);

    test_non_commutative_query_wrapper<mint2>(testcases, n_lim, queries);
    test_non_commutative_query_wrapper<mint3>(testcases, n_lim, queries);
    test_non_commutative_query_wrapper<mint4>(testcases, n_lim, queries);
    test_non_commutative_query_wrapper<mint5>(testcases, n_lim, queries);
  }

  int a, b;
  cin >> a >> b;
  cout << a + b << '\n';
}
