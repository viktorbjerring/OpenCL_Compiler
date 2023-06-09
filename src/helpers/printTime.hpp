#include<CL/cl.hpp>

#ifndef D1DD5B8A_E962_4A8A_B12B_703E0090167F
#define D1DD5B8A_E962_4A8A_B12B_703E0090167F

inline void printTime(cl::Event& evt) {
    auto start = evt.getProfilingInfo<CL_PROFILING_COMMAND_START>();
    auto end = evt.getProfilingInfo<CL_PROFILING_COMMAND_END>();
    auto nanosTotal = end - start;
    auto secondsTotal = nanosTotal / 1000000000;
    auto minutes = secondsTotal / 60;
    auto seconds = secondsTotal % 60;
    auto millis = (nanosTotal % 1000000000) / 1000000;
    auto micros = (nanosTotal % 1000000) / 1000;
    auto nanos = nanosTotal % 1000;
    std::cout << "Mandelbrot ran in: " << minutes << " minutes, " << seconds << " seconds, " << millis << " milliseconds, " << micros << " microseconds, " << nanos << " nanoseconds\n";
    std::cout << "Total nanoseconds:\n" << nanosTotal << "\n";
}

#endif /* D1DD5B8A_E962_4A8A_B12B_703E0090167F */
