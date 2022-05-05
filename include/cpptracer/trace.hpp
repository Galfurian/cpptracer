/// @file trace.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Contains the trace class, which is stored inside the tracer.

#pragma once

#include <type_traits>
#include <iomanip>
#include <string>
#include <vector>
#include <array>

#include "feq.hpp"

namespace cpptracer
{

class Trace {
public:
    /// @brief Constructor of the trace.
    Trace(std::string _name, std::string _symbol)
        : name(std::move(_name)),
          symbol(std::move(_symbol))
    {
        // Nothing to do.
    }

    /// @brief Destructor.
    virtual ~Trace() = default;

    /// @brief Provides the name of the trace.
    inline const std::string &getName() const
    {
        return name;
    }

    /// @brief Provides the symbol of the trace.
    inline const std::string &getSymbol() const
    {
        return symbol;
    }

    /// @brief Provides the $var of the trace.
    virtual std::string getVar() const = 0;

    /// @brief Provides the current value of the trace.
    virtual std::string getValue() const = 0;

    /// @brief Checks if the value has changed w.r.t. the previous one.
    /// @return <b>True</b> if the value has changed,<br>
    ///         <b>False</b> otherwise.
    virtual bool hasChanged() const = 0;

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
    using value_type   = T;
    using pointer_type = const T *;

    /// A pointer to the variable that has to be traced.
    pointer_type ptr;
    /// Previous value of the trace.
    value_type previous;

    /// @brief Constructor.
    /// @param _name     The name of the trace.
    /// @param _symbol   The symbol to assign.
    /// @param _ptr      Pointer to the variable.
    TraceWrapper(std::string _name, std::string _symbol, pointer_type _ptr, unsigned _precision = 32)
        : Trace(std::move(_name), std::move(_symbol)),
          ptr(_ptr),
          previous(),
          precision(_precision),
          tolerance()
    {
        if constexpr (std::is_same<T, float>::value) {
            tolerance = 1e-09;
        } else if constexpr (std::is_same<T, double>::value) {
            tolerance = 1e-12;
        } else if constexpr (std::is_same<T, long double>::value) {
            tolerance = 1e-24;
        }
    }

    /// @brief Destructor.
    ~TraceWrapper() override = default;

    std::string getVar() const override;

    /// @brief Provides the current value of the trace.
    std::string getValue() const override;

    /// @brief Checks if the value has changed w.r.t. the previous one.
    /// @return <b>True</b> if the value has changed,<br>
    ///         <b>False</b> otherwise.
    bool hasChanged() const override;

    /// @brief Updates the previous value with the current value.
    void updatePrevious() override
    {
        previous = (*ptr);
    }

    /// @brief Changes the output precision for floating point values.
    /// @param _precision the desired output precision.
    void setOutputPrecision(unsigned _precision)
    {
        precision = _precision;
    }

    void setTolerance(double _tolerance)
    {
        tolerance = _tolerance;
    }

private:
    /// The floating point precision.
    unsigned precision;
    /// The tolerance used to check if two floating point values are equal.
    double tolerance;
};

template <std::size_t N>
class TraceWrapper<std::array<bool, N>> : public Trace {
public:
    using value_type   = std::array<bool, N>;
    using pointer_type = const std::array<bool, N> *;

    /// A pointer to the variable that has to be traced.
    pointer_type ptr;
    /// Previous value of the trace.
    value_type previous;

    /// @brief Constructor.
    /// @param _name     The name of the trace.
    /// @param _symbol   The symbol to assign.
    /// @param _ptr      Pointer to the variable.
    TraceWrapper(std::string _name, std::string _symbol, pointer_type _ptr)
        : Trace(std::move(_name), std::move(_symbol)),
          ptr(_ptr),
          previous()
    {
    }

    /// @brief Destructor.
    ~TraceWrapper() override = default;

    std::string getVar() const override;

    /// @brief Provides the current value of the trace.
    std::string getValue() const override;

    /// @brief Checks if the value has changed w.r.t. the previous one.
    /// @return <b>True</b> if the value has changed,<br>
    ///         <b>False</b> otherwise.
    bool hasChanged() const override;

    /// @brief Updates the previous value with the current value.
    void updatePrevious() override
    {
        previous = (*ptr);
    }
};

// ----------------------------------------------------------------------------
// Provides specific definition.
template <>
inline std::string TraceWrapper<bool>::getVar() const
{
    return "$var integer 1 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
inline std::string TraceWrapper<int8_t>::getVar() const
{
    return "$var integer  8 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
inline std::string TraceWrapper<int16_t>::getVar() const
{
    return "$var integer 16 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
inline std::string TraceWrapper<int32_t>::getVar() const
{
    return "$var integer 32 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
inline std::string TraceWrapper<int64_t>::getVar() const
{
    return "$var integer 64 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
inline std::string TraceWrapper<uint8_t>::getVar() const
{
    return "$var integer  8 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
inline std::string TraceWrapper<uint16_t>::getVar() const
{
    return "$var integer 16 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
inline std::string TraceWrapper<uint32_t>::getVar() const
{
    return "$var integer 32 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
inline std::string TraceWrapper<uint64_t>::getVar() const
{
    return "$var integer 64 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
inline std::string TraceWrapper<float>::getVar() const
{
    return "$var real 1 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
inline std::string TraceWrapper<double>::getVar() const
{
    return "$var real 1 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
inline std::string TraceWrapper<long double>::getVar() const
{
    return "$var real 1 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
inline std::string TraceWrapper<std::vector<bool>>::getVar() const
{
    return "$var wire " + std::to_string(ptr->size()) + " " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <std::size_t N>
inline std::string TraceWrapper<std::array<bool, N>>::getVar() const
{
    return "$var wire " + std::to_string(N) + " " + this->getSymbol() + " " + this->getName() + " $end\n";
}

// ----------------------------------------------------------------------------
// Provides specific changing check.
template <>
inline bool TraceWrapper<bool>::hasChanged() const
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<int8_t>::hasChanged() const
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<int16_t>::hasChanged() const
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<int32_t>::hasChanged() const
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<int64_t>::hasChanged() const
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<uint8_t>::hasChanged() const
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<uint16_t>::hasChanged() const
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<uint32_t>::hasChanged() const
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<uint64_t>::hasChanged() const
{
    return (previous != (*ptr));
}

template <>
inline bool TraceWrapper<float>::hasChanged() const
{
    return !is_equal(previous, (*ptr), tolerance);
}

template <>
inline bool TraceWrapper<double>::hasChanged() const
{
    return !is_equal(previous, (*ptr), tolerance);
}

template <>
inline bool TraceWrapper<long double>::hasChanged() const
{
    return !is_equal(previous, (*ptr), tolerance);
}

template <>
inline bool TraceWrapper<std::vector<bool>>::hasChanged() const
{
    auto it_prev = previous.cbegin(), it_curr = ptr->cbegin();
    while ((it_prev != previous.cend()) && (it_curr != ptr->cend())) {
        if ((*it_curr) != (*it_prev))
            return true;
        ++it_prev, ++it_curr;
    }
    return false;
}

template <std::size_t N>
inline bool TraceWrapper<std::array<bool, N>>::hasChanged() const
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
inline std::string TraceWrapper<bool>::getValue() const
{
    return "b" + utility::dec_to_binary<1>((*ptr)) + " " + this->getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<int8_t>::getValue() const
{
    return "b" + utility::dec_to_binary<8>(*ptr) + " " + this->getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<int16_t>::getValue() const
{
    return "b" + utility::dec_to_binary<16>(*ptr) + " " + this->getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<int32_t>::getValue() const
{
    return "b" + utility::dec_to_binary<32>(*ptr) + " " + this->getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<int64_t>::getValue() const
{
    return "b" + utility::dec_to_binary<64>(*ptr) + " " + this->getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<uint8_t>::getValue() const
{
    return "b" + utility::dec_to_binary<8>(*ptr) + " " + this->getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<uint16_t>::getValue() const
{
    return "b" + utility::dec_to_binary<16>(*ptr) + " " + this->getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<uint32_t>::getValue() const
{
    return "b" + utility::dec_to_binary<32>(*ptr) + " " + this->getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<uint64_t>::getValue() const
{
    return "b" + utility::dec_to_binary<64>(*ptr) + " " + this->getSymbol() + "\n";
}

template <>
inline std::string TraceWrapper<float>::getValue() const
{
    static char buffer[512] = { 0 };
    snprintf(buffer, 512, "r%.*f %s\n", precision, *ptr, this->getSymbol().c_str());
    return buffer;
}

template <>
inline std::string TraceWrapper<double>::getValue() const
{
    static char buffer[512] = { 0 };
    snprintf(buffer, 512, "r%.*f %s\n", precision, *ptr, this->getSymbol().c_str());
    return buffer;
}

template <>
inline std::string TraceWrapper<long double>::getValue() const
{
    static char buffer[512] = { 0 };
    snprintf(buffer, 512, "r%.*Lf %s\n", precision, *ptr, this->getSymbol().c_str());
    return buffer;
}

template <>
inline std::string TraceWrapper<std::vector<bool>>::getValue() const
{
    return "b" + utility::vec_to_binary(*ptr) + " " + this->getSymbol() + "\n";
}

template <std::size_t N>
inline std::string TraceWrapper<std::array<bool, N>>::getValue() const
{
    return "b" + utility::array_to_binary(*ptr) + " " + this->getSymbol() + "\n";
}

} // namespace cpptracer
