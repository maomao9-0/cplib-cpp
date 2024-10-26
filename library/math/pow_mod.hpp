#pragma once

#include <type_traits>
#include <concepts>

#include "library/internal/type_traits.hpp"
#include "library/internal/concepts.hpp"

namespace maomao90 {
    template <integral T, internal::concepts::broadly_unsigned_integral P> requires
        internal::type_traits::is_64bit_or_less_v<T>
    constexpr T pow_mod(T b, P p, T mod) {
        using U = internal::type_traits::safely_multipliable_t<T>;
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
