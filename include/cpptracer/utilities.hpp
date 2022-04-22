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
#include <cassert>
#include <random>
#include <set>

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

/// @brief Generates a unique name of the given length.
/// @param length of the name.
/// @return The generated name.
static inline std::string get_unique_name(unsigned length = 3)
{
    static const size_t characters = 27;
    // Define the usable character.
    static const char alphanum[characters] = "abcdefghijklmnopqrstuvwxyz";
    /// Set of already used symbols.
    static std::set<std::string> usedSymbols;
    /// Will be used to obtain a seed for the random number engine
    static std::random_device rd;
    /// Standard mersenne_twister_engine seeded with rd().
    static std::mt19937 gen(rd());
    // Create an uniform distribution.
    static std::uniform_int_distribution<size_t> dis(0, characters - 1);
    // Establish a new seed.
    std::string symbol;
    do {
        symbol.clear();
        for (unsigned int it = 0; it < length; ++it) {
            symbol += alphanum[dis(gen) % (sizeof(alphanum) - 1)];
        }
    } while (!usedSymbols.insert(symbol).second);
    return symbol;
}

} // namespace cpptracer::utility