#pragma once

#include <vector>

#include "library/internal/concepts.hpp"

namespace maomao90 {
using namespace std;

// 1-indexed
// new_lazy.merge(old_lazy)
// parent_path.merge(child_path)
template <typename Path, typename Point, typename Lazy, typename T = int>
  requires internal::concepts::StaticTopTreePathPoint<Path, Point, T> &&
           internal::concepts::StaticTopTreeLazy<Lazy, Path, Point, T>
struct LazyStaticTopTree {
  LazyStaticTopTree() {}
  LazyStaticTopTree(int n, vector<vector<int>> &adj)
      : n(n), adj(adj), lc(4 * n + 5), rc(4 * n + 5), p(4 * n + 5),
        op(4 * n + 5), path(4 * n + 5, Path::id()),
        point(4 * n + 5, Point::id()), lz(4 * n + 5, Lazy::id()) {
    ptr = n + 1;
    hld(1, -1);
    r = build_path(1).first;
  }
  void init(vector<T> &_a) {
    a = _a;
    dfs(r);
  }
  void update(int u, T &x) {
    push_path(u);
    a[u] = x;
    apply(u);
    while (p[u]) {
      u = p[u];
      apply(u);
    }
  }
  void update_subtree(int u, Lazy x) {
    int orig_u = u;
    push_path(u);
    apply_update(u, x);
    while (p[u]) {
      if (op[p[u]] == AddEdge) {
        break;
      }
      if (lc[p[u]] == u) {
        apply_update(rc[p[u]], x);
      }
      u = p[u];
    }

    u = orig_u;
    while (p[u]) {
      u = p[u];
      apply(u);
    }
  }
  Path query(int u) {
    push_path(u);
    Path res = path[u];
    while (p[u]) {
      if (op[p[u]] == AddEdge) {
        break;
      }
      if (lc[p[u]] == u) {
        res = res.merge(path[rc[p[u]]]);
      }
      u = p[u];
    }
    return res;
  }

private:
  enum Op { InitVertex, AddVertex, AddEdge, Compress, Rake };
  int n;
  vector<vector<int>> adj;
  vector<T> a;
  int r, ptr;
  vector<int> lc, rc, p;
  vector<Op> op;
  vector<Path> path;
  vector<Point> point;
  vector<Lazy> lz;
  bool has_rc(int u) { return op[u] == Compress || op[u] == Rake; }
  bool is_point(int u) { return op[u] == AddEdge || op[u] == Rake; }
  int hld(int u, int p) {
    int sub = 1, mx = 0;
    for (int i = 0; i < (int)adj[u].size(); i++) {
      int v = adj[u][i];
      if (v == p) {
        adj[u].erase(adj[u].begin() + i);
        i--;
        continue;
      }
      int vsub = hld(v, u);
      sub += vsub;
      if (vsub > mx) {
        mx = vsub;
        if (i) {
          swap(adj[u][i], adj[u][0]);
        }
      }
    }
    return sub;
  }
  void dfs(int u) {
    if (lc[u]) {
      dfs(lc[u]);
    }
    if (rc[u]) {
      dfs(rc[u]);
    }
    apply(u);
  }
  void apply(int u) {
    if (op[u] == InitVertex) {
      path[u] = Path(a[u]);
    } else if (op[u] == AddVertex) {
      path[u] = point[lc[u]].add_vertex(a[u]);
    } else if (op[u] == AddEdge) {
      point[u] = path[lc[u]].add_edge();
    } else if (op[u] == Compress) {
      path[u] = path[lc[u]].merge(path[rc[u]]);
    } else if (op[u] == Rake) {
      point[u] = point[lc[u]].merge(point[rc[u]]);
    }
  }
  inline void add(int u, int l, int r, Op o) {
    lc[u] = l;
    p[l] = u;
    rc[u] = r;
    p[r] = u;
    op[u] = o;
  }
  pair<int, int> merge(vector<pair<int, int>> &lst, Op o) {
    if (lst.size() == 1) {
      return lst[0];
    }
    int tot = 0;
    for (auto [u, s] : lst) {
      tot += s;
    }
    vector<pair<int, int>> lft, rht;
    for (auto [u, s] : lst) {
      (tot > s ? lft : rht).push_back({u, s});
      tot -= 2 * s;
    }
    auto [lu, ls] = merge(lft, o);
    auto [ru, rs] = merge(rht, o);
    add(ptr, lu, ru, o);
    return {ptr++, ls + rs + 1};
  }
  pair<int, int> build_path(int u) {
    vector<pair<int, int>> lst;
    lst.push_back(build_vertex(u));
    while (!adj[u].empty()) {
      u = adj[u][0];
      lst.push_back(build_vertex(u));
    }
    return merge(lst, Compress);
  }
  pair<int, int> build_vertex(int u) {
    if (adj[u].size() <= 1) {
      op[u] = InitVertex;
      return {u, 1};
    }
    vector<pair<int, int>> lst;
    for (int i = 1; i < (int)adj[u].size(); i++) {
      lst.push_back(build_edge(adj[u][i]));
    }
    auto [v, s] = merge(lst, Rake);
    add(u, v, 0, AddVertex);
    return {u, s + 1};
  }
  pair<int, int> build_edge(int u) {
    auto [v, s] = build_path(u);
    add(ptr, v, 0, AddEdge);
    return {ptr++, s + 1};
  }

  void apply_update(int u, Lazy x) {
    if (is_point(u)) {
      point[u] = x.apply(point[u]);
    } else {
      path[u] = x.apply(path[u]);
      if (u <= n) {
        a[u] = x.apply_vertex(a[u]);
      }
    }
    lz[u] = x.merge(lz[u]);
  }
  // add lazy, and update both children of u. Reset lazy of u.
  void push(int u) {
    apply_update(lc[u], lz[u]);
    if (has_rc(u)) {
      apply_update(rc[u], lz[u]);
    }
    lz[u] = Lazy::id();
  }
  // calls push from root to parent of u (excluding u).
  void push_path(int u) {
    vector<int> path;
    while (p[u]) {
      u = p[u];
      path.push_back(u);
    }
    while (!path.empty()) {
      int u = path.back();
      path.pop_back();
      push(u);
    }
  }
};
} // namespace maomao90
