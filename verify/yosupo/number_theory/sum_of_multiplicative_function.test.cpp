#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_multiplicative_function"

#include <iostream>
#include <vector>
#include <utility>
using namespace std;

#include "library/math/min25_sieve.hpp"
#include "library/math/poly.hpp"
#include "library/math/modint.hpp"
using namespace maomao90;
using mint = static_modint<int, 469762049>;

int t;
long long n;
int a, b;

int main() {
    cin >> t;
    while (t--) {
        cin >> n >> a >> b;
        auto f = [&] (long long p, int e) {
            return mint(a) * e + mint(b) * p;
        };
        vector<mint> vg = {a, b};
        Poly<mint> g(vg);
        vector<pair<long long, mint>> res = min25_sieve(n, f, g);
        cout << res.back().second << '\n';
    }
    return 0;
}
