#ifndef TINYSTL_TYPE_TRAIT_H
#define TINYSTL_TYPE_TRAIT_H

#include <type_traits>

template<typename T, T V>
struct m_integral_constant{
    static constexpr T value = V;
    using value_type = T;
};

template<bool b>
using m_bool_constant = m_integral_constant<bool, b>;

using m_false_type = m_bool_constant<false>;
using m_true_type = m_bool_constant<true>;

#endif //TINYSTL_TYPE_TRAIT_H