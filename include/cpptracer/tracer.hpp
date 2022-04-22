/// @file tracer.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Contains the tracer class.

#pragma once

#include "compression.hpp"
#include "timeScale.hpp"
#include "utilities.hpp"
#include "version.hpp"
#include "colors.hpp"
#include "trace.hpp"

#include <vector>
#include <fstream> // std::ofstream
#include <iomanip> // std::setprecision
#include <set>
#include <string>
#include <random>
#include <cassert>
#include <stdexcept>

namespace cpptracer
{

/// @brief Hierarchical group of traces.
class Scope {
public:
    /// Name of the scope.
    std::string name;
    /// List of traces inside the scope.
    std::vector<Trace *> trace_list;
    /// List of subscopes.
    std::vector<Scope> subscopes;
    /// Pointer to the parent scope, if null this is the root.
    Scope *parent_scope;

    /// @brief Construct a new scope with the given name.
    /// @param _name name of the scope.
    Scope(std::string const &_name)
        : name(_name),
          trace_list(),
          subscopes(),
          parent_scope()
    {
        // Nothing to do.
    }

    /// @brief Prints the scope header on the output stream.
    /// @param stream the output stream.
    inline void printScopeHeader(std::ostringstream &stream) const
    {
        stream << "$scope module " << name << " $end\n";
        for (auto const &trace : trace_list) {
            stream << "    " << trace->getVar();
        }
        for (auto const &subscope : subscopes) {
            subscope.printScopeHeader(stream);
        }
        stream << "$upscope $end\n";
    }

    /// @brief Deletes the traces inside the scope, recursively.
    inline void deleteTraces()
    {
        for (auto trace : trace_list)
            delete (trace);
        for (auto &subscope : subscopes)
            subscope.deleteTraces();
    }
};

/// @brief C++ variable tracer.
class Tracer {
private:
    /// Name of the trace file.
    std::string filename;
    /// The output buffer.
    std::ostringstream outbuffer;
    /// The root of the scopes.
    Scope scopeRoot;
    /// Pointer to the current scope.
    Scope *current_scope;
    /// The timescale.
    TimeScale timescale;
    /// The timescale.
    TimeScale sampling;
    /// Identifies the first dump of the values.
    bool firstDump;
    /// Next sampling time.
    double next_sample;
    /// Enables traces compression.
    bool compressTraces;

public:
    /// @brief Constructor.
    /// @param _filename The name of the file.
    /// @param _timescale The timescale to use.
    /// @param root the name of the root scope.
    Tracer(std::string _filename,
           TimeScale const &_timescale,
           std::string root)
        : filename(std::move(_filename)),
          outbuffer(),
          scopeRoot(std::move(root)),
          current_scope(&scopeRoot),
          timescale(_timescale),
          sampling(_timescale),
          firstDump(true),
          next_sample(),
          compressTraces()
    {
        scopeRoot.parent_scope = &scopeRoot;
    }

    /// @brief Destructor.
    ~Tracer()
    {
        // Delete the traces.
        scopeRoot.deleteTraces();
        // Close the output file.
        this->closeTrace();
    }

    /// @brief Sets the sampling period.
    /// @param _sampling the sampling period.
    inline void setSampling(TimeScale const &_sampling)
    {
        sampling = _sampling;
    }

    /// @brief Activate compression, only if enabled.
    inline void enableCompression()
    {
#ifndef COMPRESSION_ENABLED
        std::cerr << "Cannot activate the compression without zlib.\n";
#else
        compressTraces = true;
#endif
    }

    /// @brief Creates the trace.
    void createTrace()
    {
        // Write the header.
        outbuffer << "$date\n";
        outbuffer << "    " + utility::get_date_time() + "\n";
        outbuffer << "$end\n";
        outbuffer << "$version\n";
        outbuffer << "    Tracer "
                  << VARIABLE_TRACER_MAJOR << "."
                  << VARIABLE_TRACER_MINOR << "."
                  << VARIABLE_TRACER_PATCH
                  << " - By Galfurian --- Mar 30, 2016\n";
        outbuffer << "$end\n";
        outbuffer << "$timescale\n";
        outbuffer << "    " + std::to_string(static_cast<int>(timescale.getBase()));
        outbuffer << timescale.getMagnitudeString() + "\n";
        outbuffer << "$end\n";

        scopeRoot.printScopeHeader(outbuffer);

        outbuffer << "$enddefinitions $end\n";
    }

    /// @brief Adds a new scope, as a sibling of the current scope.
    /// @param scope_name the name of the new scope.
    void addScope(std::string const &scope_name)
    {
        if (current_scope == nullptr)
            throw std::runtime_error("There is no current scope.");
        if (current_scope->parent_scope == nullptr)
            throw std::runtime_error("Current scope has no parent.");
        // Get the parent scope.
        auto parent_scope = current_scope->parent_scope;
        // Create the new scope.
        auto new_scope = Scope(scope_name);
        // Set the parent of the new scope.
        new_scope.parent_scope = parent_scope;
        // Add the new scope to the parent.
        parent_scope->subscopes.emplace_back(new_scope);
        // Set the current scope.
        current_scope = &parent_scope->subscopes.back();
    }

    /// @brief Adds a new scope, as a child of the current scope.
    /// @param scope_name the name of the new scope.
    void addSubScope(std::string const &scope_name)
    {
        if (current_scope == nullptr)
            throw std::runtime_error("There is no current scope.");
        // Create the new scope.
        auto new_scope = Scope(scope_name);
        // Set the parent of the new scope.
        new_scope.parent_scope = current_scope;
        // Add the new scope to the parent.
        current_scope->subscopes.emplace_back(new_scope);
        // Set the current scope.
        current_scope = &current_scope->subscopes.back();
    }

    /// @brief Closes the current scope.
    inline void closeScope()
    {
        if (current_scope == nullptr)
            throw std::runtime_error("There is no current scope.");
        if (current_scope->parent_scope == nullptr)
            throw std::runtime_error("Current scope has no parent.");
        // Set the parent scope as current scope.
        current_scope = current_scope->parent_scope;
    }

    /// @brief Add a variable to the list of traces.
    /// @param name     The name of the trace.
    /// @param variable The variable which has to be traced.
    template <typename T>
    void addTrace(T &variable, const std::string &name)
    {
        assert(current_scope && "There is no current scope.");
        current_scope->trace_list.push_back(new TraceWrapper<T>(name, utility::get_unique_name(3), &variable));
    }

    /// @brief Updates the trace file with the current variable values.
    /// @param t The time at which the traces have been updated.
    void updateTrace(const double &t)
    {
        // Write time.
        if (firstDump)
            outbuffer << "$dumpvars\n";
        else if (!this->changed())
            return;
        else if (next_sample > t)
            return;
        else
            outbuffer << '#' << this->getScaledTime<uint64_t>(t) << "\n";
        // Write the values.
        this->updateTraceRecursive(scopeRoot);
        // Write the closure.
        if (firstDump) {
            outbuffer << "$end\n";
            firstDump = false;
        }
        // Set the time of the next sample.
        next_sample += sampling.getValue();
    }

    /// @brief Checks if some value has changed.
    inline bool changed() const
    {
        return this->changedRecursive(scopeRoot);
    }

    /// @brief Closes the trace file.
    inline void closeTrace()
    {
        // The output file.
        std::ofstream outfile;
        if (compressTraces) {
#ifdef COMPRESSION_ENABLED
            filename += ".gz";
#endif
        }
        outfile.open(filename, std::ios_base::trunc);
        if (!outfile.is_open()) {
            std::cerr << "Failed to open the trace file'" << filename << "'\n";
            return;
        }
#ifdef COMPRESSION_ENABLED
        if (compressTraces) {
            // Log the compression start.
            std::cout << KYEL << "Compressing traces..." << KRST << "\n";
            // Save the original trace and the compressed trace.
            std::string trace      = outbuffer.str();
            std::string compressed = compression::compress(trace);
            // Write the trace to file.
            outfile << compressed;
            // Compute the saved space.
            auto saved = 100.0;
            saved -= utility::get_percent(compressed.capacity(),
                                          trace.capacity());
            // Log the compression statistics.
            std::cout << KYEL << "Compression completed " << KRST << "\n"
                      << std::setprecision(2)
                      << "Original size   = "
                      << trace.capacity() << " bytes\n"
                      << "Compressed size = "
                      << compressed.capacity() << " bytes\n"
                      << "Saved space = " << saved << "%\n";
        }
#else
        outfile << outbuffer.str();
#endif
        // Close the output file.
        outfile.close();
    }

private:
    /// @brief Scales the given time to the current magnitude.
    /// @param t the input time.
    /// @return the scaled time.
    template <typename T>
    inline T getScaledTime(long double const &t) const
    {
        return static_cast<T>(t / timescale.getMagnitude());
    }

    /// @brief Issue each trace to save the current value as `previous value`.
    /// @param scope the scope from which we start the update.
    inline void updateTraceRecursive(Scope &scope)
    {
        for (auto const &trace : scope.trace_list) {
            if (trace->hasChanged() || firstDump) {
                // Print the trace.
                outbuffer << trace->getValue();
                // Update previous value.
                trace->updatePrevious();
            }
        }
        for (auto &subscope : scope.subscopes) {
            this->updateTraceRecursive(subscope);
        }
    }

    /// @brief Checks if at least one variable has changed inside/below a scope.
    /// @param scope the scope from which we start the check.
    inline bool changedRecursive(Scope const &scope) const
    {
        for (auto const &trace : scope.trace_list) {
            if (trace->hasChanged())
                return true;
        }
        for (auto &subscope : scope.subscopes) {
            if (this->changedRecursive(subscope))
                return true;
        }
        return false;
    }
};

} // namespace cpptracer
