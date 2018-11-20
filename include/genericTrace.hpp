/// @file   genericTrace.hpp
/// @author Enrico Fraccaroli
/// @date   Jul 20, 2016
/// @copyright
/// Copyright (c) 2016,2017,2018 Enrico Fraccaroli <enrico.fraccaroli@gmail.com>
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
#include <sstream>
#include <iomanip>
#include <vector>
#include <type_traits>
#include <bitset>

#include "doubleOperators.hpp"

namespace vcdpp
{

class GenericTrace
{
public:
    /// @brief Constructor of the trace.
    GenericTrace(std::string const & _name, std::string const & _symbol) :
        name(_name),
        symbol(_symbol)
    {
        // Nothing to do.
    }

    /// @brief Destructor.
    virtual ~GenericTrace() = default;

    /// @brief Provides the name of the trace.
    inline std::string getName()
    {
        return name;
    }

    /// @brief Provides the symbol of the trace.
    inline std::string getSymbol()
    {
        return symbol;
    }

    /// @brief Provides the $var of the trace.
    virtual std::string getVar() = 0;

    /// @brief Provides the current value of the trace.
    virtual std::string getValue() = 0;

    /// @brief Checks if the value has changed w.r.t. the previous one.
    /// @return <b>True</b> if the value has changed,<br>
    ///         <b>False</b> otherwise.
    virtual bool hasChanged() = 0;

    /// @brief Updates the previous value with the current value.
    virtual void updatePrevious() = 0;

protected:
    /// The name of the trace.
    std::string name;
    /// The symbol assigned to the trace.
    std::string symbol;
};

}