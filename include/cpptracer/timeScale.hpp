/// @file timeScale.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Contains the helper class for time representation.

#pragma once

#include <string>
#include <iostream>

namespace cpptracer
{

/// @brief Class used to store timing dimensions.
class TimeScale {
public:
    /// Possible granularity of time.
    typedef enum Enums {
        SEC, ///< Seconds
        MS,  ///< Milliseconds
        US,  ///< Microseconds
        NS,  ///< Nanoseconds
        PS   ///< Picoseconds
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
    constexpr explicit TimeScale(uint32_t _base)
        : base(_base),
          magnitude(SEC),
          value()
    {
        value = base * getMagnitude();
    }

    /// @brief Constructor.
    /// @param _base  The value of time.
    /// @param _magnitude The scale of the trace.
    constexpr TimeScale(uint32_t _base, Enum _magnitude)
        : base(_base),
          magnitude(_magnitude),
          value()
    {
        value = base * getMagnitude();
    }

    /// @brief Return the scale dimension.
    /// @return The value of the scale.
    constexpr inline double getValue() const
    {
        return value;
    }

    /// @brief Returns the base value.
    /// @return the base value.
    constexpr inline double getBase() const
    {
        return base;
    }

    /// @brief Return the dimension of the scale.
    /// @return The magnitude of the scale.
    constexpr inline double getMagnitude() const
    {
        if (magnitude == SEC)
            return 1;
        if (magnitude == MS)
            return 1e-03;
        if (magnitude == US)
            return 1e-06;
        if (magnitude == NS)
            return 1e-09;
        if (magnitude == PS)
            return 1e-12;
        return 1;
    }

    /// @brief Return the string value for the scale.
    /// @return The scale as string.
    inline std::string getMagnitudeString() const
    {
        if (magnitude == SEC)
            return "s";
        if (magnitude == MS)
            return "ms";
        if (magnitude == US)
            return "us";
        if (magnitude == NS)
            return "ns";
        if (magnitude == PS)
            return "ps";
        return "s";
    }
};

/// @brief Checks if the first time value is lesser than the second.
/// @param lhs the first value.
/// @param rhs the second value.
/// @return true if the first value is lesser than the second.
/// @return false otherwise.
inline bool operator<(const TimeScale &lhs, const TimeScale &rhs)
{
    return lhs.getValue() < rhs.getValue();
}

/// @brief Checks if the first time value is lesser than the floating point time value.
/// @param lhs the first value.
/// @param rhs the floating point time value.
/// @return true if the first value is lesser than the second.
/// @return false otherwise.
inline bool operator<(const TimeScale &lhs, const double &rhs)
{
    return lhs.getValue() < rhs;
}

/// @brief Checks if the first floating point time value is lesser than the time value.
/// @param lhs the floating point time value.
/// @param rhs the second value.
/// @return true if the first value is lesser than the second.
/// @return false otherwise.
inline bool operator<(const double &lhs, const TimeScale &rhs)
{
    return lhs < rhs.getValue();
}

/// @brief Checks if the first time value is greather than the second.
/// @param lhs the first value.
/// @param rhs the second value.
/// @return true if the first value is greather than the second.
/// @return false otherwise.
inline bool operator>(const TimeScale &lhs, const TimeScale &rhs)
{
    return lhs.getValue() > rhs.getValue();
}

/// @brief Checks if the first time value is greather than the floating point time value.
/// @param lhs the time value.
/// @param rhs the floating point time value.
/// @return true if the first value is greather than the second.
/// @return false otherwise.
inline bool operator>(const TimeScale &lhs, const double &rhs)
{
    return lhs.getValue() > rhs;
}

/// @brief Checks if the first floating point time value is greather than the time value.
/// @param lhs the floating point time value.
/// @param rhs the time value.
/// @return true if the first value is greather than the second.
/// @return false otherwise.
inline bool operator>(const double &lhs, const TimeScale &rhs)
{
    return lhs > rhs.getValue();
}

/// @brief Sums the time value to the floating point time value.
/// @param lhs the floating point time value.
/// @param rhs the time value.
/// @return the floating point time value plus the time value.
inline double operator+=(double &lhs, const TimeScale &rhs)
{
    lhs += rhs.getValue();
    return lhs;
}

} // namespace cpptracer
