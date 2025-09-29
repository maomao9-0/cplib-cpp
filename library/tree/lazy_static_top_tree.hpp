#pragma once

#include <cassert>
#include <vector>

#include "library/internal/concepts.hpp"

namespace maomao90 {
using namespace std;

/**
 * A static top tree supporting subtree updates (via lazy propagation) and
 * subtree queries.
 *
 * Each vertex of the tree has an associated value. The static top tree can
 * answer queries about the aggregation of values within the subtree of a given
 * vertex and can also support updating all vertex values in the subtree of a
 * given vertex in \f$O(\log_2 n)\f$ time. All indices are 0-based.
 *
 * @tparam Path the @ref internal::concepts::Monoid "monoid" that stores
 *     information about path clusters. Must implement `Path(T x)` constructor
 *     for initialising from a single vertex value, and an `add_edge()` method
 *     to convert to a `Point`. Note that the `merge` function is invoked as
 *     `parent_path.merge(child_path)`.
 * @tparam Point the @ref internal::concepts::Monoid "monoid" that stores
 *     information about point clusters. Must implement an `add_vertex(T x)`
 *     method to convert to a `Path` by including the parent vertex value.
 * @tparam Lazy the @ref internal::concepts::StaticTopTreeLazy "lazy" type used
 *     to perform subtree updates.
 * @tparam T the type of the vertex value.
 * @see @ref internal::concepts::StaticTopTreePathPoint "StaticTopTreePathPoint"
 */
template <typename Path, typename Point, typename Lazy, typename T = int>
  requires internal::concepts::StaticTopTreePathPoint<Path, Point, T> &&
           internal::concepts::StaticTopTreeLazy<Lazy, Path, Point, T>
struct LazyStaticTopTree {
  LazyStaticTopTree() {}
  /**
   * Initialises the static top tree with a tree of `n` vertices where the edges
   * are represented by the adjacency list `adj`.
   *
   * The vertex values are left uninitialised. `adj` must use 0-indexing.
   *
   * @param n the number of vertices in the tree.
   * @param adj the adjacency list representing the tree's edges.
   * @pre `adj.size() == n`
   */
  LazyStaticTopTree(int n, const vector<vector<int>> &adj)
      : n(n), adj(adj), lc(4 * n, -1), rc(4 * n, -1), p(4 * n, -1), op(4 * n),
        path(4 * n, Path::id()), point(4 * n, Point::id()),
        lz(4 * n, Lazy::id()) {
    assert((int)adj.size() == n);
    ptr = n;
    hld(0, -1);
    r = build_path(0).first;
  }
  /**
   * Initialises the static top tree with a tree of `n` vertices, where vertex
   * values are given by array `a` and edges are represented by the adjacency
   * list `adj`.
   *
   * Both `adj` and `a` must use 0-indexing.
   *
   * @param n the number of vertices in the tree.
   * @param adj the adjacency list representing the tree's edges.
   * @param a the array of vertex values.
   * @pre `adj.size() == n`
   * @pre `a.size() == n`
   */
  LazyStaticTopTree(int n, const vector<vector<int>> &adj, const vector<int> &a)
      : LazyStaticTopTree(n, adj) {
    assert((int)a.size() == n);
    this->a = a;
    dfs(r);
  }
  /**
   * Set the vertex values to be equal to array `a`.
   *
   * Runs in \f$O(n)\f$ time.
   *
   * @param a the array of vertex values.
   * @pre `a.size() == n`
   */
  void init(const vector<T> &a) {
    assert((int)a.size() == n);
    this->a = a;
    dfs(r);
  }
  /**
   * Set the value of vertex `u` (0-indexed) to `x`.
   *
   * @param u the index of the vertex to update.
   * @param x the new value to assign to vertex `u`.
   *
   * @pre `0 <= u < n`.
   */
  void update(int u, const T &x) {
    assert(0 <= u && u < n);
    push_path(u);
    a[u] = x;
    apply(u);
    while (p[u] != -1) {
      u = p[u];
      apply(u);
    }
  }
  /**
   * Apply `x` to all vertex values in the subtree rooted at vertex `u`.
   *
   * @param u the index of the vertex to query.
   * @param x the update value to apply to the vertices in the subtree.
   *
   * @pre `0 <= u < n`.
   */
  void update_subtree(int u, const Lazy &x) {
    assert(0 <= u && u < n);
    int orig_u = u;
    push_path(u);
    apply_update(u, x);
    while (p[u] != -1) {
      if (op[p[u]] == AddEdge) {
        break;
      }
      if (lc[p[u]] == u) {
        apply_update(rc[p[u]], x);
      }
      u = p[u];
    }

    u = orig_u;
    while (p[u] != -1) {
      u = p[u];
      apply(u);
    }
  }
  /**
   * Query the aggregation of values over all vertices in the subtree rooted at
   * vertex `u` (0-indexed).
   *
   * @param u the index of the vertex to query.
   *
   * @pre `0 <= u < n`.
   */
  Path query(int u) {
    assert(0 <= u && u < n);
    push_path(u);
    Path res = path[u];
    while (p[u] != -1) {
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
    if (lc[u] != -1) {
      dfs(lc[u]);
    }
    if (rc[u] != -1) {
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
    rc[u] = r;
    p[l] = u;
    if (r != -1) {
      p[r] = u;
    }
    op[u] = o;
  }
  pair<int, int> merge(const vector<pair<int, int>> &lst, Op o) {
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
    add(u, v, -1, AddVertex);
    return {u, s + 1};
  }
  pair<int, int> build_edge(int u) {
    auto [v, s] = build_path(u);
    add(ptr, v, -1, AddEdge);
    return {ptr++, s + 1};
  }

  void apply_update(int u, const Lazy &x) {
    if (is_point(u)) {
      point[u] = x.apply(point[u]);
    } else {
      path[u] = x.apply(path[u]);
      if (u < n) {
        a[u] = x.apply_vertex(a[u]);
      }
    }
    lz[u] = x.merge(lz[u]);
  }
  // add lazy, and update both children of u. Reset lazy of u.
  void push(int u) {
    apply_update(lc[u], lz[u]);
    if (rc[u] != -1) {
      apply_update(rc[u], lz[u]);
    }
    lz[u] = Lazy::id();
  }
  // calls push from root to parent of u (excluding u).
  void push_path(int u) {
    vector<int> path;
    while (p[u] != -1) {
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
