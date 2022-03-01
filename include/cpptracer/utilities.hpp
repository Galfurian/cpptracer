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

inline void create_dir(std::string const &path)
{
    const int dir_err = system(("mkdir -p " + path).c_str());
    if (dir_err == -1) {
        printf("Error creating directory %s!\n", path.c_str());
        exit(1);
    }
}

inline unsigned int elfHash(std::string const &s)
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

template <typename TDone, typename TTotal>
inline double getPercent(TDone done, TTotal total)
{
    return (static_cast<double>(done) / static_cast<double>(total)) * 100.0;
}

template <typename TDone, typename TTotal>
inline std::string getPercentString(TDone done, TTotal total)
{
    std::stringstream ss;
    ss << std::setprecision(0) << std::fixed
       << std::setw(3) << std::right
       << getPercent(done, total) << "%";
    return ss.str();
}

inline double to_double(std::string const &s)
{
    std::string::size_type sz;
    return std::stod(s, &sz);
}

template <typename T>
inline T to_number(std::string const &s)
{
    std::string::size_type sz;
    return static_cast<T>(std::stol(s, &sz));
}

/// @brief Provides the current date.
std::string getDateTime();

} // namespace cpptracer::utility