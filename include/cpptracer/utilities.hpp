/// @file utilities.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Contains a set of utility functions.

#pragma once

#include <string>
#include <map>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <cmath>
#include <vector>

namespace cpptracer::utility
{

/// @brief Creates a new directory at the given location.
/// @param path of the new directory.
inline void create_dir(std::string const &path)
{
    const int dir_err = system(("mkdir -p " + path).c_str());
    if (dir_err == -1) {
        printf("Error creating directory %s!\n", path.c_str());
        exit(1);
    }
}

/// @brief Transforms the given value to a binary string.
/// @tparam length of the binary output.
/// @tparam T type of the input value.
/// @param value to print.
/// @return the string representing the binary value.
template <typename T>
const std::string dec_to_binary(T value, const T length)
{
    std::string buffer(static_cast<std::size_t>(length), '0');
    std::string::iterator it = buffer.begin();
    for (T i = 0; i < length; ++i)
        *it++ = (value & (T(1) << (length - i - T(1)))) ? '1' : '0';
    return buffer;
}

const std::string vector_to_binary(const std::vector<bool> &vector)
{
    std::string buffer(vector.size(), '0');
    std::string::iterator it = buffer.begin();
    for (std::vector<bool>::const_iterator cit = vector.cbegin(); cit != vector.cend(); ++cit, ++it)
        *it = *cit ? '1' : '0';
    return buffer;
}

template <std::size_t N>
const std::string array_to_binary(const std::array<bool, N> &array)
{
    std::string buffer(N, '0');
    std::string::iterator it = buffer.begin();
    for (typename std::array<bool, N>::const_iterator cit = array.cbegin(); cit != array.cend(); ++cit, ++it)
        *it = *cit ? '1' : '0';
    return buffer;
}

/// @brief PJW hash function is a non-cryptographic hash function created by
/// Peter J. Weinberger of AT&T Bell Labs.
/// @param s the input string.
/// @return the hash value.
inline unsigned int elf_hash(std::string const &s)
{
    unsigned int hash = 0;
    unsigned int x    = 0;
    for (const auto c : s) {
        hash = (hash << 4) + static_cast<unsigned int>(c);
        if ((x = hash & 0xF0000000L) != 0)
            hash ^= (x >> 24);
        hash &= ~x;
    }
    return hash;
}

/// @brief Computes the percent value.
/// @param done the amount currently done.
/// @param total the total amount.
/// @return the percent value.
template <typename TDone, typename TTotal>
inline double get_percent(TDone done, TTotal total)
{
    return (static_cast<double>(done) / static_cast<double>(total)) * 100.0;
}

/// @brief Generates a ***% string.
/// @param done the amount currently done.
/// @param total the total amount.
/// @return the percent string.
template <typename TDone, typename TTotal>
inline std::string get_percent_string(TDone done, TTotal total)
{
    std::stringstream ss;
    ss << std::setprecision(0) << std::fixed
       << std::setw(3) << std::right
       << get_percent(done, total) << "%";
    return ss.str();
}

/// @brief Transforms the string to double.
/// @param s the input string.
/// @return the double value.
inline double to_double(std::string const &s)
{
    std::string::size_type sz;
    return std::stod(s, &sz);
}

/// @brief Transforms the string to number.
/// @param s the input string.
/// @return the output value.
template <typename T>
inline T to_number(std::string const &s)
{
    std::string::size_type sz;
    return static_cast<T>(std::stol(s, &sz));
}

/// @brief Provides the current date.
/// @return the string containing the date.
inline std::string get_date_time()
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%b %d, %Y %I:%M:%S", timeinfo);
    std::string str(buffer);
    return str;
}

} // namespace cpptracer::utility
