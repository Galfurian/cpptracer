/// @file trace.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Contains the trace class, which is stored inside the tracer.

#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <type_traits>
#include <bitset>

#include "feq.hpp"

namespace cpptracer
{

class Trace {
public:
    /// @brief Constructor of the trace.
    Trace(std::string const &_name, std::string const &_symbol)
        : name(_name),
          symbol(_symbol)
    {
        // Nothing to do.
    }

    /// @brief Destructor.
    virtual ~Trace() = default;

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

template <typename T>
class TraceWrapper : public Trace {
public:
    /// A pointer to the variable that has to be traced.
    const T *ptr;
    /// Previous value of the trace.
    T previous;

    /// @brief Constructor.
    /// @param _name     The name of the trace.
    /// @param _symbol   The symbol to assign.
    /// @param _ptr      Pointer to the variable.
    TraceWrapper(std::string _name, std::string _symbol, const T *_ptr)
        : Trace(_name, _symbol),
          ptr(_ptr),
          previous(),
          ss()
    {
        ss << std::setprecision(32);
    }

    /// @brief Destructor.
    ~TraceWrapper() override = default;

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
template <>
inline std::string TraceWrapper<bool>::getVar()
{
    ss.str("");
    ss << "$var integer 1 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template <>
inline std::string TraceWrapper<int8_t>::getVar()
{
    ss.str("");
    ss << "$var integer  8 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template <>
inline std::string TraceWrapper<int16_t>::getVar()
{
    ss.str("");
    ss << "$var integer 16 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template <>
inline std::string TraceWrapper<int32_t>::getVar()
{
    ss.str("");
    ss << "$var integer 32 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template <>
inline std::string TraceWrapper<int64_t>::getVar()
{
    ss.str("");
    ss << "$var integer 64 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template <>
inline std::string TraceWrapper<uint8_t>::getVar()
{
    ss.str("");
    ss << "$var integer  8 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template <>
inline std::string TraceWrapper<uint16_t>::getVar()
{
    ss.str("");
    ss << "$var integer 16 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template <>
inline std::string TraceWrapper<uint32_t>::getVar()
{
    ss.str("");
    ss << "$var integer 32 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template <>
inline std::string TraceWrapper<uint64_t>::getVar()
{
    ss.str("");
    ss << "$var integer 64 " << getSymbol() << " " << getName() << " $end\n";
    return ss.str();
}

template <>
inline std::string TraceWrapper<float>::getVar()
{
    return "$var real 1 " + getSymbol() + " " + getName() + " $end\n";
}

template <>
inline std::string TraceWrapper<double>::getVar()
{
    return "$var real 1 " + getSymbol() + " " + getName() + " $end\n";
}

template <>
inline std::string TraceWrapper<long double>::getVar()
{
    return "$var real 1 " + getSymbol() + " " + getName() + " $end\n";
}

template <>
inline std::string TraceWrapper<std::vector<bool>>::getVar()
{
    ss.str("");
    ss << ptr->size();
    return "$var wire " + ss.str() + " " + getSymbol() + " " + getName() +
           " $end\n";
}

// ----------------------------------------------------------------------------
// Provides specific changing check.
template <>
inline bool TraceWrapper<bool>::hasChanged()
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<int8_t>::hasChanged()
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<int16_t>::hasChanged()
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<int32_t>::hasChanged()
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<int64_t>::hasChanged()
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<uint8_t>::hasChanged()
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<uint16_t>::hasChanged()
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<uint32_t>::hasChanged()
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<uint64_t>::hasChanged()
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<float>::hasChanged()
{
    return !is_equal(previous, (*ptr));
}

template <>
inline bool TraceWrapper<double>::hasChanged()
{
    return !is_equal(previous, (*ptr), 1e-12);
}

template <>
inline bool TraceWrapper<long double>::hasChanged()
{
    return !is_equal(previous, (*ptr), 1e-24);
}

template <>
inline bool TraceWrapper<std::vector<bool>>::hasChanged()
{
    auto it_prev = previous.cbegin(), it_curr = ptr->cbegin();
    while ((it_prev != previous.cend()) && (it_curr != ptr->cend())) {
        if ((*it_curr) != (*it_prev))
            return true;
        ++it_prev, ++it_curr;
    }
    return false;
}

// ----------------------------------------------------------------------------
// Provides specific values.
template <>
inline std::string TraceWrapper<bool>::getValue()
{
    return "b" + std::string((*ptr) ? "1" : "0") + " " + getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<int8_t>::getValue()
{
    return "b" + std::bitset<8>(static_cast<uint8_t>(*ptr)).to_string() + " " + getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<int16_t>::getValue()
{
    return "b" + std::bitset<16>(static_cast<uint16_t>(*ptr)).to_string() + " " + getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<int32_t>::getValue()
{
    return "b" + std::bitset<32>(static_cast<uint32_t>(*ptr)).to_string() + " " + getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<int64_t>::getValue()
{
    return "b" + std::bitset<64>(static_cast<uint64_t>(*ptr)).to_string() + " " + getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<uint8_t>::getValue()
{
    return "b" + std::bitset<8>(*ptr).to_string() + " " + getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<uint16_t>::getValue()
{
    return "b" + std::bitset<16>(*ptr).to_string() + " " + getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<uint32_t>::getValue()
{
    return "b" + std::bitset<32>(*ptr).to_string() + " " + getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<uint64_t>::getValue()
{
    return "b" + std::bitset<64>(*ptr).to_string() + " " + getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<float>::getValue()
{
    ss.str("");
    ss << "r" << (*ptr) << " " << getSymbol() << "\n";
    return ss.str();
}

template <>
inline std::string TraceWrapper<double>::getValue()
{
    ss.str("");
    ss << "r" << (*ptr) << " " << getSymbol() << "\n";
    return ss.str();
}

template <>
inline std::string TraceWrapper<long double>::getValue()
{
    ss.str("");
    ss << "r" << (*ptr) << " " << getSymbol() << "\n";
    return ss.str();
}

template <>
inline std::string TraceWrapper<std::vector<bool>>::getValue()
{
    std::string result("b");
    for (auto const &it : (*ptr)) {
        result.push_back((it) ? '1' : '0');
    }
    result.push_back(' ');
    result.append(getSymbol());
    result.push_back('\n');
    return result;
}

} // namespace cpptracer
