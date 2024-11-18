#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <chrono>
#include <climits>
#include <iostream>
#include <random>
using namespace std;

#include "library/math/modint.hpp"
using namespace maomao90;

mt19937_64 rng(chrono::high_resolution_clock::now().time_since_epoch().count());

template <class T> inline T gen_random_value(const T mod) {
  T res = rng();
  int r = rng() % 10;
  if (r == 0) {
    res = mod - 1;
  } else if (r == 1) {
    if (rng() % 2 == 0) {
      res = numeric_limits<T>::max();
    } else {
      res = numeric_limits<T>::min();
    }
  } else if (r == 2) {
    res = 0;
  }
  if (rng() % 10 != 0)
    res %= mod;
  return res;
}

template <typename mint> void test(int testcases) {
  const auto mod = mint::imod();
  static_assert(mod == mint::imod());
  while (testcases--) {
    long long ta = gen_random_value(mod);
    mint A = ta;
    __int128 a = ta % mod;
    if (a < 0) {
      a += mod;
    }
    assert(A.val() == a);
    assert(A == mint((long long)a));

    unsigned long long tb = gen_random_value(mod);
    mint B = tb;
    __int128 b = tb % mod;
    assert(B.val() == b);
    assert(B == mint((long long)b));

    assert((A == B) == (a == b));
    assert((A != B) == (a != b));

    __int128 c = (a + b) % mod;
    mint C = A + B;
    assert(C.val() == c);

    __int128 d = (a + mod - b) % mod;
    mint D = A - B;
    assert(D.val() == d);

    __int128 e = a * b % mod;
    mint E = A * B;
    assert(E.val() == e);

    long long f;
    do {
      f = rng() % mod;
    } while (__gcd(f, (long long)mod) != 1);
    assert(f < mod);
    mint F = f;
    mint G = F.inv();
    assert(F * G == 1);
    assert((__int128)F.val() * G.val() % mod == 1 % mod);
    long long g = G.val();

    __int128 h = e * g % mod;
    mint H = E / F;
    assert(H.val() == h);

    long long ti = gen_random_value(mod);
    long long ex = rng() % LLONG_MAX;
    if (rng() % 10 == 0)
      ex = rng() % 3;
    __int128 i = ti % mod, j = 1 % mod;
    if (i < 0) {
      i += mod;
    }
    {
      __int128 i2 = i;
      long long e2 = ex;
      while (e2) {
        if (e2 & 1) {
          j = j * i2 % mod;
        }
        i2 = i2 * i2 % mod;
        e2 >>= 1;
      }
    }
    mint I = ti;
    mint J = I.pow(ex);
    assert(I.val() == i);
    assert(J.val() == j);

    __int128 k = a == 0 ? 0 : mod - a;
    mint K = -A;
    assert(K.val() == k);
    assert(A == +A);

    __int128 l = a++;
    if (a >= mod) {
      a -= mod;
    }
    mint L = A++;
    assert(A.val() == a);
    assert(L.val() == l);

    __int128 m = gen_random_value(mod) % mod;
    if (m < 0) {
      m += mod;
    }
    mint M = mint::raw(m);
    assert(M.val() == m);

    __int128 n = ++m;
    if (m >= mod) {
      m -= mod;
    }
    if (n >= mod) {
      n -= mod;
    }
    mint N = ++M;
    assert(M.val() == m);
    assert(N.val() == n);

    __int128 o = b--;
    if (b < 0) {
      b += mod;
    }
    mint O = B--;
    assert(B.val() == b);
    assert(O.val() == o);

    __int128 p = gen_random_value(mod) % mod;
    if (p < 0) {
      p += mod;
    }
    mint P = mint::raw(p);
    assert(P.val() == p);

    __int128 q = --p;
    if (p < 0) {
      p += mod;
    }
    if (q < 0) {
      q += mod;
    }
    mint Q = --P;
    assert(P.val() == p);
    assert(Q.val() == q);
  }
}

template <typename M, M mod> void test_wrapper(int testcases = 10000) {
  test<static_modint<mod>>(testcases);
}

int main() {
  test_wrapper<int, 998244353>(1'000'000);
  test_wrapper<int, 1'000'000'007>(1'000'000);
  test_wrapper<long long, 998244353>();
  test_wrapper<long long, 1'000'000'007>();

  test_wrapper<int, INT_MAX>();
  test_wrapper<int, INT_MAX - 1>();
  test_wrapper<int, INT_MAX - 2>(1);
  test_wrapper<long long, LLONG_MAX>();
  test_wrapper<long long, LLONG_MAX - 1>();
  test_wrapper<long long, LLONG_MAX - 2>();

  test_wrapper<int, 1>();
  test_wrapper<int, 2>();
  test_wrapper<int, 3>();
  test_wrapper<int, 4>();
  test_wrapper<int, 5>();
  test_wrapper<int, 6>();
  test_wrapper<int, 7>();
  test_wrapper<int, 8>();
  test_wrapper<int, 9>();
  test_wrapper<int, 10>();
  test_wrapper<int, 100>();
  test_wrapper<int, 1000>();
  test_wrapper<int, 10000>();
  test_wrapper<int, 100000>();
  test_wrapper<int, 1000000>();
  test_wrapper<int, 10000000>();
  test_wrapper<int, 100000000>();
  test_wrapper<int, 1000000000>();
  test_wrapper<long long, 10000000000>();
  test_wrapper<long long, 100000000000>();
  test_wrapper<long long, 1000000000000>();
  test_wrapper<long long, 10000000000000>();
  test_wrapper<long long, 100000000000000>();
  test_wrapper<long long, 1000000000000000>();
  test_wrapper<long long, 10000000000000000>();
  test_wrapper<long long, 100000000000000000>();
  test_wrapper<long long, 1000000000000000000>();
  test_wrapper<int, 11>();
  test_wrapper<int, 101>();
  test_wrapper<int, 1001>();
  test_wrapper<int, 10001>();
  test_wrapper<int, 100001>();
  test_wrapper<int, 1000001>();
  test_wrapper<int, 10000001>();
  test_wrapper<int, 100000001>();
  test_wrapper<int, 1000000001>();
  test_wrapper<long long, 10000000001>();
  test_wrapper<long long, 100000000001>();
  test_wrapper<long long, 1000000000001>();
  test_wrapper<long long, 10000000000001>();
  test_wrapper<long long, 100000000000001>();
  test_wrapper<long long, 1000000000000001>();
  test_wrapper<long long, 10000000000000001>();
  test_wrapper<long long, 100000000000000001>();
  test_wrapper<long long, 1000000000000000001>();

  test_wrapper<int, 180227696>();
  test_wrapper<int, 302343120>();
  test_wrapper<int, 447813621>();
  test_wrapper<int, 454763204>();
  test_wrapper<int, 557418622>();
  test_wrapper<int, 666012019>();
  test_wrapper<int, 666121844>();
  test_wrapper<int, 726530008>();
  test_wrapper<int, 728776078>();
  test_wrapper<int, 750511963>();

  test_wrapper<long long, 180227696>();
  test_wrapper<long long, 302343120>();
  test_wrapper<long long, 447813621>();
  test_wrapper<long long, 454763204>();
  test_wrapper<long long, 557418622>();
  test_wrapper<long long, 666012019>();
  test_wrapper<long long, 666121844>();
  test_wrapper<long long, 726530008>();
  test_wrapper<long long, 728776078>();
  test_wrapper<long long, 750511963>();

  test_wrapper<long long, 4759123141>();
  test_wrapper<long long, 1012713396046690528>();
  test_wrapper<long long, 1635737873707882727>();
  test_wrapper<long long, 2289963980915342821>();
  test_wrapper<long long, 7323047834428073605>();
  test_wrapper<long long, 7685722831103890333>();
  test_wrapper<long long, 8188732928308455248>();
  test_wrapper<long long, 8350304091958478101>();
  test_wrapper<long long, 8368310815106939055>();
  test_wrapper<long long, 8683117913169507171>();

  int a, b;
  cin >> a >> b;
  cout << a + b << '\n';
}
