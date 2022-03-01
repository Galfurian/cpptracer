#include "cpptracer/variableTracer.hpp"

class SinSource {
private:
    /// The sinusoid offset.
    double offset;
    /// The sinusoid amplitude.
    double amplitude;
    /// The sinusoid frequency.
    double frequency;

public:
    /// The output value.
    double out;

    /// @brief Constructor.
    SinSource(const double &_offset,
              const double &_amplitude,
              const double &_frequency)
        : offset(_offset),
          amplitude(_amplitude),
          frequency(_frequency),
          out()
    {
        // Nothing to do.
    }

    /// @brief Destructor.
    ~SinSource() = default;

    /// @brief Advance the sinusoid.
    /// @param t The current time.
    /// @return The current value.
    inline double compute(const double &t)
    {
        out = offset + amplitude * std::sin(2 * M_PI * frequency * t);
        return out;
    }
};

int main(int, char **)
{
    // Define simulated time and timestep of the simulation.
    TimeScale simulatedTime(50, TimeScale::SEC);
    TimeScale timeStep(1, TimeScale::SEC);

    // Create the variables that have to be traced.
    // Floating Point (FP)
    long double _long_double = 1;
    double _double           = 1;
    float _float             = 1;

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
    SinSource sinSource(0, 1, 0.5);

    // Auxiliary variables.
    double first  = 0;
    double second = 1;

    // Create the trace and add the variable to the trace.
    VariableTracer trace("trace.vcd", timeStep);
    // Enable the compression.
    trace.enableCompression();

    // The root scope is "CPP".

    // Here we add a sub-scope FP.
    trace.addSubScope("FP"); // (Current Scope: CPP/FP)
    trace.addTrace(_long_double, "long_double");
    trace.addTrace(_double, "double");
    trace.addTrace(_float, "float");
    // Here we close the sub-scope.
    trace.closeScope(); //  (Current Scope: CPP)

    // Here we add the scope UINT, since the current scope is the root, this
    // scope will be place under the root. In general nothing can be at the
    // same level as CPP.
    trace.addScope("UINT"); // (Current Scope: CPP/UINT)
    trace.addTrace(_uint64_t, "uint64_t");
    trace.addTrace(_uint32_t, "uint32_t");
    trace.addTrace(_uint16_t, "uint16_t");
    trace.addTrace(_uint8_t, "uint8_t");
    // Here we don't close the scope.

    // Here we add the scope WAVE, which will be placed at the same level of
    // the previous scope.
    trace.addScope("WAVE"); // (Current Scope: CPP/WAVE)
    trace.addTrace(sinSource.out, "Sinusoid");
    trace.closeScope(); // (Current Scope: CPP)

    // Here we add the scope INT.
    trace.addScope("INT"); // (Current Scope: CPP/INT)
    // Here we define a sub-scope.
    trace.addSubScope("64_32_BIT"); // (Current Scope: CPP/INT/64_32_BIT)
    trace.addTrace(_int64_t, "int64_t");
    trace.addTrace(_int32_t, "int32_t");
    // Then we add a scope.
    trace.addScope("16_8_bit"); // (Current Scope: CPP/INT/16_8_bit)
    trace.addTrace(_int16_t, "int16_t");
    trace.addTrace(_int8_t, "int8_t");
    // Here it closes the current sub-scope.
    trace.closeScope(); // (Current Scope: CPP/INT)
    trace.closeScope(); // (Current Scope: CPP)

    // Here we close the scopes.
    trace.closeScope(); // (Current Scope: CPP)
    // The root scope cannot be closed, it will not complain.
    trace.closeScope(); // (Current Scope: CPP)

    trace.createTrace();
    // Initialize the trace.
    for (double time = 0; time < simulatedTime; time += timeStep) {
        if (time <= 1) {
            _long_double = _double = time;
        } else {
            _long_double = _double = first + second;
            first                  = second;
            second                 = _double;
        }
        _float *= static_cast<float>(M_PI);

        _uint8_t  = static_cast<uint8_t>(_uint8_t + 8);
        _uint16_t = static_cast<uint16_t>(_uint16_t + 16);
        _uint32_t += 32u;
        _uint64_t += 64UL;

        _int8_t  = static_cast<int8_t>(_int8_t - 8);
        _int16_t = static_cast<int16_t>(_int16_t - 16);
        _int32_t -= 32;
        _int64_t -= 64;
        sinSource.compute(time);
        // Update the trace.
        trace.updateTrace(time);
    }
    // Close the trace.
    trace.closeTrace();
}
