#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <vector>
using namespace std;

#include "library/hashing/multiset_hash.hpp"
using namespace maomao90;

mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());

using Hash = MultisetHash<static_modint<>, 2>;

Hash build_hash(const map<long long, long long> &cnt) {
  Hash h;
  for (auto [x, c] : cnt) {
    h = h.insert(x, c);
  }
  return h;
}

int main() {
  Hash empty_a, empty_b;
  assert(empty_a == empty_b);

  Hash after_insert = empty_a.insert(5);
  assert(after_insert != empty_a);
  assert(after_insert.erase(5) == empty_a);

  Hash perm_a, perm_b;
  vector<long long> order_a = {3, -1, 7, 3, 0, -1};
  vector<long long> order_b = {-1, 3, 0, 7, -1, 3};
  for (long long x : order_a) {
    perm_a = perm_a.insert(x);
  }
  for (long long x : order_b) {
    perm_b = perm_b.insert(x);
  }
  assert(perm_a == perm_b);

  Hash base = empty_a.insert(-4).insert(2).insert(2).insert(9);
  assert(base.offset(3).offset(-8) == base.offset(-5));

  for (long long x = -5; x <= 5; x++) {
    for (int c = 0; c <= 4; c++) {
      Hash repeated, batched;
      for (int i = 0; i < c; i++) {
        repeated = repeated.insert(x);
      }
      batched = batched.insert(x, c);
      assert(repeated == batched);
      assert(batched.erase(x, c) == Hash());
    }
  }

  for (int tc = 0; tc < 2000; tc++) {
    map<long long, long long> cnt_a, cnt_b;
    int distinct = rng() % 10;
    for (int i = 0; i < distinct; i++) {
      long long x = (long long)(rng() % 21) - 10;
      long long c = rng() % 5;
      if (c) {
        cnt_a[x] += c;
      }
    }
    cnt_b = cnt_a;

    Hash h_a = build_hash(cnt_a);
    Hash h_b = Hash();
    vector<pair<long long, long long>> items(cnt_b.begin(), cnt_b.end());
    shuffle(items.begin(), items.end(), rng);
    for (auto [x, c] : items) {
      if (rng() % 2) {
        h_b = h_b.insert(x, c);
      } else {
        for (int i = 0; i < c; i++) {
          h_b = h_b.insert(x);
        }
      }
    }
    assert(h_a == h_b);

    if (!cnt_b.empty() && rng() % 2) {
      auto it = cnt_b.begin();
      advance(it, rng() % cnt_b.size());
      long long remove = rng() % (it->second + 1);
      if (remove) {
        h_b = h_b.erase(it->first, remove);
        it->second -= remove;
        if (it->second == 0) {
          cnt_b.erase(it);
        }
      }
    } else {
      long long x = (long long)(rng() % 21) - 10;
      long long add = rng() % 4 + 1;
      h_b = h_b.insert(x, add);
      cnt_b[x] += add;
    }
    Hash oracle_b = build_hash(cnt_b);
    assert(h_b == oracle_b);
    assert((h_a == h_b) == (cnt_a == cnt_b));
    assert((h_a != h_b) == (cnt_a != cnt_b));
  }

  int a, b;
  cin >> a >> b;
  cout << a + b << '\n';
  return 0;
}
