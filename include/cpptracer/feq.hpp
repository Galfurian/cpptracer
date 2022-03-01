/// @file feq.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Functions used to deal with equality between double values.

#pragma once

#include <cmath>

namespace cpptracer
{

#define Abs(x)    ((x) < 0 ? -(x) : (x))
#define Max(a, b) ((a) > (b) ? (a) : (b))

template <typename T>
inline bool is_equal(T a, T b, double tolerance = 1e-09)
{
    T d = Max(Abs(a), Abs(b));
    return static_cast<int>(d * 1e09) == 0 || (Abs(a - b) / d) <= tolerance;
}

template <typename T>
inline bool is_lequal(T a, T b, double tolerance = 1e-09)
{
    return is_equal(a, b, tolerance) || (a < b);
}

template <typename T>
inline bool is_gequal(T a, T b, double tolerance = 1e-09)
{
    return is_equal(a, b, tolerance) || (a > b);
}

} // namespace cpptracer