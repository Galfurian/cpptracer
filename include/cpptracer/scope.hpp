/// @file scope.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#pragma once

#include "trace.hpp"

#include <memory>

namespace cpptracer
{

/// @brief Hierarchical group of traces.
class Scope
{
public:
    /// Name of the scope.
    std::string name;
    /// List of traces inside the scope.
    std::vector<std::shared_ptr<Trace>> traces;
    /// List of subscopes.
    std::vector<std::shared_ptr<Scope>> subscopes;
    /// Pointer to the parent scope, if null this is the root.
    std::weak_ptr<Scope> parent;

    /// @brief Construct a new scope with the given name.
    /// @param _name name of the scope.
    Scope(std::string _name)
        : name(std::move(_name))
    {
        // Nothing to do.
    }

    /// @brief Move constructor.
    /// @param other The other entity to move.
    Scope(Scope &&other) noexcept
        : name(std::move(other.name))
        , traces(std::move(other.traces))
        , subscopes(std::move(other.subscopes))
        , parent(std::move(other.parent))
    {
    }

    /// @brief Move assignment operator.
    /// @param other The other entity to move.
    /// @return A reference to this object.
    auto operator=(Scope &&other) noexcept -> Scope &
    {
        if (this == &other) {
            return *this;
        }
        name      = std::move(other.name);
        traces    = std::move(other.traces);
        subscopes = std::move(other.subscopes);
        parent    = other.parent;
        other.parent.reset();
        return *this;
    }

    /// @brief Copy constructor.
    /// @param other The other entity to copy.
    Scope(const Scope &other) = delete;

    /// @brief Copy assignment operator.
    /// @param other The other entity to copy.
    /// @return A reference to this object.
    auto operator=(const Scope &other) -> Scope & = delete;

    /// @brief Destructor.
    ~Scope() = default;

    /// @brief Prints the scope header on the output stream.
    /// @param stream the output stream.
    void printScopeHeader(std::ostringstream &stream) const
    {
        stream << "$scope module " << name << " $end\n";
        for (const auto &trace : traces) {
            stream << "    " << trace->getVar();
        }
        for (const auto &subscope : subscopes) {
            subscope->printScopeHeader(stream);
        }
        stream << "$upscope $end\n";
    }
};

} // namespace cpptracer
