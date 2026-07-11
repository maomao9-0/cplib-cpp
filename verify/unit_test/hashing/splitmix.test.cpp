#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <array>
#include <cassert>
#include <chrono>
#include <climits>
#include <iostream>
#include <random>
#include <unordered_set>
using namespace std;

#include "library/hashing/splitmix.hpp"
using namespace maomao90;

int main() {
  array<unsigned long long, 10> fixed_inputs = {
      0ull,
      1ull,
      2ull,
      3ull,
      0xffffffffffffffffull,
      0xfffffffffffffffeull,
      1ull << 63,
      (1ull << 63) - 1,
      (unsigned long long)LLONG_MAX - 1,
      (unsigned long long)-1234567890123456789ll,
  };

  for (unsigned long long x : fixed_inputs) {
    unsigned long long y = splitmix64(x);
    assert(y == splitmix64(x));
    assert(y == splitmix64(x));
  }

  for (int i = 0; i < (int)fixed_inputs.size(); i++) {
    for (int j = i + 1; j < (int)fixed_inputs.size(); j++) {
      assert(splitmix64(fixed_inputs[i]) != splitmix64(fixed_inputs[j]));
    }
  }

  mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());
  unordered_set<unsigned long long> seen_inputs, seen_outputs;
  while ((int)seen_inputs.size() < 200) {
    unsigned long long x = rng();
    if (seen_inputs.insert(x).second) {
      seen_outputs.insert(splitmix64(x));
      assert(splitmix64(x) == splitmix64(x));
    }
  }
  assert(seen_outputs.size() == seen_inputs.size());

  int a, b;
  cin >> a >> b;
  cout << a + b << '\n';
  return 0;
}
