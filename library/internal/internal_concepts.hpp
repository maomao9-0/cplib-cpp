#pragma once

#include <concepts>

namespace internal::concepts {
    template <typename T>
    concept Addable = requires (T a) {
        a + a;
        a += a;
    };
    template <typename T>
    concept Subtractable = requires (T a) {
        a - a;
        a -= a;
    };
    template <typename T>
    concept Multipliable = requires (T a) {
        a * a;
        a *= a;
    };
    template <typename T>
    concept Dividable =  requires (T a) {
        a / a;
        a /= a;
    };
}
