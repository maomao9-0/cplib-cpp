#define PROBLEM "https://judge.yosupo.jp/problem/primality_test"

#include <iostream>
#include <cassert>
#include <limits>
using namespace std;

#include "library/math/primality_test.hpp"
using namespace maomao90;

int main() {
    int q; cin >> q;
    while (q--) {
        long long n; cin >> n;
        bool ans = is_prime((unsigned long long) n);
        if (n <= numeric_limits<unsigned int>::max()) {
            assert(ans == is_prime((unsigned int) n));
        }
        if (ans) {
            cout << "Yes\n";
        } else {
            cout << "No\n";
        }
    }
    return 0;
}
