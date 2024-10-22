#pragma once

#include "library/internal/type_traits.hpp"

#include <cassert>
#include <type_traits>
#include <concepts>

namespace maomao90 {
    template <signed_integral T>
    constexpr T inv_gcd(T x, T mod) {
        using U = conditional_t<internal::type_traits::is_32bit_or_less_v<T>, long long, __int128>;
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
