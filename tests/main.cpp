#include "variableTracer.hpp"
#include "sinSource.hpp"

int main(int, char **)
{
    // Define simulated time and timestep of the simulation.
    TimeScale simulatedTime(50, TimeScale::SEC);
    TimeScale timeStep(1, TimeScale::SEC);
    // Create the variable that has to be traced.
    double doubleValue = 0;
    unsigned int unsignedValue = 0;
    short shortValue = 0;
    SinSource sinSource(0, 1, 0.5);
    // Auxiliary variables.
    double first = 0;
    double second = 1;
    // Create the trace and add the variable to the trace.
    VariableTracer trace("trace.vcd", timeStep);
    trace.addTrace(doubleValue, "DoubleValue");
    trace.addTrace(unsignedValue, "UnsignedValue");
    trace.addTrace(shortValue, "ShortValue");
    trace.addTrace(sinSource.out, "Sinusoid");
    trace.createTrace();
    // Initialize the trace.
    for (double time = 0; time < simulatedTime; time += timeStep)
    {
        if (time <= 1)
        {
            doubleValue = time;
        }
        else
        {
            doubleValue = first + second;
            first = second;
            second = doubleValue;
        }
        unsignedValue++;
        shortValue--;
        sinSource.compute(time);
        // Update the trace.
        trace.updateTrace(time);
    }
    // Close the trace.
    trace.closeTrace();
}
