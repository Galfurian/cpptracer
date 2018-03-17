/// @file   doubleOperators.hpp
/// @brief  Functions used to deal with equality between double values.
/// @author Enrico Fraccaroli
/// @date   Aug 23 2017
/// @copyright
/// Copyright (c) 2017, 2018 Enrico Fraccaroli <enry.frak@gmail.com>
/// Permission is hereby granted, free of charge, to any person obtaining a
/// copy of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation
/// the rights to use, copy, modify, merge, publish, distribute, sublicense,
/// and/or sell copies of the Software, and to permit persons to whom the
/// Software is furnished to do so, subject to the following conditions:
///     The above copyright notice and this permission notice shall be included
///     in all copies or substantial portions of the Software.
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.

#pragma once

#include <cmath>

#define Abs(x)    ((x) < 0 ? -(x) : (x))
#define Max(a, b) ((a) > (b) ? (a) : (b))

inline bool is_equal(double a, double b, double tolerance = 1e-09)
{
    double d = Max(Abs(a), Abs(b));
    return static_cast<int>(d * 1e09) == 0 || (Abs(a - b) / d) <= tolerance;
}

inline bool is_lequal(double a, double b, double tolerance = 1e-09)
{
    return is_equal(a, b, tolerance) || (a < b);
}

inline bool is_gequal(double a, double b, double tolerance = 1e-09)
{
    return is_equal(a, b, tolerance) || (a > b);
}
