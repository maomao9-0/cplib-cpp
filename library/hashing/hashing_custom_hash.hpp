#pragma once

#include "library/internal/hashing.hpp"
#include "library/internal/concepts.hpp"
#include "library/math/modint.hpp"

#include <array>
#include <concepts>
#include <type_traits>

namespace maomao90 {
    template <typename T>
    concept Hashing = requires(T v) {
        requires Modint<typename T::mint>;
        requires internal::concepts::Iterable<decltype(v.get_v())>;
        requires is_same_v<typename decltype(v.get_v())::value_type, typename T::mint::umod_type>;
    };
    template <Hashing T>
    struct HashingCustomHash {
        constexpr size_t operator()(const T &o) const {
            return internal::hashing::hash_function(o.get_v());
        }
    };
}
