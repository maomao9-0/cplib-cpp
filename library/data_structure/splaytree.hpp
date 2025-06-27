#pragma once

#include <cassert>
#include <vector>

#include "library/internal/concepts.hpp"

// Modified from https://judge.yosupo.jp/submission/144167

namespace maomao90 {
using namespace std;

namespace internal::splaytree {
template <internal::concepts::Monoid T, internal::concepts::Lazy<T> L,
          bool store_reverse>
struct Node {
  Node *l, *r;
  int sz;
  bool rev;
  T val, sum;
  L lz;
  Node(T val = T::id())
      : l(nullptr), r(nullptr), sz(1), rev(false), val(val), sum(val),
        lz(L::id()) {}
};

template <internal::concepts::Monoid T, internal::concepts::Lazy<T> L>
struct Node<T, L, true> {
  Node *l, *r;
  int sz;
  bool rev;
  T val, sum, rev_sum;
  L lz;
  Node(T val = T::id())
      : l(nullptr), r(nullptr), sz(1), rev(false), val(val), sum(val),
        rev_sum(val), lz(L::id()) {}
};
} // namespace internal::splaytree

/// A balanced binary search tree.
///
/// Supports operations similar to a LazySegTree. Additionally, also supports
/// splitting, merging, and range reversals. All operations work in ammortised
/// \f$O(\log_2 n)\f$ time. Note that all indices are 0-indexed, and all
/// intervals are half-open: [start, end).
///
/// @tparam T the monoid to be stored in the treap. Note that the `merge`
///     function is called using `left_monoid.merge(right_monoid)`.
/// @tparam L the type used to perform range updates on `T`. Note that the
///     `merge` function is called using `new_update.merge(old_update)`.
/// @tparam store_reverse should be `true` only if range reversal is required
///     and `T` is not commutative.
template <internal::concepts::Monoid T, internal::concepts::Lazy<T> L,
          bool store_reverse = false>
struct SplayTree {
private:
  using splaytree = SplayTree<T, L, store_reverse>;
  using node = internal::splaytree::Node<T, L, store_reverse>;
  node *root;

  int size(node *v) { return !v ? 0 : v->sz; }
  void update(node *v) {
    v->sz = 1;
    v->sum = v->val;
    if constexpr (store_reverse) {
      v->rev_sum = v->val;
    }
    if (v->l) {
      v->sz += v->l->sz;
      v->sum = v->l->sum.merge(v->sum);
      if constexpr (store_reverse) {
        v->rev_sum = v->rev_sum.merge(v->l->rev_sum);
      }
    }
    if (v->r) {
      v->sz += v->r->sz;
      v->sum = v->sum.merge(v->r->sum);
      if constexpr (store_reverse) {
        v->rev_sum = v->r->rev_sum.merge(v->rev_sum);
      }
    }
  }
  void push_down(node *v) {
    if (!v) {
      return;
    }
    if (v->l) {
      propagate(v->l, v->lz);
    }
    if (v->r) {
      propagate(v->r, v->lz);
    }
    v->lz = L::id();
    if (v->rev) {
      if (v->l) {
        reverse(v->l);
      }
      if (v->r) {
        reverse(v->r);
      }
      v->rev = false;
    }
  }
  void propagate(node *v, L x) {
    v->lz = x.merge(v->lz);
    v->val = x.apply(v->val, 1);
    v->sum = x.apply(v->sum, v->sz);
    if constexpr (store_reverse) {
      v->rev_sum = x.apply(v->rev_sum, v->sz);
    }
  }
  void reverse(node *v) {
    swap(v->l, v->r);
    if constexpr (store_reverse) {
      swap(v->sum, v->rev_sum);
    }
    v->rev ^= 1;
  }
  node *rotate_right(node *v) {
    node *l = v->l;
    v->l = l->r;
    l->r = v;
    update(v);
    update(l);
    return l;
  }
  node *rotate_left(node *v) {
    node *r = v->r;
    v->r = r->l;
    r->l = v;
    update(v);
    update(r);
    return r;
  }
  node *splay_top_down(node *v, int k) {
    push_down(v);
    int szl = v->l ? v->l->sz : 0;
    if (k == szl) {
      return v;
    }
    if (k < szl) {
      v->l = splay_top_down(v->l, k);
      v = rotate_right(v);
    } else {
      v->r = splay_top_down(v->r, k - szl - 1);
      v = rotate_left(v);
    }
    update(v);
    return v;
  }
  node *merge_inner(node *l, node *r) {
    if (!l || !r) {
      return !l ? r : l;
    }
    r = splay_top_down(r, 0);
    r->l = l;
    update(r);
    return r;
  }
  pair<node *, node *> split_inner(node *v, int k) {
    int n = size(v);
    if (k >= n) {
      return {v, nullptr};
    }
    v = splay_top_down(v, k);
    node *l = v->l;
    v->l = nullptr;
    update(v);
    return {l, v};
  }
  tuple<node *, node *, node *> split3_inner(node *v, int l, int r) {
    if (l == 0) {
      auto [b, c] = split_inner(v, r);
      return {nullptr, b, c};
    }
    v = splay_top_down(v, l - 1);
    auto [b, c] = split_inner(v->r, r - l);
    v->r = nullptr;
    update(v);
    return {v, b, c};
  }
  // Only can be used to merge if it was split using `split3_inner`. O(1)
  // compared to general `merge3_inner`.
  node *inv_split3_inner(node *a, node *b, node *c) {
    node *v = merge_inner(b, c); // O(1)
    if (!a) {
      return v;
    }
    a->r = v;
    update(a);
    return a;
  }
  node *merge3_inner(node *a, node *b, node *c) {
    node *v = merge_inner(b, c);
    return merge_inner(a, v);
  }
  node *set_inner(node *v, int k, T x) {
    v = splay_top_down(v, k);
    v->val = x;
    update(v);
    return v;
  }
  node *get_inner(node *v, int k, T &x) {
    v = splay_top_down(v, k);
    x = v->val;
    return v;
  }
  node *update_inner(node *v, int l, int r, L x) {
    if (r == l) {
      return v;
    }
    auto [a, b, c] = split3_inner(v, l, r);
    propagate(b, x);
    return inv_split3_inner(a, b, c);
  }
  node *query_inner(node *v, int l, int r, T &res) {
    if (r == l) {
      return v;
    }
    auto [a, b, c] = split3_inner(v, l, r);
    res = b->sum;
    return inv_split3_inner(a, b, c);
  }
  node *reverse_inner(node *v, int l, int r) {
    if (r == l) {
      return v;
    }
    auto [a, b, c] = split3_inner(v, l, r);
    if (b) {
      reverse(b);
    }
    return inv_split3_inner(a, b, c);
  }
  node *insert_inner(node *v, int k, node *u) {
    if (k == size(v)) {
      u->l = v;
      update(u);
      return u;
    }
    if (k == 0) {
      u->r = v;
      update(u);
      return u;
    }
    v = splay_top_down(v, k);
    u->l = v->l;
    v->l = u;
    update(u);
    update(v);
    return v;
  }
  node *erase_inner(node *v, int k) {
    v = splay_top_down(v, k);
    return merge_inner(v->l, v->r);
  }
  node *build(const vector<T> &v, int l, int r) {
    int m = (l + r) >> 1;
    node *u = new node(v[m]);
    if (m > l) {
      u->l = build(v, l, m);
    }
    if (r > m + 1) {
      u->r = build(v, m + 1, r);
    }
    update(u);
    return u;
  }
  SplayTree(node *r) : root(r) {}

public:
  SplayTree() : root(nullptr) {}
  /// Initialises the splay tree with `n` elements, all equal to the identity
  /// element `T::id()`.
  ///
  /// @param n the number of elements to be initialised with.
  explicit SplayTree(int n) : SplayTree(vector<T>(n, T::id())) {}
  /// Initialises the splay tree with values from vector `v`.
  ///
  /// @param v the vector of values to be stored in the splay tree.
  explicit SplayTree(const vector<T> &v) : root(nullptr) {
    if (!v.empty()) {
      root = build(v, 0, v.size());
    }
  }
  /// @returns the number of vertices in the splay tree.
  int size() { return size(root); }
  /// Set the `k`-th index (0-indexed) to `x`.
  ///
  /// @param k the index to set.
  /// @param x the value to set.
  ///
  /// @pre 0 <= k < size().
  void set(int k, T x) {
    assert(0 <= k && k < size());
    root = set_inner(root, k, x);
  }
  /// Get the value at the `k`-th index (0-indexed).
  ///
  /// @param k the index to get the value of.
  /// @returns the value at the `k`-th index.
  ///
  /// @pre 0 <= k < size().
  T get(int k) {
    assert(0 <= k && k < size());
    T res = T::id();
    root = get_inner(root, k, res);
    return res;
  }
  /// Apply update `x` to the half-open interval `[l, r)`.
  ///
  /// @param l the **inclusive** left endpoint (0-indexed) of the interval.
  /// @param r the **exclusive** right endpoint (0-indexed) of the interval.
  /// @param x the update to be applied.
  ///
  /// @pre 0 <= l <= r <= size().
  void update(int l, int r, L x) {
    assert(0 <= l && l <= r && r <= size());
    root = update_inner(root, l, r, x);
  }
  /// Query the half-open interval `[l, r)`.
  ///
  /// @param l the **inclusive** left endpoint (0-indexed) of the interval.
  /// @param r the **exclusive** right endpoint (0-indexed) of the interval.
  /// @returns the result of the left-associative fold in the interval.
  ///
  /// @pre 0 <= l <= r <= size().
  T query(int l, int r) {
    assert(0 <= l && l <= r && r <= size());
    T res = T::id();
    root = query_inner(root, l, r, res);
    return res;
  }
  /// Reverses the half-open interval `[l, r)`.
  ///
  /// @param l the **inclusive** left endpoint (0-indexed) of the interval.
  /// @param r the **exclusive** right endpoint (0-indexed) of the interval.
  ///
  /// @pre 0 <= l <= r <= size().
  void reverse(int l, int r) {
    assert(0 <= l && l <= r && r <= size());
    root = reverse_inner(root, l, r);
  }
  /// Inserts `x` into the `k`-th index (0-indexed).
  ///
  /// For example, if `k = 0`, `x` is inserted at the start, and if `k =
  /// size()`, `x` is inserted at the end.
  ///
  /// @param k the index to insert the value into.
  /// @param x the value to insert.
  ///
  /// @pre 0 <= k <= size().
  void insert(int k, T x) {
    assert(0 <= k && k <= size());
    root = insert_inner(root, k, new node(x));
  }
  /// Erases the value at the `k`-th index (0-indexed).
  ///
  /// @param k the index to erase.
  ///
  /// @pre 0 <= k < size().
  void erase(int k) {
    assert(0 <= k && k < size());
    root = erase_inner(root, k);
  }

  /// Splits `this` into two parts. Then, set `this` to be the left part and
  /// returns the right part.
  ///
  /// @param k the number of elements in the left part after the split.
  /// @returns the splay tree containing the remaining `size() - k` elements.
  splaytree split(int k) {
    assert(0 <= k && k <= size());
    auto [a, b] = split_inner(root, k);
    root = a;
    return splaytree(b);
  }
  /// Splits `this` into three parts. Then, set `this` to be the left part and
  /// returns the middle and right parts.
  ///
  /// @param l the number of elements in the left part after the split.
  /// @param r the total number of elements in the left and middle part, i.e.,
  ///     the middle part has `r - l` elements.
  /// @returns a pair `(middle, right)` where `middle` is the middle splay tree
  ///     containing `r - l` elements and `right` is the right splay tree
  ///     containing `size() - r` elements.
  pair<splaytree, splaytree> split(int l, int r) {
    auto [a, b, c] = split3_inner(root, l, r);
    root = a;
    return {splaytree(b), splaytree(c)};
  }
  /// Merge splay tree `o` to the right of `this`.
  ///
  /// @param o the splay tree to merge to the right of `this`.
  ///
  /// @post `o` points to an empty splay tree.
  void merge(splaytree &o) {
    root = merge_inner(root, o.root);
    o.root = nullptr;
  }

  /// Merge splay tree `b` to the right of `this`, then `c` to the right of
  /// `this` and `b`.
  ///
  /// @param b`the middle splay tree after merging.
  /// @param c the right splay tree after merging.
  ///
  /// @post `b` and `c` points to empty splay trees.
  void merge(splaytree &b, splaytree &c) {
    root = merge3_inner(root, b.root, c.root);
    b.root = c.root = nullptr;
  }
};
} // namespace maomao90
