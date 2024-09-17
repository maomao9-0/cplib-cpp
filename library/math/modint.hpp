#ifndef MAOMAO90_MODINT
#define MAOMAO90_MODINT
#include <type_traits>
#include <concepts>
#include <limits>
#include <iostream>
#include <cassert>
namespace maomao90 {
    template <signed_integral M = int, M mod = 998244353, enable_if_t<(mod >= 1), int> = 0>
    struct static_modint {
        using UM = make_unsigned_t<M>;
        using BM = conditional_t<numeric_limits<M>::max() <= numeric_limits<int>::max(),
              long long, __int128>;
        static constexpr M imod() {
            return mod;
        }
        static constexpr UM umod() {
            return mod;
        }

        static_modint(): _v(0) {}

        static constexpr static_modint raw(M v) {
            static_modint res;
            res._v = v;
            return res;
        }

        template <signed_integral T>
        constexpr static_modint(T v) {
            M x = v % imod();
            if (x < 0) {
                x += imod();
            }
            _v = x;
        }

        template <unsigned_integral T>
        constexpr static_modint(T v): _v(v % umod()) {}

        constexpr UM val() const {
            return _v;
        }

        constexpr static_modint operator+() const {
            return *this;
        }
        constexpr static_modint operator-() const {
            return raw(_v == 0 ? 0 : imod() - _v);
        }

        constexpr static_modint& operator++() {
            _v++;
            if (_v == umod()) {
                _v = 0;
            }
            return *this;
        }
        constexpr static_modint& operator--() {
            if (_v == 0) {
                _v = umod();
            }
            _v--;
            return *this;
        }
        constexpr static_modint operator++(int) {
            static_modint x = *this;
            ++*this;
            return x;
        }
        constexpr static_modint operator--(int) {
            static_modint x = *this;
            --*this;
            return x;
        }

        constexpr static_modint& operator+=(const static_modint &o) {
            _v += o._v;
            if (_v >= umod()) {
                _v -= umod();
            }
            return *this;
        }
        constexpr static_modint& operator-=(const static_modint &o) {
            if (_v < o._v) {
                _v += umod();
            }
            _v -= o._v;
            return *this;
        }
        constexpr static_modint& operator*=(const static_modint &o) {
            _v = (BM) _v * o._v % umod();
            return *this;
        }
        constexpr static_modint& operator/=(const static_modint &o) {
            return *this *= o.inv();
        }

        constexpr static_modint pow(long long p) const {
            assert(p >= 0);
            static_modint res = 1, b = *this;
            while (p) {
                if (p & 1) {
                    res *= b;
                }
                b *= b;
                p >>= 1;
            }
            return res;
        }
        constexpr static_modint inv() const {
            assert(_v > 0);
            return pow(umod() - 2);
        }

        friend constexpr static_modint operator+(const static_modint &l, const static_modint &r) {
            static_modint res = l;
            return res += r;
        }
        friend constexpr static_modint operator-(const static_modint &l, const static_modint &r) {
            static_modint res = l;
            return res -= r;
        }
        friend constexpr static_modint operator*(const static_modint &l, const static_modint &r) {
            static_modint res = l;
            return res *= r;
        }
        friend constexpr static_modint operator/(const static_modint &l, const static_modint &r) {
            static_modint res = l;
            return res /= r;
        }

        constexpr bool operator==(const static_modint &o) const {
            return _v == o._v;
        }
        constexpr bool operator!=(const static_modint &o) const {
            return !(*this == o);
        }

        friend constexpr istream& operator>>(istream &is, static_modint &o) {
            M v; is >> v;
            o = static_modint(v);
            return is;
        }
        friend constexpr ostream& operator<<(ostream &os, const static_modint &o) {
            return os << o._v;
        }
    private:
        UM _v;
    };
}
#endif
