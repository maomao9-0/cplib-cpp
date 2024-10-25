#pragma once

#include "library/internal/hashing.hpp"

#include <array>
#include <concepts>

namespace maomao90 {
    template <typename T>
    concept Hashing = requires(T v) {
        { v.get_v() } -> same_as<array<typename T::mint::umod_type, T::get_num_bases()>>;
    };
    template <Hashing T>
    struct HashingCustomHash {
        constexpr size_t operator()(const T &o) const {
            return internal::hashing::hash_function(o.get_v());
        }
    };
}
