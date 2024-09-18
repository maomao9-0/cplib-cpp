#ifndef MAOMAO90_MODINT
#define MAOMAO90_MODINT
#include <type_traits>
#include <concepts>
#include <limits>
#include <iostream>
#include <cassert>
namespace maomao90 {
    namespace internal::modint {
        constexpr long long pow_mod(long long b, long long p, int mod) {
            long long res = 1;
            while (p) {
                if (p & 1) {
                    res = res * b % mod;
                }
                b = b * b % mod;
                p >>= 1;
            }
            return res;
        }
        constexpr __int128 pow_mod(__int128 b, __int128 p, long long mod) {
            __int128 res = 1;
            while (p) {
                if (p & 1) {
                    res = res * b % mod;
                }
                b = b * b % mod;
                p >>= 1;
            }
            return res;
        }
        constexpr bool is_prime(int n) {
            if (n <= 1) {
                return false;
            }
            if (n == 2) {
                return true;
            }
            if (n % 2 == 0) {
                return false;
            }
            constexpr long long bases[3] = {2, 7, 61};
            long long d = n - 1;
            while (d % 2 == 0) d /= 2;
            for (long long a : bases) {
                long long t = d, y = pow_mod(a, t, n);
                while (t != n - 1 && y != 1 && y != n - 1) {
                    y = y * y % n;
                    t <<= 1;
                }
                if (y != n - 1 && t % 2 == 0) {
                    return false;
                }
            }
            return true;
        }
        constexpr bool is_prime(long long n) {
            if (n <= 1) {
                return false;
            }
            if (n == 2) {
                return true;
            }
            if (n % 2 == 0) {
                return false;
            }
            constexpr __int128 bases[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
            __int128 d = n - 1;
            while (d % 2 == 0) d /= 2;
            for (__int128 a : bases) {
                __int128 t = d, y = pow_mod(a, t, n);
                while (t != n - 1 && y != 1 && y != n - 1) {
                    y = y * y % n;
                    t <<= 1;
                }
                if (y != n - 1 && t % 2 == 0) {
                    return false;
                }
            }
            return true;
        }
        template <typename T>
        constexpr T inv_gcd(T x, T mod) {
            T a = mod, b = x, va = 0, vb = 1;
            while (b) {
                T k = a / b;
                a -= k * b; va -= k * vb;
                swap(a, b);
                swap(va, vb);
            }
            assert(a == 1); // gcd should be equals to 1
            return (va % mod + mod) % mod;
        }
    }
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
            ++_v;
            if (_v == umod()) {
                _v = 0;
            }
            return *this;
        }
        constexpr static_modint& operator--() {
            if (_v == 0) {
                _v = umod();
            }
            --_v;
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
            if constexpr (is_prime_mod) {
                return pow(imod() - 2);
            } else {
                return raw(internal::modint::inv_gcd<BM>(_v, imod()));
            }
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
        static constexpr bool is_prime_mod = internal::modint::is_prime(imod());
    };
}
#endif
