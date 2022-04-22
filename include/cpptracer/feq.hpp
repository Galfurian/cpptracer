/// @file feq.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Functions used to deal with equality between double values.

#pragma once

#include <cmath>

namespace cpptracer
{

/// @brief Checks if the two values are equal.
/// @param a first value.
/// @param b second value.
/// @param tolerance the tollerance for checking equality.
/// @return true if they are equal.
/// @return false if they are different values.
template <typename T>
inline bool is_equal(T a, T b, double tolerance = 1e-09)
{
    T d = std::max(std::abs(a), std::abs(b));
    return static_cast<int>(d * 1e09) == 0 || (std::abs(a - b) / d) <= tolerance;
}

} // namespace cpptracer