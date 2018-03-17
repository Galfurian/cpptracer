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

#include "doubleOperators.hpp"

#include <string>
#include <vector>
#include <iomanip>
#include <bitset>

class GenericTrace
{
public:
    /// @brief Constructor of the trace.
    GenericTrace(std::string _name, std::string _symbol) :
        name(std::move(_name)),
        symbol(std::move(_symbol))
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
    GenericTraceWrapper(std::string _name, std::string _symbol, T * _ptr) :
        GenericTrace(_name, _symbol),
        ptr(_ptr),
        previous(),
        ss()
    {
        ss << std::setprecision(16);
    }

    /// @brief Destructor.
    ~GenericTraceWrapper() override = default;

    std::string getVar() override;

    /// @brief Provides the current value of the trace.
    std::string getValue() override;

    /// @brief Checks if the value has changed w.r.t. the previous one.
    /// @return <b>True</b> if the value has changed,<br>
    ///         <b>False</b> otherwise.
    bool hasChanged() override;

    /// @brief Updates the previous value with the current value.
    void updatePrevious() override
    {
        previous = (*ptr);
    }

private:
    std::stringstream ss;

};

// ----------------------------------------------------------------------------
// Provides specific definition.
template<>
inline std::string GenericTraceWrapper<short>::getVar()
{
    ss.str("");
    ss << "$var integer 16 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template<>
inline std::string GenericTraceWrapper<int>::getVar()
{
    ss.str("");
    ss << "$var integer 32 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template<>
inline std::string GenericTraceWrapper<unsigned int>::getVar()
{
    ss.str("");
    ss << "$var integer 64 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template<>
inline std::string GenericTraceWrapper<float>::getVar()
{
    return "$var real 1 " + getSymbol() + " " + getName() + " $end\n";
}

template<>
inline std::string GenericTraceWrapper<double>::getVar()
{
    return "$var real 1 " + getSymbol() + " " + getName() + " $end\n";
}

template<>
inline std::string GenericTraceWrapper<std::vector<bool>>::getVar()
{
    ss.str("");
    ss << ptr->size();
    return "$var wire " + ss.str() + " " + getSymbol() + " " + getName() +
           " $end\n";
}

// ----------------------------------------------------------------------------
// Provides specific changing check.
template<>
inline bool GenericTraceWrapper<short>::hasChanged()
{
    return (previous != (*ptr));
}
template<>
inline bool GenericTraceWrapper<int>::hasChanged()
{
    return (previous != (*ptr));
}
template<>
inline bool GenericTraceWrapper<unsigned int>::hasChanged()
{
    return (previous != (*ptr));
}
template<>
inline bool GenericTraceWrapper<float>::hasChanged()
{
    return !is_equal(previous, (*ptr));
}
template<>
inline bool GenericTraceWrapper<double>::hasChanged()
{
    return !is_equal(previous, (*ptr));
}
template<>
inline bool GenericTraceWrapper<std::vector<bool>>::hasChanged()
{
    auto it_prev = previous.begin();
    auto it_curr = ptr->begin();
    while ((it_prev != previous.end()) && (it_curr != ptr->end()))
    {
        if ((*it_curr) != (*it_prev)) return true;
        ++it_prev;
        ++it_curr;
    }
    return false;
}

// ----------------------------------------------------------------------------
// Provides specific values.
template<>
inline std::string GenericTraceWrapper<short>::getValue()
{
    return "b" + std::bitset<16>(*ptr).to_string() + " " + getSymbol() + "\n";
}
template<>
inline std::string GenericTraceWrapper<int>::getValue()
{
    return "b" + std::bitset<32>(*ptr).to_string() + " " + getSymbol() + "\n";
}
template<>
inline std::string GenericTraceWrapper<unsigned int>::getValue()
{
    return "b" + std::bitset<64>(*ptr).to_string() + " " + getSymbol() + "\n";
}
template<>
inline std::string GenericTraceWrapper<float>::getValue()
{
    ss.str("");
    ss << "r" << (*ptr) << " " << getSymbol() << "\n";
    return ss.str();
}
template<>
inline std::string GenericTraceWrapper<double>::getValue()
{
    ss.str("");
    ss << "r" << (*ptr) << " " << getSymbol() << "\n";
    return ss.str();
}
template<>
inline std::string GenericTraceWrapper<std::vector<bool>>::getValue()
{
    std::string result("b");
    for (auto const & it : (*ptr))
    {
        result.push_back((it) ? '1' : '0');
    }
    result.push_back(' ');
    result.append(getSymbol());
    result.push_back('\n');
    return result;
}
