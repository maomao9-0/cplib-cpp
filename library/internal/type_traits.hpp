#pragma once

#include <type_traits>
#include <limits>

namespace internal::type_traits {
    template <typename T>
    using is_32bit_or_less = bool_constant<numeric_limits<T>::max() <= numeric_limits<unsigned int>::max()>;

    template <typename T>
    using is_64bit_or_less = bool_constant<numeric_limits<T>::max() <= numeric_limits<unsigned long long>::max()>;

#define ENABLE_VALUE(x) \
    template <typename T> \
    constexpr bool x##_v = x<T>::value;

    ENABLE_VALUE(is_32bit_or_less);
    ENABLE_VALUE(is_64bit_or_less);

#undef ENABLE_VALUE
}
