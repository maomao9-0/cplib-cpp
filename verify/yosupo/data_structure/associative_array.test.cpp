#define PROBLEM "https://judge.yosupo.jp/problem/associative_array"

#include <iostream>
using namespace std;

#include "library/data_structure/hashmap.hpp"
using namespace maomao90;

int main() {
    ios::sync_with_stdio(0), cin.tie(0);
    HashMap<long long, long long> hash_map;
    int q; cin >> q;
    while (q--) {
        int t; cin >> t;
        if (t == 0) {
            long long k, v; cin >> k >> v;
            hash_map[k] = v;
        } else {
            long long k; cin >> k;
            cout << hash_map[k] << '\n';
        }
    }
    return 0;
}
