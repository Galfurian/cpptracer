#include "cpptracer/tracer.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846 /* pi */
#endif

int main(int, char **)
{
    // Define simulated time and timestep of the simulation.
    cpptracer::TimeScale simulatedTime(50, cpptracer::TimeUnit::SEC);
    cpptracer::TimeScale timeStep(1, cpptracer::TimeUnit::SEC);
    // Create the variables that have to be traced.
    // Floating Point (FP)
    long double _long_double = 1;
    double _double           = 1;
    float _float             = 1;
    // Unsigned (UINT)
    std::uint64_t _uint64_t = 0;
    std::uint32_t _uint32_t = 0;
    std::uint16_t _uint16_t = 0;
    std::uint8_t _uint8_t   = 0;
    // Signed (INT)
    std::int64_t _int64_t = 0;
    std::int32_t _int32_t = 0;
    std::int16_t _int16_t = 0;
    std::int8_t _int8_t   = 0;
    // Create the trace and add the variable to the trace.
    cpptracer::Tracer tracer("compressed_trace.vcd", timeStep, "root");
    // Enable the compression.
    tracer.enableCompression();
    // Add the traces.
    tracer.addTrace(_long_double, "long_double");
    tracer.addTrace(_double, "double");
    tracer.addTrace(_float, "float");
    tracer.addTrace(_uint64_t, "uint64_t");
    tracer.addTrace(_uint32_t, "uint32_t");
    tracer.addTrace(_uint16_t, "uint16_t");
    tracer.addTrace(_uint8_t, "uint8_t");
    tracer.addTrace(_int64_t, "int64_t");
    tracer.addTrace(_int32_t, "int32_t");
    tracer.addTrace(_int16_t, "int16_t");
    tracer.addTrace(_int8_t, "int8_t");
    // Create the header.
    tracer.createTrace();
    // Initialize the trace.
    for (double time = 0; time < simulatedTime; time += timeStep) {
        _long_double = std::sin(time);
        _double      = std::cos(time);
        _float *= static_cast<float>(M_PI);
        _uint8_t  = static_cast<std::uint8_t>(_uint8_t + 8);
        _uint16_t = static_cast<std::uint16_t>(_uint16_t + 16);
        _uint32_t = static_cast<std::uint32_t>(_uint32_t + 32);
        _uint64_t = static_cast<std::uint64_t>(_uint64_t + 64);
        _int8_t   = static_cast<std::int8_t>(_int8_t - 8);
        _int16_t  = static_cast<std::int16_t>(_int16_t - 16);
        _int32_t  = static_cast<std::int32_t>(_int32_t - 32);
        _int64_t  = static_cast<std::int64_t>(_int64_t - 64);
        // Update the trace.
        tracer.updateTrace(time);
    }
    // Close the trace.
    tracer.closeTrace();
    return 0;
}
