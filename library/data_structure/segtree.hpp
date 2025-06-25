#pragma once

#include <cassert>
#include <vector>

#include "library/internal/concepts.hpp"

namespace maomao90 {
using namespace std;

// 0-indexed
// left_monoid.merge(right_monoid)
template <internal::concepts::Monoid T> struct SegTree {
  SegTree() : SegTree(0) {}
  explicit SegTree(int n) : SegTree(vector<T>(n, T::id())) {}
  explicit SegTree(const vector<T> &a) : n(int(a.size())) {
    size = 1;
    log = 0;
    while (size < n) {
      size <<= 1;
      log++;
    }
    v = vector<T>(2 * size, T::id());
    for (int i = 0; i < n; i++) {
      v[size + i] = a[i];
    }
    for (int i = size - 1; i >= 1; i--) {
      update(i);
    }
  }

  void set(int p, T x) {
    assert(0 <= p && p < n);
    p += size;
    v[p] = x;
    for (int i = 1; i <= log; i++) {
      update(p >> i);
    }
  }

  T get(int p) {
    assert(0 <= p && p < n);
    return v[p + size];
  }

  // query [l, r] inclusive of both endpoints
  T qry(int l, int r) {
    if (l > r) {
      return T::id();
    }
    assert(0 <= l && r < n);
    r++;

    l += size;
    r += size;

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

  T all_qry() { return v[1]; }

  template <bool (*pred)(T)> int max_right(int l) {
    return max_right(l, [](T x) { return pred(x); });
  }

  // returns largest x such that pred(qry(l, x)) is true
  template <class P> int max_right(int l, P pred) {
    assert(0 <= l && l <= n);
    assert(pred(T::id()));
    if (l == n) {
      return n - 1;
    }
    l += size;
    T sm = T::id();
    do {
      while (l % 2 == 0) {
        l >>= 1;
      }
      if (!pred(sm.merge(v[l]))) {
        while (l < size) {
          l = (2 * l);
          if (pred(sm.merge(v[l]))) {
            sm = sm.merge(v[l]);
            l++;
          }
        }
        return l - 1 - size;
      }
      sm = sm.merge(v[l]);
      l++;
    } while ((l & -l) != l);
    return n - 1;
  }

  template <bool (*pred)(T)> int min_left(int r) {
    return min_left(r, [](T x) { return pred(x); });
  }
  // returns smallest x such that pred(qry(x, r)) is true
  template <class P> int min_left(int r, P pred) {
    assert(-1 <= r && r < n);
    if (r == -1) {
      return 0;
    }
    r++;
    assert(pred(T::id()));
    r += size;
    T sm = T::id();
    do {
      r--;
      while (r > 1 && (r % 2)) {
        r >>= 1;
      }
      if (!pred(v[r].merge(sm))) {
        while (r < size) {
          r = (2 * r + 1);
          if (pred(v[r].merge(sm))) {
            sm = v[r].merge(sm);
            r--;
          }
        }
        return r + 1 - size;
      }
      sm = v[r].merge(sm);
    } while ((r & -r) != r);
    return 0;
  }

private:
  int n, size, log;
  vector<T> v;

  void update(int k) { v[k] = v[2 * k].merge(v[2 * k + 1]); }
};
} // namespace maomao90
