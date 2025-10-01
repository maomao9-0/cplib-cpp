#pragma once

#include <cassert>
#include <vector>

#include "library/internal/concepts.hpp"

// Modified from https://judge.yosupo.jp/submission/268423

namespace maomao90 {
using namespace std;

/**
 * A segment tree supporting point updates and range queries.
 *
 * All operations runs in \f$O(\log_2 n)\f$ time.
 *
 * @note All indices are 0-based, and all ranges are represented as half-open
 * intervals: `[start, end)`.
 *
 * @tparam T the @ref internal::concepts::Monoid "monoid" type to be stored in
 *     the segment tree. The `merge` function is invoked as
 *    `left_monoid.merge(right_monoid)`.
 */
template <internal::concepts::Monoid T> struct SegTree {
  SegTree() : SegTree(0) {}
  /**
   * Constructs a segment tree of size `n`, with all elements initialized
   * to the identity element `T::id()`.
   *
   * @param n the size of the segment tree.
   */
  explicit SegTree(int n) : SegTree(vector<T>(n, T::id())) {}

  /**
   * Constructs a segment tree from the values in vector `a`.
   *
   * Time complexity: \f$O(n)\f$.
   *
   * @param a the vector of initial values to build the segment tree.
   */
  explicit SegTree(const vector<T> &a) : n(int(a.size())) {
    size_pw2 = 1;
    log = 0;
    while (size_pw2 < n) {
      size_pw2 <<= 1;
      log++;
    }
    v = vector<T>(2 * size_pw2, T::id());
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
    return v[p + size_pw2];
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
    assert(0 <= l && l <= r && r <= n);

    l += size_pw2;
    r += size_pw2;

    T sml = T::id(), smr = T::id();
    int i = 1;
    for (; l < r; i++, l >>= 1, r >>= 1) {
      if (l & 1) {
        sml = sml.merge(v[l++]);
      }
      if (r & 1) {
        smr = v[--r].merge(smr);
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
    T sm = T::id();
    do {
      while (l % 2 == 0) {
        l >>= 1;
      }
      if (!pred(sm.merge(v[l]))) {
        while (l < size_pw2) {
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
    T sm = T::id();
    do {
      r--;
      while (r > 1 && (r % 2)) {
        r >>= 1;
      }
      if (!pred(v[r].merge(sm))) {
        while (r < size_pw2) {
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

  void update(int k) { v[k] = v[2 * k].merge(v[2 * k + 1]); }
};
} // namespace maomao90
