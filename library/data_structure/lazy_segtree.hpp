#pragma once

#include <cassert>
#include <concepts>
#include <vector>

namespace maomao90 {
using namespace std;
namespace internal::concepts {
// left.merge(right)
template <typename T>
concept Monoid = requires(const T &a, const T &b) {
  { T::id() } -> same_as<T>;
  { a.merge(b) } -> same_as<T>;
};

// new_update.merge(old_update)
template <typename L, typename T>
concept Lazy = requires(const L &lz, const T &x, const int len) {
  { lz.apply(x, len) } -> same_as<T>;
  requires Monoid<L>;
};
} // namespace internal::concepts

// 0-indexed
template <internal::concepts::Monoid T, internal::concepts::Lazy<T> L>
struct LazySegTree {
  LazySegTree() : LazySegTree(0) {}
  explicit LazySegTree(int n) : LazySegTree(vector<T>(n, T::id())) {}
  explicit LazySegTree(const vector<T> &_v) : _n(int(_v.size())) {
    size = 1;
    log = 0;
    while (size < _n) {
      size <<= 1;
      log++;
    }
    v = vector<T>(2 * size, T::id());
    lz = vector<L>(size, L::id());
    for (int i = 0; i < _n; i++) {
      v[size + i] = _v[i];
    }
    for (int i = size - 1; i >= 1; i--) {
      update(i);
    }
  }

  void set(int p, T x) {
    assert(0 <= p && p < _n);
    p += size;
    for (int i = log; i >= 1; i--) {
      push(p >> i);
    }
    v[p] = x;
    for (int i = 1; i <= log; i++) {
      update(p >> i);
    }
  }

  T get(int p) {
    assert(0 <= p && p < _n);
    p += size;
    T res = v[p];
    for (int i = 1; i <= log; i++) {
      res = lz[p >> i].apply(res, 1);
    }
    return res;
  }

  // query [l, r] inclusive of both endpoints
  T qry(int l, int r) {
    if (l > r) {
      return T::id();
    }
    assert(0 <= l && r < _n);
    r++;

    l += size;
    r += size;

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

  T all_qry() { return v[1]; }

  void upd(int p, L f) {
    assert(0 <= p && p < _n);
    p += size;
    for (int i = log; i >= 1; i--) {
      push(p >> i);
    }
    v[p] = f.apply(v[p], 1);
    for (int i = 1; i <= log; i++) {
      update(p >> i);
    }
  }

  // update [l, r] inclusive of both endpoints
  void upd(int l, int r, L f) {
    if (l > r) {
      return;
    }
    assert(0 <= l && r < _n);
    r++;

    l += size;
    r += size;

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
          all_apply(l++, f);
        }
        if (r & 1) {
          all_apply(--r, f);
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

  template <bool (*pred)(T)> int max_right(int l) {
    return max_right(l, [](T x) { return pred(x); });
  }

  // returns largest x such that pred(qry(l, x)) is true
  template <class P> int max_right(int l, P pred) {
    assert(0 <= l && l <= _n);
    assert(pred(T::id()));
    if (l == _n) {
      return _n - 1;
    }
    l += size;
    for (int i = log; i >= 1; i--) {
      push(l >> i);
    }
    T sm = T::id();
    do {
      while (l % 2 == 0) {
        l >>= 1;
      }
      if (!pred(sm.merge(v[l]))) {
        while (l < size) {
          push(l);
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
    return _n - 1;
  }

  template <bool (*pred)(T)> int min_left(int r) {
    return min_left(r, [](T x) { return pred(x); });
  }
  // returns smallest x such that pred(qry(x, l)) is true
  template <class P> int min_left(int r, P pred) {
    assert(-1 <= r && r < _n);
    if (r == -1) {
      return 0;
    }
    r++;
    assert(pred(T::id()));
    r += size;
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
        while (r < size) {
          push(r);
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
  int _n, size, log;
  vector<T> v;
  vector<L> lz;

  void update(int k) { v[k] = v[2 * k].merge(v[2 * k + 1]); }
  void all_apply(int k, L f) {
    v[k] = f.apply(v[k], 1 << (log - 31 + __builtin_clz(k)));
    if (k < size) {
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
