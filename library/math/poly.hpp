#pragma once

#include <vector>

namespace maomao90 {
    template <typename T>
    struct Poly {
        constexpr Poly(): _v(1, 0) {}
        constexpr Poly(int n): _v(n) {}
        constexpr Poly(vector<T> v): _v(v) {}

        constexpr int degree() const {
            return _v.size() - 1;
        }
        constexpr T& operator[](int i) {
            return _v[i];
        }
    private:
        vector<T> _v;
    };
}
