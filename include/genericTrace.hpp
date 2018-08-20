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
#include <sstream>
#include <iomanip>
#include <vector>
#include <type_traits>
#include <bitset>

#include "doubleOperators.hpp"

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
        ss << std::setprecision(32);
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
inline std::string GenericTraceWrapper<bool>::getVar()
{
    ss.str("");
    ss << "$var integer 1 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template<>
inline std::string GenericTraceWrapper<int8_t>::getVar()
{
    ss.str("");
    ss << "$var integer  8 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template<>
inline std::string GenericTraceWrapper<int16_t>::getVar()
{
    ss.str("");
    ss << "$var integer 16 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template<>
inline std::string GenericTraceWrapper<int32_t>::getVar()
{
    ss.str("");
    ss << "$var integer 32 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template<>
inline std::string GenericTraceWrapper<int64_t>::getVar()
{
    ss.str("");
    ss << "$var integer 64 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template<>
inline std::string GenericTraceWrapper<uint8_t>::getVar()
{
    ss.str("");
    ss << "$var integer  8 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template<>
inline std::string GenericTraceWrapper<uint16_t>::getVar()
{
    ss.str("");
    ss << "$var integer 16 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template<>
inline std::string GenericTraceWrapper<uint32_t>::getVar()
{
    ss.str("");
    ss << "$var integer 32 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template<>
inline std::string GenericTraceWrapper<uint64_t>::getVar()
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
inline std::string GenericTraceWrapper<long double>::getVar()
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
inline bool GenericTraceWrapper<bool>::hasChanged()
{
    return (previous != (*ptr));
}

template<>
inline bool GenericTraceWrapper<int8_t>::hasChanged()
{
    return (previous != (*ptr));
}

template<>
inline bool GenericTraceWrapper<int16_t>::hasChanged()
{
    return (previous != (*ptr));
}

template<>
inline bool GenericTraceWrapper<int32_t>::hasChanged()
{
    return (previous != (*ptr));
}

template<>
inline bool GenericTraceWrapper<int64_t>::hasChanged()
{
    return (previous != (*ptr));
}

template<>
inline bool GenericTraceWrapper<uint8_t>::hasChanged()
{
    return (previous != (*ptr));
}

template<>
inline bool GenericTraceWrapper<uint16_t>::hasChanged()
{
    return (previous != (*ptr));
}

template<>
inline bool GenericTraceWrapper<uint32_t>::hasChanged()
{
    return (previous != (*ptr));
}

template<>
inline bool GenericTraceWrapper<uint64_t>::hasChanged()
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
    return !is_equal(previous, (*ptr), 1e-12);
}

template<>
inline bool GenericTraceWrapper<long double>::hasChanged()
{
    return !is_equal(previous, (*ptr), 1e-24);
}

template<>
inline bool GenericTraceWrapper<std::vector<bool>>::hasChanged()
{
    auto it_prev = previous.begin(), it_curr = ptr->begin();
    while ((it_prev != previous.end()) && (it_curr != ptr->end()))
    {
        if ((*it_curr) != (*it_prev)) return true;
        ++it_prev, ++it_curr;
    }
    return false;
}

// ----------------------------------------------------------------------------
// Provides specific values.
template<>
inline std::string GenericTraceWrapper<bool>::getValue()
{
    return "b" + std::string((*ptr) ? "1" : "0") + " " + getSymbol() + "\n";
}

template<>
inline std::string GenericTraceWrapper<int8_t>::getValue()
{
    return "b" + std::bitset<8>(
        static_cast<uint8_t>(*ptr)).to_string() + " " + getSymbol() + "\n";
}

template<>
inline std::string GenericTraceWrapper<int16_t>::getValue()
{
    return "b" + std::bitset<16>(
        static_cast<uint16_t>(*ptr)).to_string() + " " + getSymbol() + "\n";
}

template<>
inline std::string GenericTraceWrapper<int32_t>::getValue()
{
    return "b" + std::bitset<32>(
        static_cast<uint32_t>(*ptr)).to_string() + " " + getSymbol() + "\n";
}

template<>
inline std::string GenericTraceWrapper<int64_t>::getValue()
{
    return "b" + std::bitset<64>(
        static_cast<uint64_t>(*ptr)).to_string() + " " + getSymbol() + "\n";
}

template<>
inline std::string GenericTraceWrapper<uint8_t>::getValue()
{
    return "b" + std::bitset<8>(*ptr).to_string() + " " + getSymbol() + "\n";
}

template<>
inline std::string GenericTraceWrapper<uint16_t>::getValue()
{
    return "b" + std::bitset<16>(*ptr).to_string() + " " + getSymbol() + "\n";
}

template<>
inline std::string GenericTraceWrapper<uint32_t>::getValue()
{
    return "b" + std::bitset<32>(*ptr).to_string() + " " + getSymbol() + "\n";
}

template<>
inline std::string GenericTraceWrapper<uint64_t>::getValue()
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
inline std::string GenericTraceWrapper<long double>::getValue()
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
