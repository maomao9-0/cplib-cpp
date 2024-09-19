#pragma once

#include <type_traits>
#include <concepts>
#include <limits>

namespace maomao90 {
    template <integral T>
    constexpr T pow_mod(T b, T p, T mod) {
        using U = conditional_t<numeric_limits<T>::max() <= numeric_limits<unsigned int>::max(), unsigned long long, unsigned __int128>;
        T res = 1;
        b %= mod;
        if (b < 0) {
            b += mod;
        }
        while (p) {
            if (p & 1) {
                res = (U) res * b % mod;
            }
            b = (U) b * b % mod;
            p >>= 1;
        }
        return res;
    }
}
