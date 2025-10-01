#pragma once

#include <cassert>
#include <vector>

#include "library/internal/concepts.hpp"

// Modified from https://judge.yosupo.jp/submission/242469

namespace maomao90 {
using namespace std;

/**
 * A lazy segment tree supporting range updates and range queries.
 *
 * All operations runs in \f$O(\log_2 n)\f$ time. Note that all indices are
 * 0-based, and all ranges are represented as half-open intervals:
 * `[start, end)`.
 *
 * @tparam T the @ref internal::concepts::Monoid "monoid" type to be stored in
 *     the segment tree. The `merge` function is invoked as
 *    `left_monoid.merge(right_monoid)`.
 * @tparam L the @ref internal::concepts::Lazy "lazy" type used for range
 *     updates. The `merge` function is invoked as
 *     `new_update.merge(old_update)`.
 */
template <internal::concepts::Monoid T, internal::concepts::Lazy<T> L>
struct LazySegTree {
  LazySegTree() : LazySegTree(0) {}
  /**
   * Constructs a segment tree of size `n`, with all elements initialized
   * to the identity element `T::id()`.
   *
   * @param n the size of the segment tree.
   */
  explicit LazySegTree(int n) : LazySegTree(vector<T>(n, T::id())) {}

  /**
   * Constructs a segment tree from the values in vector `a`.
   *
   * Time complexity: \f$O(n)\f$.
   *
   * @param a the vector of initial values to build the segment tree.
   */
  explicit LazySegTree(const vector<T> &a) : n(int(a.size())) {
    size_pw2 = 1;
    log = 0;
    while (size_pw2 < n) {
      size_pw2 <<= 1;
      log++;
    }
    v = vector<T>(2 * size_pw2, T::id());
    lz = vector<L>(size_pw2, L::id());
    for (int i = 0; i < n; i++) {
      v[size_pw2 + i] = a[i];
    }
    for (int i = size_pw2 - 1; i >= 1; i--) {
      update(i);
    }
  }

  /**
   * Returns the number of elements stored in the segment tree.
   *
   * @returns the size of the segment tree.
   */
  int size() { return n; }

  /**
   * Updates the element at index `p` (0-indexed) to the value `x`.
   *
   * @param p the index to update.
   * @param x the new value to assign.
   *
   * @pre `0 <= p < size()`.
   */
  void set(int p, T x) {
    assert(0 <= p && p < n);
    p += size_pw2;
    for (int i = log; i >= 1; i--) {
      push(p >> i);
    }
    v[p] = x;
    for (int i = 1; i <= log; i++) {
      update(p >> i);
    }
  }

  /**
   * Returns the value stored at index `p` (0-indexed).
   *
   * @param p the index to access.
   * @returns the value at index `p`.
   *
   * @pre `0 <= p < size()`.
   */
  T get(int p) {
    assert(0 <= p && p < n);
    p += size_pw2;
    T res = v[p];
    for (int i = 1; i <= log; i++) {
      res = lz[p >> i].apply(res, 1);
    }
    return res;
  }

  /**
   * Queries the segment tree over the half-open interval `[l, r)`.
   *
   * @param l the **inclusive** left endpoint (0-indexed) of the query range.
   * @param r the **exclusive** right endpoint (0-indexed) of the query range.
   * @returns the result obtained by merging all elements in `[l, r)` using a
   *     left-associative fold.
   *
   * @pre `0 <= l <= r <= size()`.
   */
  T query(int l, int r) {
    if (l >= r) {
      return T::id();
    }
    assert(0 <= l && r <= n);

    l += size_pw2;
    r += size_pw2;

    int llen = 0, rlen = 0;
    T sml = T::id(), smr = T::id();
    int i = 1, l2 = l, r2 = r;
    for (; l < r; i++, l >>= 1, r >>= 1) {
      if (l & 1) {
        sml = sml.merge(v[l++]), llen += 1 << (i - 1);
      }
      if (r & 1) {
        smr = v[--r].merge(smr), rlen += 1 << (i - 1);
      }
      if (((l2 >> i) << i) != l2) {
        sml = lz[l2 >> i].apply(sml, llen);
      }
      if (((r2 >> i) << i) != r2) {
        smr = lz[(r2 - 1) >> i].apply(smr, rlen);
      }
    }
    for (; i <= log; i++) {
      if (((l2 >> i) << i) != l2) {
        sml = lz[l2 >> i].apply(sml, llen);
      }
      if (((r2 >> i) << i) != r2) {
        smr = lz[(r2 - 1) >> i].apply(smr, rlen);
      }
    }
    return sml.merge(smr);
  }

  /**
   * Queries the entire segment tree, i.e., equivalent to `query(0, size())`.
   *
   * Time complexity: \f$O(1)\f$.
   *
   * @returns the result obtained by merging all elements in the segment tree
   *     using a left-associative fold.
   */
  T all_query() { return v[1]; }

  /**
   * Applies update `x` to the element at index `p` (0-indexed).
   *
   * @param p the index to update.
   * @param x the update value to apply.
   *
   * @pre `0 <= p < size()`.
   */
  void update(int p, L x) {
    assert(0 <= p && p < n);
    p += size_pw2;
    for (int i = log; i >= 1; i--) {
      push(p >> i);
    }
    v[p] = x.apply(v[p], 1);
    for (int i = 1; i <= log; i++) {
      update(p >> i);
    }
  }

  /**
   * Applies update `x` to the half-open interval `[l, r)`.
   *
   * @param l the **inclusive** left endpoint (0-indexed) of the update range.
   * @param r the **exclusive** right endpoint (0-indexed) of the update range.
   * @param x the update value to apply.
   *
   * @pre `0 <= l <= r <= size()`.
   */
  void update(int l, int r, L x) {
    assert(0 <= l && l <= r && r <= n);

    l += size_pw2;
    r += size_pw2;

    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l) {
        push(l >> i);
      }
      if (((r >> i) << i) != r) {
        push((r - 1) >> i);
      }
    }

    {
      int l2 = l, r2 = r;
      while (l < r) {
        if (l & 1) {
          all_apply(l++, x);
        }
        if (r & 1) {
          all_apply(--r, x);
        }
        l >>= 1;
        r >>= 1;
      }
      l = l2;
      r = r2;
    }

    for (int i = 1; i <= log; i++) {
      if (((l >> i) << i) != l) {
        update(l >> i);
      }
      if (((r >> i) << i) != r) {
        update((r - 1) >> i);
      }
    }
  }

  /**
   * Finds the largest index `x` such that the predicate returns true for the
   * left-associative fold over the half-open interval `[l, x)`.
   *
   * @tparam P the type of the predicate function.
   * @param l the **inclusive** left endpoint (0-indexed) from which the search
   *     proceeds to the right.
   * @param pred a function that accepts a parameter of type `T`, and returns a
   *     boolean.
   * @returns the largest index `x` such that `pred(query(l, x))` is true. Note
   *     that `query(l, x)` computes the left-associative fold over
   *     `[l, x)`, and is exclusive of the right endpoint.
   *
   * @pre `0 <= l <= size()`.
   * @pre `pred(T::id()) == true`. In particular, `pred(query(l, l))` must be
   *     true.
   */
  template <class P> int max_right(int l, P pred) {
    assert(0 <= l && l <= n);
    assert(pred(T::id()));
    if (l == n) {
      return n;
    }
    l += size_pw2;
    for (int i = log; i >= 1; i--) {
      push(l >> i);
    }
    T sm = T::id();
    do {
      while (l % 2 == 0) {
        l >>= 1;
      }
      if (!pred(sm.merge(v[l]))) {
        while (l < size_pw2) {
          push(l);
          l = (2 * l);
          if (pred(sm.merge(v[l]))) {
            sm = sm.merge(v[l]);
            l++;
          }
        }
        return l - size_pw2;
      }
      sm = sm.merge(v[l]);
      l++;
    } while ((l & -l) != l);
    return n;
  }

  /**
   * @overload
   */
  template <bool (*pred)(T)> int max_right(int l) {
    return max_right(l, [](T x) { return pred(x); });
  }

  /**
   * Finds the smallest index `x` such that the predicate returns true for the
   * left-associative fold over the half-open interval `[x, r)`.
   *
   * @tparam P the type of the predicate function.
   * @param r the **exclusive** right endpoint (0-indexed) from which the search
   *     proceeds to the left.
   * @param pred a function that accepts a parameter of type `T`, and returns a
   *     boolean.
   * @returns the smallest index `x` such that `pred(query(x, r))` is true. Note
   *     that `query(x, r)` computes the left-associative fold over `[x, r)`,
   *     and is exclusive of the right bound.
   *
   * @pre `0 <= r <= size()`.
   * @pre `pred(T::id()) == true`. In other words, `pred` should be true for
   *     `query(r, r)`.
   */
  template <class P> int min_left(int r, P pred) {
    assert(0 <= r && r <= n);
    if (r == 0) {
      return 0;
    }
    assert(pred(T::id()));
    r += size_pw2;
    for (int i = log; i >= 1; i--) {
      push((r - 1) >> i);
    }
    T sm = T::id();
    do {
      r--;
      while (r > 1 && (r % 2)) {
        r >>= 1;
      }
      if (!pred(v[r].merge(sm))) {
        while (r < size_pw2) {
          push(r);
          r = (2 * r + 1);
          if (pred(v[r].merge(sm))) {
            sm = v[r].merge(sm);
            r--;
          }
        }
        return r + 1 - size_pw2;
      }
      sm = v[r].merge(sm);
    } while ((r & -r) != r);
    return 0;
  }

  /**
   * @overload
   */
  template <bool (*pred)(T)> int min_left(int r) {
    return min_left(r, [](T x) { return pred(x); });
  }

private:
  int n, size_pw2, log;
  vector<T> v;
  vector<L> lz;

  void update(int k) { v[k] = v[2 * k].merge(v[2 * k + 1]); }
  void all_apply(int k, L f) {
    v[k] = f.apply(v[k], 1 << (log - 31 + __builtin_clz(k)));
    if (k < size_pw2) {
      lz[k] = f.merge(lz[k]);
    }
  }
  void push(int k) {
    all_apply(2 * k, lz[k]);
    all_apply(2 * k + 1, lz[k]);
    lz[k] = L::id();
  }
};
} // namespace maomao90
