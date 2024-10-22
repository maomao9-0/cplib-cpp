#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_multiplicative_function_large"

#include <iostream>
#include <vector>
#include <utility>
using namespace std;

#include "library/math/multiplicative_function.hpp"
#include "library/math/poly.hpp"
#include "library/math/modint.hpp"
using namespace maomao90;
using mint = static_modint<469762049>;

int t;
long long n;
int a, b;

int main() {
    cin >> t;
    while (t--) {
        cin >> n >> a >> b;
        long long m = 1;
        while ((m + 1) * (m + 1) <= n) {
            m++;
        }
        vector<mint> sumg_0(m * 2), sumg_1(m * 2);
        for (int i = 1; i <= m; i++) {
            sumg_0[i - 1] = i - 1;
            sumg_1[i - 1] = (long long) i * (i + 1) / 2 - 1;
            sumg_0[2 * m - i] = n / i - 1;
            sumg_1[2 * m - i] = mint(n / i) * (n / i + 1) / 2 - 1;
        }
        vector<mint> sumgp_0 = lucy_dp(n, sumg_0, [&] (long long p) {
                return mint::raw(1);
                });
        vector<mint> sumgp_1 = lucy_dp(n, sumg_1, [&] (long long p) {
                return mint(p);
                });
        vector<mint> sumfp(m * 2);
        for (int i = 0; i < 2 * m; i++) {
            sumfp[i] = sumgp_0[i] * a + sumgp_1[i] * b;
        }
        auto f = [&] (long long p, int e) {
            return mint(a) * e + mint(b) * p;
        };
        vector<mint> sumf = min25_sieve(n, sumfp, f);
        cout << sumf.back() << '\n';
    }
    return 0;
}
