#pragma once

#include "library/math/pow_mod.hpp"
#include "library/internal/type_traits.hpp"

#include <concepts>
#include <type_traits>

namespace maomao90 {
    template <unsigned_integral T>
    constexpr bool miller_rabin(const T &n, const T *bases, const size_t size) {
        using U = internal::type_traits::safely_multipliable_t<T>;
        if (n <= 1) {
            return false;
        }
        if (n == 2) {
            return true;
        }
        if (n % 2 == 0) {
            return false;
        }
        T d = n - 1;
        while (d % 2 == 0) d /= 2;
        for (int i = 0; i < size; i++) {
            T a = bases[i];
            if (a % n == 0) {
                continue;
            }
            T t = d, y = pow_mod<T, T>(a, t, n);
            while (t != n - 1 && y != 1 && y != n - 1) {
                y = (U) y * y % n;
                t <<= 1;
            }
            if (y != n - 1 && t % 2 == 0) {
                return false;
            }
        }
        return true;
    }
    template <unsigned_integral T>
    constexpr bool is_prime(T n) {
        if constexpr (internal::type_traits::is_32bit_or_less_v<T>) {
            T bases[3] = {2, 7, 61};
            return miller_rabin<T>(n, bases, 3);
        } else {
            T bases[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
            return miller_rabin<T>(n, bases, 7);
        }
    }
}
