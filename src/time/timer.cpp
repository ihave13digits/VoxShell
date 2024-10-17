#include "timer.h"

double Timer::Tick()
{
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(now - dt);
    dt = now;
    delta = elapsed_time.count();
    return delta;
}
