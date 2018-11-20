/// @file   timeScale.hpp
/// @author Enrico Fraccaroli
/// @date   Jul 20, 2016
/// @copyright
/// Copyright (c) 2016,2017,2018 Enrico Fraccaroli <enrico.fraccaroli@gmail.com>
/// Permission to use, copy, modify, and distribute this software for any
/// purpose with or without fee is hereby granted, provided that the above
/// copyright notice and this permission notice appear in all copies.
///
/// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
/// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
/// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
/// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
/// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
/// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
/// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#pragma once

#include <string>
#include <iostream>

/// @brief Class used to store timing dimensions.
class TimeScale
{
public:
    /// Possible granularity of time.
    typedef enum Enums
    {
        SEC,    ///< Seconds
        MS,     ///< Milliseconds
        US,     ///< Microseconds
        NS,     ///< Nanoseconds
        PS      ///< Picoseconds
    } Enum;

private:
    /// The base value.
    uint32_t base;
    /// The magnitude.
    Enum magnitude;
    /// The real value.
    double value;

public:
    /// @brief Constructor.
    /// @param _base  The value of time.
    /// @param _magnitude The scale of the trace.
    explicit TimeScale(uint32_t _base) :
        base(_base),
        magnitude(SEC),
        value()
    {
        value = base * getMagnitude();
    }

    /// @brief Constructor.
    /// @param _base  The value of time.
    /// @param _magnitude The scale of the trace.
    TimeScale(uint32_t _base, Enum _magnitude) :
        base(_base),
        magnitude(_magnitude),
        value()
    {
        value = base * getMagnitude();
    }

    /// @brief Return the scale dimension.
    /// @return The value of the scale.
    inline double getValue() const
    {
        return value;
    }

    inline double getBase() const
    {
        return base;
    }

    /// @brief Return the dimension of the scale.
    /// @return The magnitude of the scale.
    inline double getMagnitude() const
    {
        if (magnitude == SEC) return 1;
        if (magnitude == MS) return 1e-03;
        if (magnitude == US) return 1e-06;
        if (magnitude == NS) return 1e-09;
        if (magnitude == PS) return 1e-12;
        return 1;
    }

    /// @brief Return the string value for the scale.
    /// @return The scale as string.
    inline std::string getMagnitudeString() const
    {
        if (magnitude == SEC) return "s";
        if (magnitude == MS) return "ms";
        if (magnitude == US) return "us";
        if (magnitude == NS) return "ns";
        if (magnitude == PS) return "ps";
        return "s";
    }
};

inline bool operator<(const TimeScale & lhs, const TimeScale & rhs)
{
    return lhs.getValue() < rhs.getValue();
}

inline bool operator<(const TimeScale & lhs, const double & rhs)
{
    return lhs.getValue() < rhs;
}

inline bool operator<(const double & lhs, const TimeScale & rhs)
{
    return lhs < rhs.getValue();
}

inline bool operator>(const TimeScale & lhs, const TimeScale & rhs)
{
    return lhs.getValue() > rhs.getValue();
}

inline bool operator>(const double & lhs, const TimeScale & rhs)
{
    return lhs > rhs.getValue();
}

inline bool operator>(const TimeScale & lhs, const double & rhs)
{
    return lhs.getValue() > rhs;
}

inline double operator+=(double & lhs, const TimeScale & rhs)
{
    lhs += rhs.getValue();
    return lhs;
}
