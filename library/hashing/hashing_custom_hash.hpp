#pragma once

#include "library/internal/hashing.hpp"

#include <array>
#include <concepts>

namespace maomao90 {
    template <typename T>
    concept Hashing = requires(T v) {
        { T::get_num_bases() } -> convertible_to<size_t>;
        { v.get_v() } -> same_as<array<typename T::mint::umod_type, T::get_num_bases()>>;
    };
    template <Hashing T>
    struct HashingCustomHash {
        inline static const array<unsigned long long, T::get_num_bases()> fixed_random = 
            internal::hashing::gen_fixed_random<T::get_num_bases()>();
        size_t operator()(const T &o) const {
            unsigned long long res = 0;
            auto v = o.get_v();
            for (int i = 0; i < T::get_num_bases(); i++) {
                res ^= splitmix64(v[i] ^ fixed_random[i]);
            }
            return res;
        }
    };
}
