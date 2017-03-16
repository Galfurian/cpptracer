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

#include "traceNameGenerator.hpp"

#include <cstdlib>

TraceNameGenerator::TraceNameGenerator(const size_t & _length) :
    length(_length),
    usedSymbols()
{
    // Nothing to do.
}

TraceNameGenerator::~TraceNameGenerator()
{
    // Nothing to do.
}

std::string TraceNameGenerator::getUniqueName()
{
    // Establish a new seed.
    srand(static_cast<unsigned int>(time(NULL)));
    std::string symbol;
    while (true)
    {
        static const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";
        for (unsigned int it = 0; it < length; ++it)
        {
            symbol += alphanum[rand() % (sizeof(alphanum) - 1)];
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

