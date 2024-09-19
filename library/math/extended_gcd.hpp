#pragma once

#include <cassert>
#include <type_traits>
#include <concepts>
#include <limits>

namespace maomao90 {
    template <signed_integral T>
    constexpr T inv_gcd(T x, T mod) {
        using U = conditional_t<numeric_limits<T>::max() <= numeric_limits<int>::max(), long long, __int128>;
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
