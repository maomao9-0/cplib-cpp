#pragma once

#include <bitset>
#include <vector>

#include "library/internal/hashing.hpp"

namespace maomao90 {
    template <typename K, typename T, typename Hash = internal::hashing::HashObject<K>, bool KEEP_HISTORY = false, int LG = 20>
    struct HashMap {
        constexpr T& operator[](const K &k) {
            int index = get_index(k);
            if (!vis[index]) {
                vis[index] = 1;
                key[index] = k;
                value[index] = T();
                if constexpr (KEEP_HISTORY) {
                    history.push_back(index);
                }
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
            if constexpr (KEEP_HISTORY) {
                for (int i : history) {
                    vis[i] = 0;
                }
                history.clear();
            } else {
                vis.reset();
            }
        }
    private:
        static constexpr int MOD = 1 << LG;
        bitset<MOD> vis;
        K key[MOD];
        T value[MOD];
        vector<int> history;

        constexpr int get_index(const K &k) const {
            unsigned long long hash = Hash()(k);
            int index = hash >> (64 - LG);
            while (vis[index] && key[index] != k) {
                index = (index + 1) & (MOD - 1);
            }
            return index;
        }
    };
}
