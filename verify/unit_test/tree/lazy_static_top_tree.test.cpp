#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <set>
#include <tuple>
#include <vector>
using namespace std;

#include "library/tree/lazy_static_top_tree.hpp"
#include "library/tree/static_top_tree.hpp"
using namespace maomao90;

struct Point;

struct Path {
  long long sum;
  int sz;

  Path(long long x) : sum(x), sz(1) {}
  Path(long long sum, int sz) : sum(sum), sz(sz) {}

  static Path id() { return {0, 0}; }
  Path merge(const Path &o) const { return {sum + o.sum, sz + o.sz}; }
  Point add_edge() const;
};

struct Point {
  long long sum;
  int sz;

  static Point id() { return {0, 0}; }
  Point merge(const Point &o) const { return {sum + o.sum, sz + o.sz}; }
  Path add_vertex(long long x) const { return {sum + x, sz + 1}; }
};

Point Path::add_edge() const { return {sum, sz}; }

struct Lazy {
  long long delta;

  static Lazy id() { return {0}; }
  Lazy merge(const Lazy &o) const { return {delta + o.delta}; }
  Path apply(const Path &p) const { return {p.sum + delta * p.sz, p.sz}; }
  Point apply(const Point &p) const { return {p.sum + delta * p.sz, p.sz}; }
  long long apply_vertex(long long x) const { return x + delta; }
};

mt19937 rng(123456789);

enum TreeShape {
  UniformParent,
  RootBiasedParent,
  RecentBiasedParent,
  Prufer,
  BinaryLike,
};

vector<int> build_parent(const vector<vector<int>> &adj) {
  int n = adj.size();
  vector<int> parent(n, -1);
  vector<int> order = {0};
  for (int i = 0; i < (int)order.size(); i++) {
    int u = order[i];
    for (int v : adj[u]) {
      if (v == parent[u]) {
        continue;
      }
      parent[v] = u;
      order.push_back(v);
    }
  }
  return parent;
}

long long brute_query(const vector<long long> &a, const vector<int> &parent,
                      int root) {
  long long res = 0;
  for (int u = 0; u < (int)a.size(); u++) {
    int v = u;
    while (v != -1 && v != root) {
      v = parent[v];
    }
    if (v == root) {
      res += a[u];
    }
  }
  return res;
}

void brute_update_subtree(vector<long long> &a, const vector<int> &parent,
                          int root, long long delta) {
  for (int u = 0; u < (int)a.size(); u++) {
    int v = u;
    while (v != -1 && v != root) {
      v = parent[v];
    }
    if (v == root) {
      a[u] += delta;
    }
  }
}

void add_edge(vector<vector<int>> &adj, int u, int v) {
  adj[u].push_back(v);
  adj[v].push_back(u);
}

vector<vector<int>> gen_tree_from_parent(int n, auto gen_parent) {
  vector<vector<int>> adj(n);
  for (int u = 1; u < n; u++) {
    add_edge(adj, u, gen_parent(u));
  }
  return adj;
}

vector<vector<int>> gen_prufer_tree(int n) {
  if (n <= 2) {
    return gen_tree_from_parent(n, [](int) { return 0; });
  }
  vector<int> code(n - 2);
  vector<int> deg(n, 1);
  for (int &x : code) {
    x = rng() % n;
    deg[x]++;
  }
  set<int> leaves;
  for (int i = 0; i < n; i++) {
    if (deg[i] == 1) {
      leaves.insert(i);
    }
  }
  vector<vector<int>> adj(n);
  for (int x : code) {
    int leaf = *leaves.begin();
    leaves.erase(leaves.begin());
    add_edge(adj, leaf, x);
    deg[leaf]--;
    deg[x]--;
    if (deg[x] == 1) {
      leaves.insert(x);
    }
  }
  int u = *leaves.begin();
  leaves.erase(leaves.begin());
  int v = *leaves.begin();
  add_edge(adj, u, v);
  return adj;
}

vector<vector<int>> gen_random_tree(int n, TreeShape shape) {
  vector<vector<int>> adj;
  if (shape == UniformParent) {
    adj = gen_tree_from_parent(n, [](int u) { return (int)(rng() % u); });
  } else if (shape == RootBiasedParent) {
    adj = gen_tree_from_parent(n, [](int u) {
      return (int)(rng() % max(1, min(u, max(2, u / 4 + 1))));
    });
  } else if (shape == RecentBiasedParent) {
    adj = gen_tree_from_parent(n, [](int u) {
      int width = max(1, min(u, max(2, u / 4 + 1)));
      int lo = u - width;
      return lo + rng() % (u - lo);
    });
  } else if (shape == Prufer) {
    adj = gen_prufer_tree(n);
  } else {
    adj = gen_tree_from_parent(n, [](int u) { return (u - 1) / 2; });
  }
  for (auto &row : adj) {
    shuffle(row.begin(), row.end(), rng);
  }
  return adj;
}

long long gen_value() { return (int)(rng() % 21) - 10; }

template <class Tree>
Tree init_tree(int n, const vector<vector<int>> &adj, const vector<long long> &a) {
  Tree tree;
  if (rng() % 2) {
    tree = Tree(n, adj, a);
  } else {
    tree = Tree(n, adj);
    tree.init(a);
  }
  return tree;
}

template <class Tree>
void assert_all_queries(Tree &tree, const vector<long long> &a,
                        const vector<int> &parent) {
  for (int u = 0; u < (int)a.size(); u++) {
    assert(tree.query(u).sum == brute_query(a, parent, u));
  }
}

template <class Tree>
void test_point_update_subtree_query(int testcases, int n_lim, int queries,
                                     const vector<TreeShape> &shapes) {
  while (testcases--) {
    int n = rng() % n_lim + 1;
    auto adj = gen_random_tree(n, shapes[rng() % shapes.size()]);
    auto parent = build_parent(adj);
    vector<long long> a(n);
    for (long long &x : a) {
      x = gen_value();
    }

    Tree tree = init_tree<Tree>(n, adj, a);
    assert_all_queries(tree, a, parent);

    int q = queries;
    while (q--) {
      int op = rng() % 4;
      if (op == 0) {
        int u = rng() % n;
        long long x = gen_value();
        tree.update(u, x);
        a[u] = x;
      } else if (op == 1) {
        int u = rng() % n;
        assert(tree.query(u).sum == brute_query(a, parent, u));
      } else if (op == 2) {
        for (long long &x : a) {
          x = gen_value();
        }
        tree.init(a);
      } else {
        assert_all_queries(tree, a, parent);
      }
    }
  }
}

template <class Tree>
void test_subtree_update_subtree_query(int testcases, int n_lim, int queries,
                                       const vector<TreeShape> &shapes) {
  while (testcases--) {
    int n = rng() % n_lim + 1;
    auto adj = gen_random_tree(n, shapes[rng() % shapes.size()]);
    auto parent = build_parent(adj);
    vector<long long> a(n);
    for (long long &x : a) {
      x = gen_value();
    }

    Tree tree = init_tree<Tree>(n, adj, a);
    assert_all_queries(tree, a, parent);

    int q = queries;
    while (q--) {
      int op = rng() % 5;
      if (op == 0) {
        int u = rng() % n;
        long long x = gen_value();
        tree.update(u, x);
        a[u] = x;
      } else if (op == 1) {
        int u = rng() % n;
        long long delta = gen_value();
        tree.update_subtree(u, {delta});
        brute_update_subtree(a, parent, u, delta);
      } else if (op == 2) {
        int u = rng() % n;
        assert(tree.query(u).sum == brute_query(a, parent, u));
      } else if (op == 3) {
        for (long long &x : a) {
          x = gen_value();
        }
        tree.init(a);
      } else {
        assert_all_queries(tree, a, parent);
      }
    }
  }
}

void test_root_point_update_with_pending_subtree_lazy() {
  vector<vector<int>> adj = {{1, 2}, {0}, {0}};
  vector<long long> a = {1, 2, 3};
  LazyStaticTopTree<Path, Point, Lazy, long long> tree(3, adj, a);

  tree.update(0, -2);
  tree.update(0, -2);
  tree.update(0, -2);
  tree.update(0, -2);
  tree.update_subtree(0, {-2});
  tree.update(0, -2);

  assert(tree.query(0).sum == -1);
}

void test_init_clears_pending_lazy() {
  vector<vector<int>> adj = {{1, 2}, {0}, {0}};
  vector<long long> a = {1, 2, 3};
  LazyStaticTopTree<Path, Point, Lazy, long long> tree(3, adj, a);

  tree.update_subtree(0, {5});
  tree.init(a);

  vector<int> parent = build_parent(adj);
  assert_all_queries(tree, a, parent);
}

int main() {
  test_root_point_update_with_pending_subtree_lazy();
  test_init_clears_pending_lazy();

  vector<TreeShape> all_shapes = {
      UniformParent,
      RootBiasedParent,
      RecentBiasedParent,
      Prufer,
      BinaryLike,
  };
  for (auto [testcases, n_lim, queries] :
       vector<tuple<int, int, int>>{
           {200, 8, 400},
           {80, 32, 300},
           {20, 128, 200},
           {6, 512, 100},
       }) {
    test_point_update_subtree_query<StaticTopTree<Path, Point, long long>>(
        testcases, n_lim, queries, all_shapes);
    test_point_update_subtree_query<
        LazyStaticTopTree<Path, Point, Lazy, long long>>(testcases, n_lim,
                                                         queries, all_shapes);
    test_subtree_update_subtree_query<
        LazyStaticTopTree<Path, Point, Lazy, long long>>(testcases, n_lim,
                                                         queries, all_shapes);
  }

  int a, b;
  cin >> a >> b;
  cout << a + b << '\n';
}
