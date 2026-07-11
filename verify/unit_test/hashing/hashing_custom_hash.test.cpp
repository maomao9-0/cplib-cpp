#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <array>
#include <cassert>
#include <iostream>
using namespace std;

#include "library/hashing/hashing_custom_hash.hpp"
using namespace maomao90;

struct RefHashing {
  using mint = static_modint<>;
  array<unsigned int, 3> v;
  const array<unsigned int, 3> &get_v() const { return v; }
};

int main() {
  RefHashing x{{1, 2, 3}};
  HashingCustomHash<RefHashing> h;
  assert(h(x) == internal::hashing::hash_function(x.get_v()));

  int a, b;
  cin >> a >> b;
  cout << a + b << '\n';
  return 0;
}
