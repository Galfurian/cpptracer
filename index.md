---
layout: default
---

# CPPTracer

[![Ubuntu](https://github.com/Galfurian/cpptracer/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/Galfurian/cpptracer/actions/workflows/ubuntu.yml)
[![Windows](https://github.com/Galfurian/cpptracer/actions/workflows/windows.yml/badge.svg)](https://github.com/Galfurian/cpptracer/actions/workflows/windows.yml)
[![MacOS](https://github.com/Galfurian/cpptracer/actions/workflows/macos.yml/badge.svg)](https://github.com/Galfurian/cpptracer/actions/workflows/macos.yml)
[![Documentation](https://github.com/Galfurian/cpptracer/actions/workflows/documentation.yml/badge.svg)](https://github.com/Galfurian/cpptracer/actions/workflows/documentation.yml)

## Overview

`cpptracer` is a C++ library for tracing variable values and generating Value
Change Dump (VCD) files. It allows you to track the changes of variables over
time and output these changes in VCD format. The library supports hierarchical
scoping of traces, compression of trace data, and fine-grained control over the
sampling period and timescale.

## Features

- **Trace Variables**: Trace changes to any variable in your program, including
  basic types, arrays, and more complex structures.
- **Hierarchical Scoping**: Organize traces in nested scopes, similar to how
  signals are grouped in VCD files.
- **Compression**: Enable optional compression for VCD files to save disk space.
- **Custom Timescales and Sampling**: Control the granularity of time and the
  sampling period of your traces.
- **Automatic VCD Generation**: Automatically output variable values in the VCD
  format, capturing their state at each timestep.
  
## Installation

Clone the repository and include the necessary header files in your C++ project.

```bash
git clone https://github.com/Galfurian/cpptracer.git
```

You can then include the `tracer.hpp` header in your C++ project:

```c++
#include <cpptracer/tracer.hpp>
```

## Example Usage

Here are some examples.

### 1. Minimal Example

Here’s a minimal example that demonstrates how to trace a simple variable and
generate a VCD file.

```cpp
#include <cpptracer/tracer.hpp>

int main()
{
    cpptracer::TimeScale timeStep(1, cpptracer::TimeUnit::SEC);
    int myVar = 42;

    cpptracer::Tracer tracer("minimal_trace.vcd", timeStep, "root");
    tracer.addTrace(myVar, "myVar");
    tracer.createTrace();

    for (double time = 0; time < 10; time += 1)
    {
        myVar = time * 2;
        tracer.updateTrace(time);
    }
    tracer.closeTrace();
    return 0;
}
```

### 2. Minimal Example with Compression

This example shows how to enable compression for the generated VCD trace.

```cpp
#include <cpptracer/tracer.hpp>

int main()
{
    cpptracer::TimeScale timeStep(1, cpptracer::TimeUnit::SEC);
    int myVar = 42;

    cpptracer::Tracer tracer("compressed_trace.vcd", timeStep, "root");
    tracer.enableCompression();  // Enable compression
    tracer.addTrace(myVar, "myVar");
    tracer.createTrace();

    for (double time = 0; time < 10; time += 1)
    {
        myVar = time * 2;
        tracer.updateTrace(time);
    }
    tracer.closeTrace();
    return 0;
}
```

### 3. Minimal Example with Scopes

This example demonstrates how to organize traces using scopes.

```cpp
#include <cpptracer/tracer.hpp>

int main()
{
    cpptracer::TimeScale timeStep(1, cpptracer::TimeUnit::SEC);
    int myVar1 = 42;
    int myVar2 = 84;
    int myVar3 = 12;

    cpptracer::Tracer tracer("scoped_trace.vcd", timeStep, "root");
    
    // Scope after: CPP/SCOPE1
    tracer.addScope("SCOPE1");
    tracer.addTrace(myVar1, "myVar1");

    // Scope after: CPP/SCOPE1/SUBSCOPE1
    tracer.addSubScope("SUBSCOPE1");
    tracer.addTrace(myVar2, "myVar2");
    tracer.closeScope(); // Closes CPP/SCOPE1/SUBSCOPE1.

    // Scope after: CPP/SCOPE2
    tracer.addScope("SCOPE2");
    tracer.addTrace(myVar3, "myVar3");
    
    // Scope after: CPP
    tracer.closeScope();

    tracer.createTrace();

    for (double time = 0; time < 10; time += 1)
    {
        myVar1 = time * 2;
        myVar2 = myVar1 * 2;
        tracer.updateTrace(time);
    }
    tracer.closeTrace();
    return 0;
}
```

## Classes

### Tracer

The main class used to manage traces and generate the VCD file.

- **addTrace**: Add a trace for a specific variable.
- **addScope**: Add a new scope to organize traces, it joints the other sibling
  scopes at the same level.
- **addSubScope**: Add a new sub-scope under the current scope.
- **updateTrace**: Update traces with the latest values at a specific time.
- **closeTrace**: Finalize the trace file and write to disk.
- **enableCompression**: Enable compression for the trace data.
  
### Trace

The base class representing a traceable variable. This class is used to track changes in variable values.

- **getVar**: Get the `$var` representation for the trace.
- **getValue**: Get the current value of the trace.
- **hasChanged**: Check if the trace value has changed.
- **updatePrevious**: Update the previous value with the current value.

### TraceWrapper

A template class that wraps a variable to be traced.

## Contributing

Feel free to fork the repository, open issues, and submit pull requests. All
contributions are welcome!

## License

Copyright (c) 2016-2025 Enrico Fraccaroli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
