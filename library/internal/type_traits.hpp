#pragma once

#include <type_traits>
#include <limits>
#include <utility>

namespace maomao90::internal::type_traits {
#define ENABLE_VALUE(x) \
    template <typename T> \
    constexpr bool x##_v = x<T>::value
#define ENABLE_TYPE(x) \
    template <typename T> \
    using x##_t = typename x<T>::type

    template <typename T>
    using is_32bit_or_less = bool_constant<numeric_limits<T>::max() <= numeric_limits<unsigned int>::max()>;
    ENABLE_VALUE(is_32bit_or_less);

    template <typename T>
    using is_64bit_or_less = bool_constant<numeric_limits<T>::max() <= numeric_limits<unsigned long long>::max()>;
    ENABLE_VALUE(is_64bit_or_less);

    template <typename T>
    using safely_multipliable_unsigned = conditional<is_32bit_or_less_v<T>, unsigned long long, unsigned __int128>;
    ENABLE_TYPE(safely_multipliable_unsigned);

    template <typename T>
    using safely_multipliable_signed = conditional<is_32bit_or_less_v<T>, long long, __int128>;
    ENABLE_TYPE(safely_multipliable_signed);

    template <typename T>
    using safely_multipliable = conditional<is_signed_v<T>, safely_multipliable_signed_t<T>, safely_multipliable_unsigned_t<T>>;
    ENABLE_TYPE(safely_multipliable);

    
    template <typename>
    struct is_pair : false_type {};
    template <typename T, typename U>
    struct is_pair<pair<T, U>> : true_type {};
    ENABLE_TYPE(is_pair)

#undef ENABLE_VALUE
#undef ENABLE_TYPE(x)
}
