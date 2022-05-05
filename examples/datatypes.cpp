#include "cpptracer/tracer.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846 /* pi */
#endif
#ifndef M_PIf
#define M_PIf 3.14159265358979323846f /* pi */
#endif

int main(int, char **)
{
    // Define simulated time and timestep of the simulation.
    cpptracer::TimeScale simulatedTime(50, cpptracer::TimeUnit::SEC);
    cpptracer::TimeScale timeStep(1, cpptracer::TimeUnit::SEC);

    // Create the variables that have to be traced.
    // Floating Point (FP)
    long double _long_double = 1.;
    double _double           = 1.;
    float _float             = 1.;

    // Unsigned (UINT)
    uint64_t _uint64_t = 0;
    uint32_t _uint32_t = 0;
    uint16_t _uint16_t = 0;
    uint8_t _uint8_t   = 0;

    // Signed (INT)
    int64_t _int64_t = 0;
    int32_t _int32_t = 0;
    int16_t _int16_t = 0;
    int8_t _int8_t   = 0;

    // Waves (WAVE)
    // The sinusoid offset.
    double offset = 0.0;
    // The sinusoid amplitude.
    double amplitude = 1.0;
    // The sinusoid frequency.
    double frequency = 0.1;
    // The sinusoid wave.
    double sine_wave = 0.5;

    // Create the trace and add the variable to the trace.
    cpptracer::Tracer tracer("datatypes.vcd", timeStep, "root");

    // Add traces.
    auto long_double_trace = tracer.addTrace(_long_double, "long_double");
    auto double_trace      = tracer.addTrace(_double, "double");
    auto float_trace       = tracer.addTrace(_float, "float");
    tracer.addTrace(_uint64_t, "uint64_t");
    tracer.addTrace(_uint32_t, "uint32_t");
    tracer.addTrace(_uint16_t, "uint16_t");
    tracer.addTrace(_uint8_t, "uint8_t");
    tracer.addTrace(sine_wave, "Sinusoid");
    tracer.addTrace(_int64_t, "int64_t");
    tracer.addTrace(_int32_t, "int32_t");
    tracer.addTrace(_int16_t, "int16_t");
    tracer.addTrace(_int8_t, "int8_t");

    // Create the header.
    tracer.createTrace();
    
    // Set the precision for the floating-point traces.    
    long_double_trace->setPrecision(9);
    double_trace->setPrecision(6);
    float_trace->setPrecision(3);

    // Initialize the trace.
    for (double time = 0; time < simulatedTime; time += timeStep) {
        _long_double *= M_PI;
        _double *= M_PI;
        _float *= M_PIf;

        _uint8_t += static_cast<uint8_t>(8u);
        _uint16_t += static_cast<uint16_t>(16U);
        _uint32_t += static_cast<uint32_t>(32U);
        _uint64_t += static_cast<uint64_t>(64U);

        _int8_t -= static_cast<int8_t>(8);
        _int16_t -= static_cast<int16_t>(16);
        _int32_t -= static_cast<int32_t>(32);
        _int64_t -= static_cast<int64_t>(64);

        sine_wave = offset + amplitude * std::sin(2 * M_PI * frequency * time);
        
        // Update the trace.
        tracer.updateTrace(time);
    }
    // Close the trace.
    tracer.closeTrace();
    return 0;
}
