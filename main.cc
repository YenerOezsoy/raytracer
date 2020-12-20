//
// Created by Yener Özsoy on 16.12.20.
//
#include <iostream>
#include <math.h>
#include "sqrt_opt.h"
#include "measure_time.h"

template <size_t LOOPS = 2>
void measure_sqrt_time(void) {
    float value = 9;
    MeasureTime<std::chrono::nanoseconds> time;
    std::cout << "math sqrt" << std::endl;
    time.start_clock();
    float root = sqrt(value);

    time.stop_clock();
    std::cout << time.time_clock().count()<< " [ns] result: " << root << std::endl;
    time.reset_clock();

    std::cout << "sqrt1 (Newton method for single float, one time a loop)" << std::endl;
    time.start_clock();
    root = sqrt1<LOOPS>(&value);
    time.stop_clock();
    std::cout << time.time_clock().count() << " [ns] result: " << root << std::endl;
    time.reset_clock();
}

int main(int argc, char *argv[]) {
    measure_sqrt_time<2>();
}
