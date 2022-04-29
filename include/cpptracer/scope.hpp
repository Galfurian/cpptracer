/// @file scope.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief

#pragma once

#include "trace.hpp"

namespace cpptracer
{

/// @brief Hierarchical group of traces.
class Scope {
public:
    /// Name of the scope.
    std::string name;
    /// List of traces inside the scope.
    std::vector<Trace *> traces;
    /// List of subscopes.
    std::vector<Scope *> subscopes;
    /// Pointer to the parent scope, if null this is the root.
    Scope *parent;

    /// @brief Construct a new scope with the given name.
    /// @param _name name of the scope.
    Scope(std::string _name)
        : name(std::move(_name)),
          traces(),
          subscopes(),
          parent()
    {
        // Nothing to do.
    }

    ~Scope()
    {
        for (auto trace : traces)
            delete trace;
        for (auto subscope : subscopes)
            delete subscope;
    }

    /// @brief Prints the scope header on the output stream.
    /// @param stream the output stream.
    inline void printScopeHeader(std::ostringstream &stream) const
    {
        stream << "$scope module " << name << " $end\n";
        for (auto trace : traces)
            stream << "    " << trace->getVar();
        for (auto subscope : subscopes)
            subscope->printScopeHeader(stream);
        stream << "$upscope $end\n";
    }
};

} // namespace cpptracer
