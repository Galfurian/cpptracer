#include "cpptracer/tracer.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846 /* pi */
#endif

int main(int, char **)
{
    // Define simulated time and timestep of the simulation.
    cpptracer::TimeScale simulatedTime(50, cpptracer::TimeUnit::SEC);
    cpptracer::TimeScale timeStep(1, cpptracer::TimeUnit::SEC);

    // Signed (INT)
    std::int64_t _int64_t = 0;
    std::int32_t _int32_t = 0;
    std::int16_t _int16_t = 0;
    std::int8_t _int8_t   = 0;

    // Create the trace and add the variable to the trace.
    cpptracer::Tracer tracer("scope.vcd", timeStep, "CPP");

    // The root scope is "CPP".

    // Here we add the scope INT.
    tracer.addScope("INT"); // (Current Scope: CPP/INT)
    // Here we define a sub-scope.
    tracer.addSubScope("64_32_BIT"); // (Current Scope: CPP/INT/64_32_BIT)
    tracer.addTrace(_int64_t, "int64_t");
    tracer.addTrace(_int32_t, "int32_t");
    // Then we add a scope.
    tracer.addScope("16_8_bit"); // (Current Scope: CPP/INT/16_8_bit)
    tracer.addTrace(_int16_t, "int16_t");
    tracer.addTrace(_int8_t, "int8_t");
    // Here it closes the current sub-scope.
    tracer.closeScope(); // (Current Scope: CPP/INT)
    tracer.closeScope(); // (Current Scope: CPP)

    tracer.createTrace();
    // Initialize the trace.
    for (double time = 0; time < simulatedTime; time += timeStep) {
        _int8_t  = static_cast<std::int8_t>(_int8_t - 8);
        _int16_t = static_cast<std::int16_t>(_int16_t - 16);
        _int32_t = static_cast<std::int32_t>(_int32_t - 32);
        _int64_t = static_cast<std::int64_t>(_int64_t - 64);
        // Update the trace.
        tracer.updateTrace(time);
    }
    // Close the trace.
    tracer.closeTrace();
    return 0;
}
