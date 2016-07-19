/// @file   CPPTracer.hpp
/// @brief  Implementation of a simple tracer for c++ variables.
/// @author Enrico Fraccaroli
/// @date   Nov 17 2015
/// @copyright
/// Copyright (c) 2015, 2016 Enrico Fraccaroli <enrico.fraccaroli@gmail.com>
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

#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

/// @brief A name generator for traces.
class TraceNameGenerator
{
    private:
        /// Lenght of the name.
        size_t length;
        /// Set of already used symbols.
        std::set<std::string> usedSymbols;
    public:

        /// @brief Constructor.
        /// @param _length The length of the name.
        TraceNameGenerator(const size_t & _length = 3) :
            length(_length),
            usedSymbols()
        {
            ::srand(time(0));
        }

        /// @brief Allows to generate a unique name.
        /// @return The generated name.
        std::string getUniqueName()
        {
            std::string symbol;
            while (true)
            {
                static const char alphanum[] = "abcdefghijklmnopqrstuvwxyz";
                for (unsigned int it = 0; it < length; ++it)
                {
                    symbol += alphanum[::rand() % (sizeof(alphanum) - 1)];
                }
                if (!usedSymbols.insert(symbol).second)
                {
                    symbol.clear();
                }
                else
                {
                    break;
                }
            }
            return symbol;
        }
};

/// @brief Contains the information concerning a trace.
template<typename T>
class CPPTrace
{
    public:
        /// The name of the trace.
        std::string name;
        /// The symbol assigned to the trace.
        std::string symbol;
        /// A pointer to the variable that has to be traced.
        T * ptr;
        /// Previous value of the trace.
        T previous;

        /// @brief Constructor of the trace.
        /// @param _name     The name of the trace.
        /// @param _symbol   The symbol to assign.
        /// @param _ptr      Pointer to the variable.
        /// @param _previous Previous value.
        CPPTrace(std::string _name, std::string _symbol, T * _ptr, T _previous = 0) :
            name(_name),
            symbol(_symbol),
            ptr(_ptr),
            previous(_previous)
        {
            // Nothing to do.
        }

        /// @brief Provides the name of the trace.
        std::string getName()
        {
            return name;
        }

        /// @brief Provides the symbol of the trace.
        std::string getSymbol()
        {
            return symbol;
        }

        /// @brief Provides the current value of the trace.
        T getValue()
        {
            return (*ptr);
        }

        /// @brief Checks if the value has changed w.r.t. the previous one.
        /// @return <b>True</b> if the value has changed,<br>
        ///         <b>False</b> otherwise.
        bool hasChanged()
        {
            return (previous != this->getValue());
        }

        /// @brief Updates the previous value with the current value.
        void updatePrevious()
        {
            previous = this->getValue();
        }
};

/// @brief Class used to store timing dimensions.
class TimeScale
{
    public:
        /// Possible granularity of time.
        typedef enum Enums
        {
            SEC,
            MS,
            US,
            NS,
            PS
        } Enum;

    private:
        /// Value of the timescale.
        double timeValue;
        /// The order of magnitute of the timescale.
        Enum scaleValue;

    public:
        /// @brief Constructor.
        /// @param _timeValue  The value of time.
        /// @param _scaleValue The scale of the trace.
        TimeScale(double _timeValue, Enum _scaleValue) :
            timeValue(_timeValue),
            scaleValue(_scaleValue)
        {
            // Nothing to do.
        }

        /// @brief Return the scale dimension.
        /// @return The value of the scale.
        double getValue()
        {
            return timeValue;
        }

        /// @brief Return the scale dimension.
        /// @return The value of the scale.
        double getScaledValue()
        {
            return timeValue / this->getScaleMagnitude();
        }

        /// @brief Return the string value for the scale.
        /// @return The scale as string.
        std::string scaleToString()
        {
            if (scaleValue == SEC) return "s";
            if (scaleValue == MS) return "ms";
            if (scaleValue == US) return "us";
            if (scaleValue == NS) return "ns";
            if (scaleValue == PS) return "ps";
            return "s";
        }

        /// @brief Return the dimension of the scale.
        /// @return The magnitude of the scale.
        unsigned long int getScaleMagnitude()
        {
            if (scaleValue == SEC) return 1;
            if (scaleValue == MS) return 1e03;
            if (scaleValue == US) return 1e06;
            if (scaleValue == NS) return 1e09;
            if (scaleValue == PS) return 1e12;
            return 1;
        }
};

/// @brief C++ variable tracer.
template<typename T>
class CPPTracer
{
    private:
        /// Name of the trace file.
        std::string filename;
        /// The output file.
        std::ofstream outfile;
        /// The list of traces.
        std::vector<CPPTrace<T> > traceList;
        /// Flag which, during the update phase, identifies the first sample.
        bool firstSample;
        /// The timescale.
        TimeScale timescale;
        /// A name generator for traces.
        TraceNameGenerator nameGenerator;

    public:
        /// @brief Constructor.
        /// @param _filename The name of the file.
        /// @param _timescale The timescale to use.
        CPPTracer(std::string _filename, TimeScale _timescale) :
            filename(_filename),
            outfile(),
            traceList(),
            firstSample(true),
            timescale(_timescale),
            nameGenerator()
        {
        }

        /// @brief Destructor.
        ~CPPTracer()
        {
            // Check if the output file is closed.
            if (outfile.is_open())
            {
                outfile.close();
            }
        }

        /// @brief Creates the trace.
        void createTrace()
        {
            // Open the file.
            outfile.open(filename.c_str(), std::fstream::out | std::fstream::trunc);

            // Write the header.
            outfile << "$date" << std::endl;
            outfile << "    " << getDateTime() << std::endl;
            outfile << "$end" << std::endl << std::endl;
            outfile << "$version" << std::endl;
            outfile << "    CPPTracer 2.1.0 - By Galfurian --- Mar 30, 2016" << std::endl;
            outfile << "$end" << std::endl << std::endl;
            outfile << "$timescale" << std::endl;
            outfile << "    " << timescale.getValue() << timescale.scaleToString() << std::endl;
            outfile << "$end" << std::endl << std::endl;
            outfile << "$scope" << std::endl;
            outfile << "    module CPP" << std::endl;
            outfile << "$end" << std::endl << std::endl;
            for (unsigned int it = 0; it < traceList.size(); ++it)
            {
                CPPTrace<T> trace = traceList.at(it);
                outfile << "$var real 1 " << trace.symbol << " " << trace.name << " $end" << std::endl;
            }
            outfile << std::endl;
            outfile << "$upscope $end" << std::endl << std::endl;
            outfile << "$enddefinitions $end" << std::endl << std::endl;
            outfile << std::setprecision(18);
            outfile << std::fixed;
        }

        /// @brief Add a variable to the list of traces.
        /// @param name     The name of the trace.
        /// @param variable The variable which has to be traced.
        void addTrace(std::string name, T * variable)
        {
            traceList.push_back(CPPTrace<T>(name, nameGenerator.getUniqueName(), variable));
        }

        /// @brief Updates the trace file with the current variable values.
        /// @param currentTime The time at which the traces have been updated.
        void updateTrace(double currentTime)
        {
            bool changed = false;
            for (unsigned int it = 0; it < traceList.size(); ++it)
            {
                if (traceList.at(it).hasChanged())
                {
                    changed = true;
                    break;
                }
            }
            if (!changed)
            {
                return;
            }

            outfile << "#" << (currentTime * timescale.getScaleMagnitude()) << std::endl;
            for (unsigned int it = 0; it < traceList.size(); ++it)
            {
                // Skip unchanged signals, unless it is the first round.
                if (!firstSample && !traceList.at(it).hasChanged()) continue;
                // Print the trace.
                outfile << "r" << traceList.at(it).getValue() << " " << traceList.at(it).getSymbol() << std::endl;
                // Update previous value.
                traceList.at(it).updatePrevious();
            }
            // Change the firstSample flag.
            if (firstSample) firstSample = false;
        }

        /// @brief Closes the trace file.
        void closeTrace()
        {
            outfile.close();
        }

    private:
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
