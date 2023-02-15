#include "cpptracer/tracer.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846 /* pi */
#endif
#ifndef M_PIf
#define M_PIf 3.14159265358979323846f /* pi */
#endif

inline bool add_bits(bool b1, bool b2, bool &carry)
{
    bool sum = (b1 ^ b2) ^ carry;
    carry    = (b1 && b2) || (b1 && carry) || (b2 && carry);
    return sum;
}

inline std::vector<bool> operator+(const std::vector<bool> &lhs, const std::vector<bool> &rhs)
{
    std::vector<bool> result(lhs.size());
    size_t N1 = lhs.size(), N2 = rhs.size();
    bool carry = false;
    for (size_t it = 0; it < result.size(); ++it)
        result[result.size() - it - 1] = add_bits(
            (it < N1) ? lhs[N1 - it - 1] : false,
            (it < N2) ? rhs[N2 - it - 1] : false, carry);
    return result;
}

template <size_t N1, size_t N2>
inline auto operator+(const std::array<bool, N1> &lhs, const std::array<bool, N2> &rhs)
{
    std::array<bool, std::max(N1, N2)> result;
    bool carry = false;
    for (size_t it = 0; it < result.size(); ++it)
        result[result.size() - it - 1] = add_bits(
            (it < N1) ? lhs[N1 - it - 1] : false,
            (it < N2) ? rhs[N2 - it - 1] : false, carry);
    return result;
}

int main(int, char **)
{
    // Define simulated time and timestep of the simulation.
    cpptracer::TimeScale simulatedTime(1000, cpptracer::TimeUnit::SEC);
    cpptracer::TimeScale timeStep(1, cpptracer::TimeUnit::SEC);

    // Create the variables that have to be traced.
    // Floating Point (FP)
    long double _long_double = 1.;
    double _double           = 1.;
    float _float             = 1.;

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

    // Waves (WAVE)
    // The sinusoid offset.
    double offset = 0.0;
    // The sinusoid amplitude.
    double amplitude = 1.0;
    // The sinusoid frequency.
    double frequency = 0.1;
    // The sinusoid wave.
    double sine_wave = 0.5;

    // Bool
    bool _bool = false;

    // Vector.
    std::vector<bool> stl_vector{ false, false, false, false };
    // Array.
    std::array<bool, 7> stl_array{ false, false, false, false, false, false, false };

    // Auxiliary variables.
    std::vector<bool> stl_vector_one{ true };
    std::array<bool, 1> stl_array_one{ true };

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
    tracer.addTrace(_bool, "bool");
    tracer.addTrace(stl_vector, "stl_vector");
    tracer.addTrace(stl_array, "stl_array");

    // Create the header.
    tracer.createTrace();

    // Set the precision for the floating-point traces.
    // Set the precision for the floating-point traces.
    long_double_trace->setPrecision(9);
    double_trace->setPrecision(6);
    float_trace->setPrecision(3);

    // Initialize the trace.
    for (double time = 0; time < simulatedTime; time += timeStep) {
        _long_double *= M_PI;
        _double *= M_PI;
        _float *= M_PIf;

        _uint8_t  = static_cast<std::uint8_t>(_uint8_t + 8);
        _uint16_t = static_cast<std::uint16_t>(_uint16_t + 16);
        _uint32_t = static_cast<std::uint32_t>(_uint32_t + 32);
        _uint64_t = static_cast<std::uint64_t>(_uint64_t + 64);
        _int8_t   = static_cast<std::int8_t>(_int8_t - 8);
        _int16_t  = static_cast<std::int16_t>(_int16_t - 16);
        _int32_t  = static_cast<std::int32_t>(_int32_t - 32);
        _int64_t  = static_cast<std::int64_t>(_int64_t - 64);

        _bool = !_bool;

        stl_vector = stl_vector + stl_vector_one;
        stl_array  = stl_array + stl_array_one;

        sine_wave = offset + amplitude * std::sin(2 * M_PI * frequency * time);
        // Update the trace.
        tracer.updateTrace(time);
    }
    // Close the trace.
    tracer.closeTrace();
    return 0;
}
