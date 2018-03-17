/// @file   variableTracer.hpp
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

#include "timeScale.hpp"
#include "genericTrace.hpp"
#include "traceNameGenerator.hpp"

#include <vector>
#include <fstream> // std::ofstream
#include <iomanip> // std::setprecision

/// @brief C++ variable tracer.
class VariableTracer
{
private:
    /// Name of the trace file.
    std::string filename;
    /// The output file.
    FILE * outfile;
    /// The list of traces.
    std::vector<GenericTrace *> traceList;
    /// The timescale.
    TimeScale timescale;
    /// A name generator for traces.
    TraceNameGenerator nameGenerator;
    /// Checks if the file is
    bool opened;
    /// Identifies the first dump of the values.
    bool firstDump;

public:
    /// @brief Constructor.
    /// @param _filename The name of the file.
    /// @param _timescale The timescale to use.
    VariableTracer(std::string _filename,
                   TimeScale const & _timescale) :
        filename(std::move(_filename)),
        outfile(),
        traceList(),
        timescale(_timescale),
        nameGenerator(),
        opened(),
        firstDump(true)
    {
        // Nothing to do.
    }

    /// @brief Destructor.
    ~VariableTracer()
    {
        // Delete the traces.
        for (auto it : traceList)
        {
            delete (it);
        }
        // Close the output file.
        this->closeTrace();
    }

    /// @brief Creates the trace.
    void createTrace()
    {
        // Open the file.
        outfile = fopen(filename.c_str(), "w");

        // Write the header.
        std::string header;
        header += "$date\n";
        header += "    " + this->getDateTime() + "\n";
        header += "$end\n\n";

        header += "$version\n";
        header += "    VariableTracer 2.1.0 - By Galfurian --- Mar 30, 2016\n";
        header += "$end\n\n";

        header += "$timescale\n";
        header += "    " + std::to_string(
            static_cast<int>(timescale.getTimeValue()));
        header += timescale.scaleToString() + "\n";
        header += "$end\n\n";

        header += "$scope\n";
        header += "    module CPP\n";
        header += "$end\n\n";
        for (auto const & it : traceList)
        {
            header += it->getVar();
        }
        header += "\n";
        header += "$upscope $end\n\n";
        header += "$enddefinitions $end\n";
        fputs(header.c_str(), outfile);

        opened = true;
    }

    /// @brief Add a variable to the list of traces.
    /// @param name     The name of the trace.
    /// @param variable The variable which has to be traced.
    template<typename T>
    void addTrace(T & variable, const std::string & name)
    {
        traceList.push_back(
            new GenericTraceWrapper<T>(
                name, nameGenerator.getUniqueName(), &variable));
    }

    /// @brief Updates the trace file with the current variable values.
    /// @param t The time at which the traces have been updated.
    void updateTrace(const double & t)
    {
        // Add the current time.
        if (firstDump)
        {
            fputs("$dumpvars\n", outfile);
        }
        else if (!this->changed())
        {
            return;
        }
        else
        {
            fputc('#', outfile);
            fputs(get_time(t).c_str(), outfile);
            fputc('\n', outfile);
        }
        // Dump the values.
        for (auto const & it : traceList)
        {
            // Dump only changed signals.
            if (it->hasChanged() || firstDump)
            {
                // Print the trace.
                fputs(it->getValue().c_str(), outfile);
                // Update previous value.
                it->updatePrevious();
            }
        }
        if (firstDump)
        {
            fputs("$end\n", outfile);
            firstDump = false;
        }
    }

    inline bool changed() const
    {
        for (auto const & it : traceList)
        {
            if (it->hasChanged()) return true;
        }
        return false;
    }

    /// @brief Closes the trace file.
    void closeTrace()
    {
        if (opened)
        {
            fclose(outfile);
            opened = false;
        }
    }

private:
    inline std::string get_time(double const & t) const
    {
        return std::to_string(
            static_cast<long long int>(t / timescale.getScaleValue()));
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

};
