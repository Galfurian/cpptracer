#include "CPPTracer.hpp"

int main(int argc, char **argv)
{
    // Create the trace.
    CPPTracer<long unsigned int> trace("trace.vcd", TimeScale(1, TimeScale::SEC));
    // Create the variable that has to be traced.
    long unsigned int first = 0;
    long unsigned int second = 1;
    long unsigned int value;
    // Add the variable to the trace.
    trace.addTrace("Value", &value);
    // Initialize the trace.
    trace.createTrace();
    for (long unsigned int time = 0; time <= 50; ++time)
    {
        if (time <= 1)
        {
            value = time;
        }
        else
        {
            value = first + second;
            first = second;
            second = value;
        }
        trace.updateTrace(time);
    }
    trace.closeTrace();
}
