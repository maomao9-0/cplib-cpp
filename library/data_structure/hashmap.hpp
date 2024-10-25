#pragma once

#include <bitset>

namespace maomao90 {
    template <typename K, typename T, int LG = 20, class Hash = hash<K>>
    struct HashMap {
        constexpr T& operator[](const K &k) {
            int index = get_index(k);
            if (!vis[index]) {
                vis[index] = 1;
                key[index] = k;
                value[index] = T();
            }
            return value[index];
        }
        constexpr bool erase(const K &k) {
            int index = get_index(k);
            if (!vis[index]) {
                return 0;
            }
            vis[index] = 0;
            return 1;
        }
        constexpr bool contains(const K &k) const {
            int index = get_index(k);
            return vis[index];
        }

        constexpr void clear() {
            vis.reset();
        }
    private:
        static constexpr int MOD = 1 << LG;
        bitset<MOD> vis;
        K key[MOD];
        T value[MOD];

        constexpr int get_index(const K &k) const {
            long long hash = Hash()(k);
            int index = hash & (MOD - 1);
            while (vis[index] && key[index] != k) {
                index = (index + 1) & (MOD - 1);
            }
            return index;
        }
    };
}
