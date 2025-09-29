#pragma once

#include <cassert>
#include <vector>

#include "library/internal/concepts.hpp"

namespace maomao90 {
using namespace std;

/**
 * A static top tree supporting single vertex updates and subtree queries.
 *
 * Each vertex of the tree has an associated value. The static top tree can
 * answer queries about the aggregation of values within the subtree of a given
 * vertex and can also support updates to individual vertex values in
 * \f$O(\log_2 n)\f$ time. All indices are 0-based.
 *
 * @tparam Path the @ref internal::concepts::Monoid "monoid" that stores
 *     information about path clusters. Must implement `Path(T x)` constructor
 *     for initialising from a single vertex value, and an `add_edge()` method
 *     to convert to a `Point`. Note that the `merge` function is invoked as
 *     `parent_path.merge(child_path)`.
 * @tparam Point the @ref internal::concepts::Monoid "monoid" that stores
 *     information about point clusters. Must implement an `add_vertex(T x)`
 *     method to convert to a `Path` by including the parent vertex value.
 * @tparam T the type of the vertex value.
 * @see @ref internal::concepts::StaticTopTreePathPoint "StaticTopTreePathPoint"
 */
template <typename Path, typename Point, typename T = int>
  requires internal::concepts::StaticTopTreePathPoint<Path, Point, T>
struct StaticTopTree {
  StaticTopTree() {}
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
  StaticTopTree(int n, const vector<vector<int>> &adj)
      : n(n), adj(adj), lc(4 * n, -1), rc(4 * n, -1), p(4 * n, -1), op(4 * n),
        path(4 * n, Path::id()), point(4 * n, Point::id()) {
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
  StaticTopTree(int n, const vector<vector<int>> &adj, const vector<int> &a)
      : StaticTopTree(n, adj) {
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
    a[u] = x;
    apply(u);
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
};
} // namespace maomao90
