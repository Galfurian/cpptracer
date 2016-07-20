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

/// @brief A name generator for traces.
class TraceNameGenerator
{
    private:
        /// Lenght of the name.
        size_t length;
        /// Set of already used symbols.
        std::set<std::string> usedSymbols;
    public:

        /// @brief Constructor.
        /// @param _length The length of the name.
        TraceNameGenerator(const size_t & _length = 3);

        /// @brief Destructor.
        virtual ~TraceNameGenerator();

        /// @brief Allows to generate a unique name.
        /// @return The generated name.
        std::string getUniqueName();
};
