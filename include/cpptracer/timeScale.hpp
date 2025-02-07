/// @file timeScale.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Contains the helper class for time representation.

#pragma once

#include <iostream>
#include <string>

namespace cpptracer
{

/// @brief Class used to store time units.
class TimeUnit
{
public:
    /// Possible granularity of time.
    enum Enums : unsigned char {
        SEC,     ///< Seconds
        MS,      ///< Milliseconds
        US,      ///< Microseconds
        NS,      ///< Nanoseconds
        PS,      ///< Picoseconds
        FS       ///< Femtoseconds
    } time_unit; ///< The time unit.

    /// @brief Constructs a time unit.
    /// @param _time_unit the time unit.
    constexpr TimeUnit(Enums _time_unit)
        : time_unit(_time_unit)
    {
    }

    /// @brief Return the numberical value of the time unit.
    /// @return the numberical value of the time unit.
    constexpr auto toValue() const -> double
    {
        if (time_unit == MS) {
            return 1e-03;
        }
        if (time_unit == US) {
            return 1e-06;
        }
        if (time_unit == NS) {
            return 1e-09;
        }
        if (time_unit == PS) {
            return 1e-12;
        }
        if (time_unit == FS) {
            return 1e-15;
        }
        return 1;
    }

    /// @brief Return the string representation of the time unit.
    /// @return the string representation of the time unit.
    constexpr auto toString() const -> const char *
    {
        if (time_unit == SEC) {
            return "s";
        }
        if (time_unit == MS) {
            return "ms";
        }
        if (time_unit == US) {
            return "us";
        }
        if (time_unit == NS) {
            return "ns";
        }
        if (time_unit == PS) {
            return "ps";
        }
        if (time_unit == FS) {
            return "fs";
        }
        return "s";
    }
};

/// @brief Class used to store timing dimensions.
class TimeScale
{
private:
    /// The time number.
    unsigned time_number;
    /// The time unit.
    TimeUnit time_unit;
    /// The real value.
    double value;

public:
    /// @brief Constructs a time scale in terms of seconds.
    /// @param _time_number the time number.
    constexpr explicit TimeScale(unsigned _time_number)
        : time_number(_time_number)
        , time_unit(TimeUnit::SEC)
        , value(time_number * time_unit.toValue())
    {
        // Nothing to do.
    }

    /// @brief Constructs a time scale.
    /// @param _time_number the time number.
    /// @param _time_unit the time unit.
    constexpr TimeScale(unsigned _time_number, TimeUnit _time_unit)
        : time_number(_time_number)
        , time_unit(_time_unit)
        , value(time_number * time_unit.toValue())
    {
        // Nothing to do.
    }

    /// @brief Return the scale dimension.
    /// @return The value of the scale.
    constexpr auto getValue() const -> double { return value; }

    /// @brief Returns the time number.
    /// @return the time number.
    constexpr auto getTimeNumber() const -> unsigned { return time_number; }

    /// @brief Return the time unit.
    /// @return the time unit.
    constexpr auto getTimeUnit() const -> const TimeUnit & { return time_unit; }
};

/// @brief Checks if the first time value is lesser than the second.
/// @param lhs the first value.
/// @param rhs the second value.
/// @return true if the first value is lesser than the second.
/// @return false otherwise.
inline auto operator<(const TimeScale &lhs, const TimeScale &rhs) -> bool { return lhs.getValue() < rhs.getValue(); }

/// @brief Checks if the first time value is lesser than the floating point time value.
/// @param lhs the first value.
/// @param rhs the floating point time value.
/// @return true if the first value is lesser than the second.
/// @return false otherwise.
inline auto operator<(const TimeScale &lhs, const double &rhs) -> bool { return lhs.getValue() < rhs; }

/// @brief Checks if the first floating point time value is lesser than the time value.
/// @param lhs the floating point time value.
/// @param rhs the second value.
/// @return true if the first value is lesser than the second.
/// @return false otherwise.
inline auto operator<(const double &lhs, const TimeScale &rhs) -> bool { return lhs < rhs.getValue(); }

/// @brief Checks if the first time value is greather than the second.
/// @param lhs the first value.
/// @param rhs the second value.
/// @return true if the first value is greather than the second.
/// @return false otherwise.
inline auto operator>(const TimeScale &lhs, const TimeScale &rhs) -> bool { return lhs.getValue() > rhs.getValue(); }

/// @brief Checks if the first time value is greather than the floating point time value.
/// @param lhs the time value.
/// @param rhs the floating point time value.
/// @return true if the first value is greather than the second.
/// @return false otherwise.
inline auto operator>(const TimeScale &lhs, const double &rhs) -> bool { return lhs.getValue() > rhs; }

/// @brief Checks if the first floating point time value is greather than the time value.
/// @param lhs the floating point time value.
/// @param rhs the time value.
/// @return true if the first value is greather than the second.
/// @return false otherwise.
inline auto operator>(const double &lhs, const TimeScale &rhs) -> bool { return lhs > rhs.getValue(); }

/// @brief Sums the time value to the floating point time value.
/// @param lhs the floating point time value.
/// @param rhs the time value.
/// @return the floating point time value plus the time value.
inline auto operator+=(double &lhs, const TimeScale &rhs) -> double
{
    lhs += rhs.getValue();
    return lhs;
}

} // namespace cpptracer
