/// @file tracer.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief Contains the tracer class.

#pragma once

#include "compression.hpp"
#include "timeScale.hpp"
#include "utilities.hpp"
#include "colors.hpp"
#include "trace.hpp"

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

namespace cpptracer
{

class Scope {
public:
    std::string name;
    std::vector<Trace *> traceList;
    std::vector<Scope> subscopes;
    Scope *parentScope;

    /// @brief Constructor.
    Scope(std::string const &_name)
        : name(_name),
          traceList(),
          subscopes(),
          parentScope()
    {
        // Nothing to do.
    }

    inline void printScope(std::ostringstream &stream) const
    {
        stream << "$scope module " << name << " $end\n";
        for (auto const &trace : traceList) {
            stream << "    " << trace->getVar();
        }
        for (auto const &subscope : subscopes) {
            subscope.printScope(stream);
        }
        stream << "$upscope $end\n";
    }

    inline void deleteTraces()
    {
        for (auto trace : traceList)
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
    Tracer(std::string const &_filename,
           TimeScale const &_timescale)
        : filename(_filename),
          outbuffer(),
          scopeRoot("CPP"),
          current_scope(&scopeRoot),
          timescale(_timescale),
          sampling(_timescale),
          firstDump(true),
          next_sample(),
          compressTraces()
    {
        scopeRoot.parentScope = &scopeRoot;
    }

    /// @brief Destructor.
    ~Tracer()
    {
        // Delete the traces.
        scopeRoot.deleteTraces();
        // Close the output file.
        this->closeTrace();
    }

    inline void setSampling(TimeScale const &_sampling)
    {
        sampling = _sampling;
    }

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
        outbuffer << "    " + this->getDateTime() + "\n";
        outbuffer << "$end\n";
        outbuffer << "$version\n";
        outbuffer << "    Tracer "
                  << VARIABLE_TRACER_MAJOR << "."
                  << VARIABLE_TRACER_MINOR << "."
                  << VARIABLE_TRACER_PATCH
                  << " - By Galfurian --- Mar 30, 2016\n";
        outbuffer << "$end\n";
        outbuffer << "$timescale\n";
        outbuffer << "    " +
                         std::to_string(static_cast<int>(timescale.getBase()));
        outbuffer << timescale.getMagnitudeString() + "\n";
        outbuffer << "$end\n";

        scopeRoot.printScope(outbuffer);

        outbuffer << "$enddefinitions $end\n";
    }

    void addScope(std::string const &scopeName)
    {
        assert(current_scope && "There is no current scope.");
        assert(current_scope->parentScope && "Current scope has no parent.");
        auto parentScope = current_scope->parentScope;
        parentScope->subscopes.emplace_back(Scope(scopeName));
        parentScope->subscopes.back().parentScope = parentScope;
        current_scope                             = &parentScope->subscopes.back();
    }

    void addSubScope(std::string const &scopeName)
    {
        assert(current_scope && "There is no current scope.");
        current_scope->subscopes.emplace_back(Scope(scopeName));
        current_scope->subscopes.back().parentScope = current_scope;
        current_scope                               = &current_scope->subscopes.back();
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
    template <typename T>
    void addTrace(T &variable, const std::string &name)
    {
        assert(current_scope && "There is no current scope.");
        current_scope->traceList.push_back(
            new TraceWrapper<T>(
                name, Tracer::getUniqueName(), &variable));
    }

    /// @brief Updates the trace file with the current variable values.
    /// @param t The time at which the traces have been updated.
    void updateTrace(const double &t)
    {
        // --------------------------------------------------------------------
        // TIME
        // --------------------------------------------------------------------
        if (firstDump)
            outbuffer << "$dumpvars\n";
        else if (!this->changed())
            return;
        else if (next_sample > t)
            return;
        else
            outbuffer << '#' << get_time<uint64_t>(t) << "\n";
        // --------------------------------------------------------------------
        // VALUES
        // --------------------------------------------------------------------
        this->updateTraceRecursive(scopeRoot);
        // --------------------------------------------------------------------
        // END
        // --------------------------------------------------------------------
        if (firstDump) {
            outbuffer << "$end\n";
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
            saved -= utility::getPercent(compressed.capacity(),
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
    template <typename T>
    inline T get_time(long double const &t) const
    {
        return static_cast<T>(t / timescale.getMagnitude());
    }

    /// @brief Provides the current date.
    std::string getDateTime()
    {
        time_t rawtime;
        struct tm *timeinfo;
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
        static std::uniform_int_distribution<size_t> dis(0, characters - 1);
        // Establish a new seed.
        std::string symbol;
        do {
            symbol.clear();
            for (unsigned int it = 0; it < length; ++it) {
                symbol += alphanum[dis(gen) % (sizeof(alphanum) - 1)];
            }
        } while (!usedSymbols.insert(symbol).second);
        return symbol;
    }

    inline void updateTraceRecursive(Scope &scope)
    {
        for (auto const &trace : scope.traceList) {
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

    inline bool changedRecursive(Scope const &scope) const
    {
        for (auto const &trace : scope.traceList) {
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
