/// @file   traceReader.cpp
/// @author Enrico Fraccaroli
/// @date   Jun 23, 2017
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

#include "traceReader.hpp"
#include <iostream>
#include <cstdlib>

TraceReader::TraceReader(std::string _filename) :
    filename(_filename),
    infile(),
    traceList()
{
    // Nothing to do.
}

TraceReader::~TraceReader()
{
    // Delete the traces.
    for (unsigned int it = 0; it < traceList.size(); ++it)
    {
        delete (traceList.at(it));
    }
    // Close the output file.
    if (infile.is_open())
    {
        infile.close();
    }
}

bool TraceReader::initializeReader()
{
    // Open the file.
    infile.open(filename.c_str(), std::fstream::in);
    if (!infile.is_open())
    {
        return false;
    }
    std::cout << "Reading the trace...\n";
    std::string line;
    while (getline(infile, line))
    {
        if (line.find("$enddefinitions") != std::string::npos)
        {
            break;
        }
        if (!BeginWith(line, "$var"))
        {
            continue;
        }
        std::vector<std::string> fields = SplitString(line, " ");
        if (fields.size() != 6)
        {
            continue;
        }
        std::string type(fields[1]);
        std::string length(fields[2]);
        std::string id(fields[3]);
        std::string name(fields[4]);
        if (type == "real")
        {
            traceList.push_back(
                new GenericTraceWrapper<double>(
                    name,
                    id,
                    new double(0)));
        }
    }
    return true;
}

void TraceReader::updateTrace()
{
    std::string line;
    while (getline(infile, line))
    {
        if (BeginWith(line, "#"))
        {
            break;
        }
        std::vector<std::string> fields = SplitString(line, " ");
        if (fields.size() != 2)
        {
            break;
        }
        for (unsigned int it = 0; it < traceList.size(); ++it)
        {
            if (traceList.at(it)->getSymbol() == fields[1])
            {
                traceList.at(it)->setValue(atof(fields[0].substr(1).c_str()));
            }
        }
    }
}

GenericTrace * TraceReader::findTrace(const std::string & name) const
{
    for (unsigned int it = 0; it < traceList.size(); ++it)
    {
        if (traceList[it]->getName() == name)
        {
            return traceList[it];
        }
    }
    return NULL;
}

void TraceReader::closeTrace()
{
    infile.close();
}

inline bool BeginWith(const std::string & source, const std::string & prefix)
{
    return source.compare(0, prefix.size(), prefix) == 0;
}

std::vector<std::string> SplitString(const std::string & source,
                                     const std::string & delimiter)
{
    std::vector<std::string> result;
    size_t pos = 0;
    std::string tmp = source;
    while ((pos = tmp.find(delimiter)) != std::string::npos)
    {
        result.push_back(tmp.substr(0, pos));
        tmp.erase(0, pos + delimiter.length());
    }
    if (!tmp.empty())
    {
        result.push_back(tmp);
    }
    return result;
}
