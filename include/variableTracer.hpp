/// @file   variableTracer.hpp
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

#include "genericTrace.hpp"
#include "timeScale.hpp"

#include <vector>
#include <fstream> // std::ofstream
#include <iomanip> // std::setprecision
#include <set>
#include <string>
#include <random>
#include <cassert>

#define VARIABLE_TRACER_MAJOR 3
#define VARIABLE_TRACER_MINOR 0
#define VARIABLE_TRACER_PATCH 0


/// @brief C++ variable tracer.
class VariableTracer
{
private:
    typedef struct scope_t
    {
        std::string name;
        std::vector<GenericTrace *> traceList;
        std::vector<scope_t> subscopes;
        scope_t * parentScope;

        /// @brief Constructor.
        scope_t(std::string const & _name) :
            name(_name),
            traceList(),
            subscopes(),
            parentScope()
        {
            // Nothing to do.
        }

        inline void printScope(std::ofstream & stream) const
        {
            stream << "$scope module " << name << " $end\n";
            for (auto const & trace : traceList)
            {
                stream << "    " << trace->getVar();
            }
            for (auto const & subscope : subscopes)
            {
                subscope.printScope(stream);
            }
            stream << "$upscope $end\n";
        }

        inline void deleteTraces()
        {
            for (auto trace : traceList)
                delete (trace);
            for (auto & subscope : subscopes)
                subscope.deleteTraces();
        }
    } scope_t;

    /// Name of the trace file.
    std::string filename;
    /// The output file.
    std::ofstream outfile;
    /// The root of the scopes.
    scope_t scopeRoot;
    /// Pointer to the current scope.
    scope_t * current_scope;
    /// The timescale.
    TimeScale timescale;
    /// The timescale.
    TimeScale sampling;
    /// Identifies the first dump of the values.
    bool firstDump;
    /// Next sampling time.
    double next_sample;

public:
    /// @brief Constructor.
    /// @param _filename The name of the file.
    /// @param _timescale The timescale to use.
    VariableTracer(std::string const & _filename,
                   TimeScale const & _timescale) :
        filename(_filename),
        outfile(_filename),
        scopeRoot("CPP"),
        current_scope(&scopeRoot),
        timescale(_timescale),
        sampling(_timescale),
        firstDump(true),
        next_sample()
    {
        scopeRoot.parentScope = &scopeRoot;
    }

    /// @brief Destructor.
    ~VariableTracer()
    {
        // Delete the traces.
        scopeRoot.deleteTraces();
        // Close the output file.
        this->closeTrace();
    }

    inline void setSampling(TimeScale const & _sampling)
    {
        sampling = _sampling;
    }

    /// @brief Creates the trace.
    void createTrace()
    {
        // Write the header.
        outfile << "$date\n";
        outfile << "    " + this->getDateTime() + "\n";
        outfile << "$end\n\n";
        outfile << "$version\n";
        outfile << "    VariableTracer "
                << VARIABLE_TRACER_MAJOR << "."
                << VARIABLE_TRACER_MINOR << "."
                << VARIABLE_TRACER_PATCH
                << " - By Galfurian --- Mar 30, 2016\n";
        outfile << "$end\n\n";
        outfile << "$timescale\n";
        outfile <<
                "    " + std::to_string(static_cast<int>(timescale.getBase()));
        outfile << timescale.getMagnitudeString() + "\n";
        outfile << "$end\n\n";

        scopeRoot.printScope(outfile);

        outfile << "$enddefinitions $end\n";
    }

    void addScope(std::string const & scopeName)
    {
        assert(current_scope && "There is no current scope.");
        assert(current_scope->parentScope && "Current scope has no parent.");
        auto parentScope = current_scope->parentScope;
        parentScope->subscopes.emplace_back(scope_t(scopeName));
        parentScope->subscopes.back().parentScope = parentScope;
        current_scope = &parentScope->subscopes.back();
    }

    void addSubScope(std::string const & scopeName)
    {
        assert(current_scope && "There is no current scope.");
        current_scope->subscopes.emplace_back(scope_t(scopeName));
        current_scope->subscopes.back().parentScope = current_scope;
        current_scope = &current_scope->subscopes.back();
    }

    inline void closeScope()
    {
        assert(current_scope && "There is no current scope.");
        assert(current_scope->parentScope && "Current scope has no parent.");
        current_scope = current_scope->parentScope;
    }

    /// @brief Add a variable to the list of traces.
    /// @param name     The name of the trace.
    /// @param variable The variable which has to be traced.
    template<typename T>
    void addTrace(T & variable, const std::string & name)
    {
        assert(current_scope && "There is no current scope.");
        current_scope->traceList.push_back(
            new GenericTraceWrapper<T>(
                name, VariableTracer::getUniqueName(), &variable));
    }

    /// @brief Updates the trace file with the current variable values.
    /// @param t The time at which the traces have been updated.
    void updateTrace(const double & t)
    {
        // --------------------------------------------------------------------
        // TIME
        // --------------------------------------------------------------------
        if (firstDump) outfile << "$dumpvars\n";
        else if (!this->changed()) return;
        else if (next_sample > t) return;
        else outfile << '#' << get_time<uint64_t>(t) << "\n";
        // --------------------------------------------------------------------
        // VALUES
        // --------------------------------------------------------------------
        this->updateTraceRecursive(scopeRoot);
        // --------------------------------------------------------------------
        // END
        // --------------------------------------------------------------------
        if (firstDump)
        {
            outfile << "$end\n";
            firstDump = false;
        }
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
        if (outfile.is_open()) outfile.close();
    }

private:
    template<typename T>
    inline T get_time(long double const & t) const
    {
        return static_cast<T>(t / timescale.getMagnitude());
    }

    /// @brief Provides the current date.
    std::string getDateTime()
    {
        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        strftime(buffer, 80, "%b %d, %Y %I:%M:%S", timeinfo);
        std::string str(buffer);
        return str;
    }

    /// @brief Allows to generate a unique name.
    /// @return The generated name.
    static inline std::string getUniqueName()
    {
        static const size_t characters = 27;
        // Define the usable character.
        static const char alphanum[characters] = "abcdefghijklmnopqrstuvwxyz";
        /// Lenght of the name.
        static size_t length = 3;
        /// Set of already used symbols.
        static std::set<std::string> usedSymbols;
        /// Will be used to obtain a seed for the random number engine
        static std::random_device rd;
        /// Standard mersenne_twister_engine seeded with rd().
        static std::mt19937 gen(rd());
        // Create an uniform distribution.
        static std::uniform_int_distribution<int> dis(0, characters - 1);
        // Establish a new seed.
        std::string symbol;
        do
        {
            symbol.clear();
            for (unsigned int it = 0; it < length; ++it)
            {
                symbol += alphanum[dis(gen) % (sizeof(alphanum) - 1)];
            }
        } while (!usedSymbols.insert(symbol).second);
        return symbol;
    }

    inline void updateTraceRecursive(scope_t & scope)
    {
        for (auto const & trace : scope.traceList)
        {
            if (trace->hasChanged() || firstDump)
            {
                // Print the trace.
                outfile << trace->getValue();
                // Update previous value.
                trace->updatePrevious();
            }
        }
        for (auto & subscope : scope.subscopes)
        {
            this->updateTraceRecursive(subscope);
        }
    }

    inline bool changedRecursive(scope_t const & scope) const
    {
        for (auto const & trace : scope.traceList)
        {
            if (trace->hasChanged()) return true;
        }
        for (auto & subscope : scope.subscopes)
        {
            if (this->changedRecursive(subscope)) return true;
        }
        return false;
    }
};
