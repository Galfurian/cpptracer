/// @file trace.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Contains the trace class, which is stored inside the tracer.

#pragma once

#include <array>
#include <cstdint>
#include <iomanip>
#include <string>
#include <type_traits>
#include <vector>

#include "feq.hpp"
#include "utilities.hpp"

namespace cpptracer
{

/// @brief Class used to store a trace.
class Trace
{
public:
    /// @brief Constructor of the trace.
    /// @param _name the name of the trace.
    /// @param _symbol the symbol assigned to the trace.
    Trace(std::string _name, std::string _symbol)
        : name(std::move(_name))
        , symbol(std::move(_symbol))
    {
        // Nothing to do.
    }

    /// @brief Copy constructor.
    /// @param other The other entity to copy.
    Trace(const Trace &other) = default;

    /// @brief Move constructor.
    /// @param other The other entity to move.
    Trace(Trace &&other) noexcept = default;

    /// @brief Copy assignment operator.
    /// @param other The other entity to copy.
    /// @return A reference to this object.
    auto operator=(const Trace &other) -> Trace & = default;

    /// @brief Move assignment operator.
    /// @param other The other entity to move.
    /// @return A reference to this object.
    auto operator=(Trace &&other) noexcept -> Trace & = default;

    /// @brief Destructor.
    virtual ~Trace() = default;

    /// @brief Provides the name of the trace.
    /// @return the name of the trace.
    auto getName() const -> const std::string & { return name; }

    /// @brief Provides the symbol of the trace.
    /// @return the symbol of the trace.
    auto getSymbol() const -> const std::string & { return symbol; }

    /// @brief Provides the $var of the trace.
    /// @return the $var of the trace.
    virtual auto getVar() const -> std::string = 0;

    /// @brief Provides the current value of the trace.
    /// @return the current value of the trace.
    virtual auto getValue() const -> std::string = 0;

    /// @brief Checks if the value has changed w.r.t. the previous one.
    /// @return <b>True</b> if the value has changed,<br>
    ///         <b>False</b> otherwise.
    virtual auto hasChanged() const -> bool = 0;

    /// @brief Updates the previous value with the current value.
    virtual void updatePrevious() = 0;

private:
    /// The name of the trace.
    std::string name;
    /// The symbol assigned to the trace.
    std::string symbol;
};

/// @brief Class used to store a trace of a specific type.
/// @tparam T the type of the traced variable.
template <typename T>
class TraceWrapper : public Trace
{
public:
    /// @brief The type of the traced variable.
    using value_type   = T;
    /// @brief The pointer type of the traced variable.
    using pointer_type = const T *;

    /// @brief Constructor.
    /// @param _name the name of the trace.
    /// @param _symbol the symbol to assign.
    /// @param _ptr pointer to the variable.
    /// @param _precision the desired output precision.
    TraceWrapper(std::string _name, std::string _symbol, pointer_type _ptr, int _precision = 32)
        : Trace(std::move(_name), std::move(_symbol))
        , ptr(_ptr)
        , previous()
        , precision(_precision)

    {
        if constexpr (std::is_same<T, float>::value) {
            tolerance = 1e-09;
        } else if constexpr (std::is_same<T, double>::value) {
            tolerance = 1e-12;
        } else if constexpr (std::is_same<T, long double>::value) {
            tolerance = 1e-24;
        }
    }

    /// @brief Copy constructor.
    /// @param other The other entity to copy.
    TraceWrapper(const TraceWrapper &other) = default;

    /// @brief Move constructor.
    /// @param other The other entity to move.
    TraceWrapper(TraceWrapper &&other) noexcept = default;

    /// @brief Copy assignment operator.
    /// @param other The other entity to copy.
    /// @return A reference to this object.
    auto operator=(const TraceWrapper &other) -> TraceWrapper & = default;

    /// @brief Move assignment operator.
    /// @param other The other entity to move.
    /// @return A reference to this object.
    auto operator=(TraceWrapper &&other) noexcept -> TraceWrapper & = default;

    ~TraceWrapper() override = default;

    auto getVar() const -> std::string override;

    auto getValue() const -> std::string override;

    auto hasChanged() const -> bool override;

    void updatePrevious() override { previous = (*ptr); }

    /// @brief Changes the output precision for floating point values.
    /// @param _precision the desired output precision.
    void setPrecision(int _precision) { precision = _precision; }

    /// @brief Sets the tollerance for checking equality between floating point values.
    /// @param _tolerance the tollerance for checking equality.
    void setTolerance(double _tolerance) { tolerance = _tolerance; }

private:
    /// A pointer to the variable that has to be traced.
    pointer_type ptr;
    /// Previous value of the trace.
    value_type previous;
    /// The floating point precision.
    int precision;
    /// The tolerance used to check if two floating point values are equal.
    double tolerance{};
};

/// @brief Specialization for bool arrays.
/// @tparam N the size of the array.
template <std::size_t N>
class TraceWrapper<std::array<bool, N>> : public Trace
{
public:
    /// @brief The type of the traced variable.
    using value_type   = std::array<bool, N>;
    /// @brief The pointer type of the traced variable.
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
        : Trace(std::move(_name), std::move(_symbol))
        , ptr(_ptr)
        , previous()
    {
    }

    /// @brief Copy constructor.
    /// @param other The other entity to copy.
    TraceWrapper(const TraceWrapper &other) = default;

    /// @brief Move constructor.
    /// @param other The other entity to move.
    TraceWrapper(TraceWrapper &&other) noexcept = default;

    /// @brief Copy assignment operator.
    /// @param other The other entity to copy.
    /// @return A reference to this object.
    auto operator=(const TraceWrapper &other) -> TraceWrapper & = default;

    /// @brief Move assignment operator.
    /// @param other The other entity to move.
    /// @return A reference to this object.
    auto operator=(TraceWrapper &&other) noexcept -> TraceWrapper & = default;

    ~TraceWrapper() override = default;

    auto getVar() const -> std::string override;

    auto getValue() const -> std::string override;

    auto hasChanged() const -> bool override;

    void updatePrevious() override { previous = (*ptr); }
};

// ----------------------------------------------------------------------------
// Provides specific definition.
template <>
auto TraceWrapper<bool>::getVar() const -> std::string
{
    return "$var integer 1 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
auto TraceWrapper<int8_t>::getVar() const -> std::string
{
    return "$var integer  8 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
auto TraceWrapper<int16_t>::getVar() const -> std::string
{
    return "$var integer 16 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
auto TraceWrapper<int32_t>::getVar() const -> std::string
{
    return "$var integer 32 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
auto TraceWrapper<int64_t>::getVar() const -> std::string
{
    return "$var integer 64 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
auto TraceWrapper<uint8_t>::getVar() const -> std::string
{
    return "$var integer  8 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
auto TraceWrapper<uint16_t>::getVar() const -> std::string
{
    return "$var integer 16 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
auto TraceWrapper<uint32_t>::getVar() const -> std::string
{
    return "$var integer 32 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
auto TraceWrapper<uint64_t>::getVar() const -> std::string
{
    return "$var integer 64 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
auto TraceWrapper<float>::getVar() const -> std::string
{
    return "$var real 32 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
auto TraceWrapper<double>::getVar() const -> std::string
{
    return "$var real 64 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
auto TraceWrapper<long double>::getVar() const -> std::string
{
    return "$var real 64 " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <>
auto TraceWrapper<std::vector<bool>>::getVar() const -> std::string
{
    return "$var wire " + std::to_string(ptr->size()) + " " + this->getSymbol() + " " + this->getName() + " $end\n";
}

template <std::size_t N>
auto TraceWrapper<std::array<bool, N>>::getVar() const -> std::string
{
    return "$var wire " + std::to_string(N) + " " + this->getSymbol() + " " + this->getName() + " $end\n";
}

// ----------------------------------------------------------------------------
// Provides specific changing check.
template <>
auto TraceWrapper<bool>::hasChanged() const -> bool
{
    return (previous != (*ptr));
}

template <>
auto TraceWrapper<int8_t>::hasChanged() const -> bool
{
    return (previous != (*ptr));
}

template <>
auto TraceWrapper<int16_t>::hasChanged() const -> bool
{
    return (previous != (*ptr));
}

template <>
auto TraceWrapper<int32_t>::hasChanged() const -> bool
{
    return (previous != (*ptr));
}

template <>
auto TraceWrapper<int64_t>::hasChanged() const -> bool
{
    return (previous != (*ptr));
}

template <>
auto TraceWrapper<uint8_t>::hasChanged() const -> bool
{
    return (previous != (*ptr));
}

template <>
auto TraceWrapper<uint16_t>::hasChanged() const -> bool
{
    return (previous != (*ptr));
}

template <>
auto TraceWrapper<uint32_t>::hasChanged() const -> bool
{
    return (previous != (*ptr));
}

template <>
auto TraceWrapper<uint64_t>::hasChanged() const -> bool
{
    return (previous != (*ptr));
}

template <>
auto TraceWrapper<float>::hasChanged() const -> bool
{
    return !is_equal(previous, (*ptr), tolerance);
}

template <>
auto TraceWrapper<double>::hasChanged() const -> bool
{
    return !is_equal(previous, (*ptr), tolerance);
}

template <>
auto TraceWrapper<long double>::hasChanged() const -> bool
{
    return !is_equal(previous, (*ptr), tolerance);
}

template <>
auto TraceWrapper<std::vector<bool>>::hasChanged() const -> bool
{
    auto it_prev = previous.cbegin();
    auto it_curr = ptr->cbegin();
    while ((it_prev != previous.cend()) && (it_curr != ptr->cend())) {
        if ((*it_curr) != (*it_prev)) {
            return true;
        }
        ++it_prev, ++it_curr;
    }
    return false;
}

template <std::size_t N>
auto TraceWrapper<std::array<bool, N>>::hasChanged() const -> bool
{
    auto it_prev = previous.cbegin();
    auto it_curr = ptr->cbegin();
    while ((it_prev != previous.cend()) && (it_curr != ptr->cend())) {
        if ((*it_curr) != (*it_prev)) {
            return true;
        }
        ++it_prev, ++it_curr;
    }
    return false;
}

// ----------------------------------------------------------------------------
// Provides specific values.
template <>
auto TraceWrapper<bool>::getValue() const -> std::string
{
    return std::string((*ptr) ? "b1" : "b0") + this->getSymbol() + "\n";
}

template <>
auto TraceWrapper<int8_t>::getValue() const -> std::string
{
    return "b" + utility::dec_to_binary(*ptr, int8_t(8)) + " " + this->getSymbol() + "\n";
}

template <>
auto TraceWrapper<int16_t>::getValue() const -> std::string
{
    return "b" + utility::dec_to_binary(*ptr, int16_t(16)) + " " + this->getSymbol() + "\n";
}

template <>
auto TraceWrapper<int32_t>::getValue() const -> std::string
{
    return "b" + utility::dec_to_binary(*ptr, int32_t(32)) + " " + this->getSymbol() + "\n";
}

template <>
auto TraceWrapper<int64_t>::getValue() const -> std::string
{
    return "b" + utility::dec_to_binary(*ptr, int64_t(64)) + " " + this->getSymbol() + "\n";
}

template <>
auto TraceWrapper<uint8_t>::getValue() const -> std::string
{
    return "b" + utility::dec_to_binary(*ptr, uint8_t(8)) + " " + this->getSymbol() + "\n";
}

template <>
auto TraceWrapper<uint16_t>::getValue() const -> std::string
{
    return "b" + utility::dec_to_binary(*ptr, uint16_t(16)) + " " + this->getSymbol() + "\n";
}

template <>
auto TraceWrapper<uint32_t>::getValue() const -> std::string
{
    return "b" + utility::dec_to_binary(*ptr, uint32_t(32)) + " " + this->getSymbol() + "\n";
}

template <>
auto TraceWrapper<uint64_t>::getValue() const -> std::string
{
    return "b" + utility::dec_to_binary(*ptr, uint64_t(64)) + " " + this->getSymbol() + "\n";
}

template <>
auto TraceWrapper<float>::getValue() const -> std::string
{
    std::ostringstream oss;
    oss << std::scientific << std::setprecision(precision) << "r" << *ptr << " " << this->getSymbol() << "\n";
    return oss.str();
}

template <>
auto TraceWrapper<double>::getValue() const -> std::string
{
    std::ostringstream oss;
    oss << std::scientific << std::setprecision(precision) << "r" << *ptr << " " << this->getSymbol() << "\n";
    return oss.str();
}

template <>
auto TraceWrapper<long double>::getValue() const -> std::string
{
    std::ostringstream oss;
    oss << std::scientific << std::setprecision(precision) << "r" << *ptr << " " << this->getSymbol() << "\n";
    return oss.str();
}

template <>
auto TraceWrapper<std::vector<bool>>::getValue() const -> std::string
{
    return "b" + utility::vector_to_binary(*ptr) + " " + this->getSymbol() + "\n";
}

template <std::size_t N>
auto TraceWrapper<std::array<bool, N>>::getValue() const -> std::string
{
    return "b" + utility::array_to_binary(*ptr) + " " + this->getSymbol() + "\n";
}

} // namespace cpptracer
