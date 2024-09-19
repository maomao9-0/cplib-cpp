#pragma once

#include <cassert>

namespace maomao90 {
    template <typename T>
    constexpr T inv_gcd(T x, T mod) {
        T a = mod, b = x, va = 0, vb = 1;
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
