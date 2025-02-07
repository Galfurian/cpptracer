/// @file utilities.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Contains a set of utility functions.

#pragma once

#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

namespace cpptracer
{

namespace utility
{

/// @brief Creates a new directory at the given location.
/// @param path of the new directory.
inline void create_dir(std::string const &path)
{
    mode_t mode = 0755;
    int dir_err = mkdir(path.c_str(), mode);
    if (dir_err == -1 && errno != EEXIST) {
        std::cerr << "Error creating directory " << path << "!\n";
        exit(1);
    }
}

/// @brief Transforms the given value to a binary string.
/// @tparam T type of the input value.
/// @param value the input value.
/// @param length the length of the binary string.
/// @return the string representing the binary value.
template <typename T>
auto dec_to_binary(T value, T length) -> std::string
{
    std::string buffer(static_cast<std::size_t>(length), '0');
    std::string::iterator it = buffer.begin();
    for (T i = 0; i < length; ++i) {
        *it++ = (value & (T(1) << (length - i - T(1)))) ? '1' : '0';
    }
    return buffer;
}

/// @brief Transforms the boolean vector to a binary string.
/// @param vector the input vector.
/// @return the string representing the binary value.
auto vector_to_binary(const std::vector<bool> &vector) -> std::string
{
    std::string buffer(vector.size(), '0');
    std::string::iterator it = buffer.begin();
    for (auto cit = vector.cbegin(); cit != vector.cend(); ++cit, ++it) {
        *it = *cit ? '1' : '0';
    }
    return buffer;
}

/// @brief Transforms an array of booleans to a binary string.
/// @tparam N the number of elements in the array.
/// @param array the input array.
/// @return the string representing the binary value.
template <std::size_t N>
auto array_to_binary(const std::array<bool, N> &array) -> std::string
{
    std::string buffer(N, '0');
    std::string::iterator it = buffer.begin();
    for (typename std::array<bool, N>::const_iterator cit = array.cbegin(); cit != array.cend(); ++cit, ++it) {
        *it = *cit ? '1' : '0';
    }
    return buffer;
}

/// @brief PJW hash function is a non-cryptographic hash function created by
/// Peter J. Weinberger of AT&T Bell Labs.
/// @param s the input string.
/// @return the hash value.
inline auto elf_hash(std::string const &s) -> unsigned int
{
    unsigned int hash = 0;
    unsigned int x    = 0;
    for (const auto c : s) {
        hash = (hash << 4) + static_cast<unsigned int>(c);
        x    = hash & 0xF0000000L;
        if (x != 0) {
            hash ^= (x >> 24);
        }
        hash &= ~x;
    }
    return hash;
}

/// @brief Computes the percent value.
/// @param done the amount currently done.
/// @param total the total amount.
/// @return the percent value.
template <typename TDone, typename TTotal>
inline auto get_percent(TDone done, TTotal total) -> double
{
    return (static_cast<double>(done) / static_cast<double>(total)) * 100.0;
}

/// @brief Generates a ***% string.
/// @param done the amount currently done.
/// @param total the total amount.
/// @return the percent string.
template <typename TDone, typename TTotal>
inline auto get_percent_string(TDone done, TTotal total) -> std::string
{
    std::stringstream ss;
    ss << std::setprecision(0) << std::fixed << std::setw(3) << std::right << get_percent(done, total) << "%";
    return ss.str();
}

/// @brief Transforms the string to double.
/// @param s the input string.
/// @return the double value.
inline auto to_double(std::string const &s) -> double
{
    std::string::size_type sz = 0;
    return std::stod(s, &sz);
}

/// @brief Transforms the string to number.
/// @param s the input string.
/// @return the output value.
template <typename T>
inline auto to_number(std::string const &s) -> T
{
    std::string::size_type sz = 0;
    return static_cast<T>(std::stol(s, &sz));
}

/// @brief Provides the current date.
/// @return The string containing the date.
inline auto get_date_time() -> std::string
{
    try {
        // Get the current time
        auto now             = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        // Convert to local time
        std::tm timeinfo{};
        if (localtime_r(&now_time, &timeinfo) == nullptr) {
            std::cerr << "Error getting local time!\n";
            return "";
        }

        // Format date and time
        std::ostringstream oss;
        oss << std::put_time(&timeinfo, "%b %d, %Y %I:%M:%S");

        return oss.str();
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << '\n';
        return "";
    }
}

} // namespace utility

} // namespace cpptracer
