/// @file   timeScale.hpp
/// @author Enrico Fraccaroli
/// @date   Jul 20, 2016
/// @copyright
/// Copyright (c) 2016 Enrico Fraccaroli <enrico.fraccaroli@gmail.com>
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
    /// Value of the timescale.
    double timeValue;
    /// The order of magnitute of the timescale.
    Enum scaleValue;

public:
    /// @brief Constructor.
    /// @param _timeValue  The value of time.
    /// @param _scaleValue The scale of the trace.
    explicit TimeScale(double _timeValue) :
        timeValue(_timeValue),
        scaleValue(SEC)
    {
        // Nothing to do.
    }

    /// @brief Constructor.
    /// @param _timeValue  The value of time.
    /// @param _scaleValue The scale of the trace.
    TimeScale(double _timeValue, Enum _scaleValue) :
        timeValue(_timeValue),
        scaleValue(_scaleValue)
    {
        // Nothing to do.
    }

    /// @brief Return the scale dimension.
    /// @return The value of the scale.
    inline double getValue() const
    {
        return timeValue * this->getScaleValue();
    }

    inline double getTimeValue() const
    {
        return timeValue;
    }

    /// @brief Return the dimension of the scale.
    /// @return The magnitude of the scale.
    inline double getScaleValue() const
    {
        if (scaleValue == SEC) return 1;
        if (scaleValue == MS) return 1e-03;
        if (scaleValue == US) return 1e-06;
        if (scaleValue == NS) return 1e-09;
        if (scaleValue == PS) return 1e-12;
        return 1;
    }

    /// @brief Return the string value for the scale.
    /// @return The scale as string.
    inline std::string scaleToString() const
    {
        if (scaleValue == SEC) return "s";
        if (scaleValue == MS) return "ms";
        if (scaleValue == US) return "us";
        if (scaleValue == NS) return "ns";
        if (scaleValue == PS) return "ps";
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

