#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"


#include <iostream>
using namespace std;

#include "library/math/primality_test.hpp"
using namespace maomao90;

int main() {
    int q; cin >> q;
    while (q--) {
        long long n; cin >> n;
        if (is_prime((unsigned long long) n)) {
            cout << "Yes\n";
        } else {
            cout << "No\n";
        }
    }
    return 0;
}
