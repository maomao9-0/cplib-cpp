#pragma once

#include "library/internal/type_traits.hpp"

#include <type_traits>
#include <concepts>

namespace maomao90 {
    template <integral T> requires
        internal::type_traits::is_64bit_or_less_v<T>
    constexpr T pow_mod(T b, T p, T mod) {
        using U = conditional_t<internal::type_traits::is_32bit_or_less_v<T>, unsigned long long, unsigned __int128>;
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
