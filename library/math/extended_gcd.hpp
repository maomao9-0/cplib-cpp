#pragma once

#include <cassert>
#include <type_traits>
#include <concepts>

#include "library/internal/type_traits.hpp"

namespace maomao90 {
    template <signed_integral T> requires
        internal::type_traits::is_64bit_or_less_v<T>
    constexpr T inv_gcd(T x, T mod) {
        using U = internal::type_traits::safely_multipliable_t<T>;
        U a = mod, b = x, va = 0, vb = 1;
        while (b) {
            T k = a / b;
            a -= k * b; va -= k * vb;
            swap(a, b);
            swap(va, vb);
        }
        assert(a == 1); // gcd should be equal to 1
        return (va % mod + mod) % mod;
    }
}
