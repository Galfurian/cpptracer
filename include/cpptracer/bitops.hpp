/// @file bitops.hpp
/// @brief Bitmasks functions.

#pragma once

namespace cpptracer
{

#define bit_set(V, B)          ((V) | (1UL << (B)))   ///< Sets the given bit.
#define bit_clear(V, B)        ((V) & ~(1UL << (B)))  ///< Clears the given bit.
#define bit_flip(V, B)         ((V) ^ (1UL << (B)))   ///< Flips the given bit.
#define bit_set_assign(V, B)   ((V) |= (1UL << (B)))  ///< Sets the given bit, permanently.
#define bit_clear_assign(V, B) ((V) &= ~(1UL << (B))) ///< Clears the given bit, permanently.
#define bit_flip_assign(V, B)  ((V) ^= (1UL << (B)))  ///< Flips the given bit, permanently.

template <typename T>
constexpr inline auto bit_check(T value, T bit)
{
    return value & (T(1) << bit);
}

#define bitmask_set(V, M)          ((V) | (M))   ///< Sets the bits identified by the mask.
#define bitmask_clear(V, M)        ((V) & ~(M))  ///< Clears the bits identified by the mask.
#define bitmask_flip(V, M)         ((V) ^ (M))   ///< Flips the bits identified by the mask.
#define bitmask_set_assign(V, M)   ((V) |= (M))  ///< Sets the bits identified by the mask, permanently.
#define bitmask_clear_assign(V, M) ((V) &= ~(M)) ///< Clears the bits identified by the mask, permanently.
#define bitmask_flip_assign(V, M)  ((V) ^= (M))  ///< Flips the bits identified by the mask, permanently.
#define bitmask_check(V, M)        ((V) & (M))   ///< Checks if the bits identified by the mask are all 1.

}