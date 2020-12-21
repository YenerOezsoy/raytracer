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

    std::cout << "sqrt2 (Newton method for sequence of 4 floats)" << std::endl;
    float values[4] = {4,9,16,250000};
    float *value_p = values;
    float roots[4];
    float *root_p = roots;
    time.start_clock();
    sqrt2<LOOPS>(value_p, root_p);
    time.stop_clock();
    std::cout << time.time_clock().count() << " [ns] result: " << root_p[0] << ", " << root_p[1] << ", " << root_p[2] << ", " << root_p[3] <<std::endl;
    time.reset_clock();

    std::cout << "sqrt3 (Newton method for sequence of 4 floats, SIMD)" << std::endl;
    float values_simd[4] = {4,9,16,250000};
    float *value_p_simd = values_simd;
    float roots_simd[4];
    float *root_p_simd = roots_simd;
    time.start_clock();
    sqrt3<LOOPS>(value_p_simd, root_p_simd);
    time.stop_clock();
    std::cout << time.time_clock().count() << " [ns] result: " << root_p[0] << ", " << root_p[1] << ", " << root_p[2] << ", " << root_p[3] <<std::endl;
    time.reset_clock();

}

int main(int argc, char *argv[]) {
    measure_sqrt_time<2>();
}
