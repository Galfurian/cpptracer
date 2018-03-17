/// @file   traceNameGenerator.hpp
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

#include <set>
#include <string>
#include <random>

/// @brief A name generator for traces.
class TraceNameGenerator
{
public:

    /// @brief Constructor.
    /// @param _length The length of the name.
    TraceNameGenerator(const size_t & _length = 3) :
        length(_length),
        usedSymbols(),
        rd(),
        gen(rd())
    {
        // Nothing to do.
    }

    /// @brief Destructor.
    virtual ~TraceNameGenerator() = default;

    /// @brief Allows to generate a unique name.
    /// @return The generated name.
    inline std::string getUniqueName()
    {
        static const size_t characters = 27;
        // Define the usable character.
        static const char alphanum[characters] = "abcdefghijklmnopqrstuvwxyz";
        // Create an uniform distribution.
        std::uniform_int_distribution<int> dis(0, characters - 1);
        // Establish a new seed.
        std::string symbol;
        while (true)
        {
            for (unsigned int it = 0; it < length; ++it)
            {
                symbol += alphanum[dis(gen) % (sizeof(alphanum) - 1)];
            }
            if (!usedSymbols.insert(symbol).second)
            {
                symbol.clear();
            }
            else
            {
                break;
            }
        }
        return symbol;
    }

private:
    /// Lenght of the name.
    size_t length;
    /// Set of already used symbols.
    std::set<std::string> usedSymbols;
    /// Will be used to obtain a seed for the random number engine
    std::random_device rd;
    /// Standard mersenne_twister_engine seeded with rd().
    std::mt19937 gen;

};
