#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{

private:

    std::chrono::high_resolution_clock::time_point dt = std::chrono::high_resolution_clock::now();

public:

    double delta = 0.0;

    double Tick();

};

#endif