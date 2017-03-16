/// @file   genericTrace.hpp
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

#include <string>

class GenericTrace
{
public:
    /// The name of the trace.
    std::string name;
    /// The symbol assigned to the trace.
    std::string symbol;

    /// @brief Constructor of the trace.
    GenericTrace(std::string _name, std::string _symbol);

    /// @brief Destructor.
    virtual ~GenericTrace();

    /// @brief Provides the name of the trace.
    std::string getName();

    /// @brief Provides the symbol of the trace.
    std::string getSymbol();

    /// @brief Provides the current value of the trace.
    virtual double getValue() = 0;

    /// @brief Checks if the value has changed w.r.t. the previous one.
    /// @return <b>True</b> if the value has changed,<br>
    ///         <b>False</b> otherwise.
    virtual bool hasChanged() = 0;

    /// @brief Updates the previous value with the current value.
    virtual void updatePrevious() = 0;
};

template<typename T>
class GenericTraceWrapper :
    public GenericTrace
{
public:
    /// A pointer to the variable that has to be traced.
    T * ptr;
    /// Previous value of the trace.
    T previous;

    /// @brief Constructor.
    /// @param _name     The name of the trace.
    /// @param _symbol   The symbol to assign.
    /// @param _ptr      Pointer to the variable.
    /// @param _previous Previous value.
    GenericTraceWrapper(std::string _name, std::string _symbol, T * _ptr, T _previous = 0) :
        GenericTrace(_name, _symbol),
        ptr(_ptr),
        previous(_previous)
    {
        // Nothing to do.
    }

    /// @brief Destructor.
    virtual ~GenericTraceWrapper()
    {
        // Nothing to do.
    }

    /// @brief Provides the current value of the trace.
    virtual double getValue()
    {
        return static_cast<double>(*ptr);
    }

    /// @brief Checks if the value has changed w.r.t. the previous one.
    /// @return <b>True</b> if the value has changed,<br>
    ///         <b>False</b> otherwise.
    virtual bool hasChanged()
    {
        return (previous != (*this->ptr));
    }

    /// @brief Updates the previous value with the current value.
    virtual void updatePrevious()
    {
        previous = (*this->ptr);
    }
};
