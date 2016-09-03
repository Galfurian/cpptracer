/// @file   variableTracer.cpp
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

#include "variableTracer.hpp"

VariableTracer::VariableTracer(std::string _filename, TimeScale _timescale) :
        filename(_filename),
        outfile(),
        traceList(),
        firstSample(true),
        timescale(_timescale),
        nameGenerator()
{
    // Nothing to do.
}

VariableTracer::~VariableTracer()
{
    // Delete the traces.
    for (unsigned int it = 0; it < traceList.size(); ++it)
    {
        delete (traceList.at(it));
    }
    // Close the output file.
    if (outfile.is_open())
    {
        outfile.close();
    }
}

void VariableTracer::createTrace()
{
    // Open the file.
    outfile.open(filename.c_str(), std::fstream::out | std::fstream::trunc);

    // Write the header.
    outfile << "$date" << std::endl;
    outfile << "    " << getDateTime() << std::endl;
    outfile << "$end" << std::endl << std::endl;
    outfile << "$version" << std::endl;
    outfile << "    VariableTracer 2.1.0 - By Galfurian --- Mar 30, 2016" << std::endl;
    outfile << "$end" << std::endl << std::endl;
    outfile << "$timescale" << std::endl;
    outfile << "    " << timescale.getValue() << timescale.scaleToString() << std::endl;
    outfile << "$end" << std::endl << std::endl;
    outfile << "$scope" << std::endl;
    outfile << "    module CPP" << std::endl;
    outfile << "$end" << std::endl << std::endl;
    for (unsigned int it = 0; it < traceList.size(); ++it)
    {
        outfile << "$var real 1 " << traceList.at(it)->getSymbol();
        outfile << " " << traceList.at(it)->getName() << " $end" << std::endl;
    }
    outfile << std::endl;
    outfile << "$upscope $end" << std::endl << std::endl;
    outfile << "$enddefinitions $end" << std::endl << std::endl;
    outfile << std::setprecision(18);
    outfile << std::scientific;
}

void VariableTracer::updateTrace(double currentTime)
{
    bool changed = false;
    for (unsigned int it = 0; it < traceList.size(); ++it)
    {
        if (traceList.at(it)->hasChanged())
        {
            changed = true;
            break;
        }
    }
    if (!changed)
    {
        return;
    }

    outfile << "#" << static_cast<long unsigned int>(currentTime * timescale.getMagnitude()) << std::endl;
    for (unsigned int it = 0; it < traceList.size(); ++it)
    {
        // Skip unchanged signals, unless it is the first round.
        if (!firstSample && !traceList.at(it)->hasChanged()) continue;
        // Print the trace.
        outfile << "r" << traceList.at(it)->getValue() << " " << traceList.at(it)->getSymbol() << std::endl;
        // Update previous value.
        traceList.at(it)->updatePrevious();
    }
    // Change the firstSample flag.
    if (firstSample) firstSample = false;
}

void VariableTracer::closeTrace()
{
    outfile.close();
}

std::string VariableTracer::getDateTime()
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
