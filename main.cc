//
// Created by Yener Özsoy on 16.12.20.
//
#include <iostream>
#include <math.h>
#include "sqrt_opt.h"
#include "measure_time.h"
#include <random>

double random_double()
{
    static std::random_device device;
    static std::uniform_real_distribution<double> dist(0.0, 10000.0);
    return dist(device);
}

template <size_t LOOPS = 3>
void measure_sqrt_time(void) {
    //math.h library
    float value = 9;
    std::cout << "sqrt (Math.h library)" << std::endl;
    std::cout << "value: " << value <<std::endl;
    float root = sqrt(value);
    std::cout << "result: " << root << std::endl;

    //sqrt1
    std::cout << "sqrt1 (Newton method for single float, one time a loop)" << std::endl;
    std::cout << "value: " << value <<std::endl;
    root = sqrt1<LOOPS>(&value);
    std::cout << "result: " << root << std::endl;

    //sqrt2
    std::cout << "sqrt2 (Newton method for sequence of 4 floats)" << std::endl;
    float values[4] = {4,9,16,250000};
    float *value_p = values;
    float roots[4];
    float *root_p = roots;

    std::cout << "values: " << values[0] << ", " << values[1] << ", " << values[2] << ", " << values[3] <<std::endl;
    sqrt2<LOOPS>(value_p, root_p);
    std::cout << "result: " << root_p[0] << ", " << root_p[1] << ", " << root_p[2] << ", " << root_p[3] <<std::endl;

    //sqrt3
    std::cout << "sqrt3 (Newton method for sequence of 4 floats, SIMD)" << std::endl;
    float roots_simd[4];
    float *root_p_simd = roots_simd;

    std::cout << "values: " << values[0] << ", " << values[1] << ", " << values[2] << ", " << values[3] <<std::endl;
    sqrt3<LOOPS>(value_p, root_p_simd);
    std::cout << "result: " << root_p[0] << ", " << root_p[1] << ", " << root_p[2] << ", " << root_p[3] <<std::endl;

    for (int i = 0; i < 10; i++) {
        float random_value = random_double();
        float value[4] = {random_value, random_value, random_value, random_value};
        float *value_p = value;
        float result = sqrt(random_value);

        float result_sqrt1 = sqrt1(&random_value);

        float result_sqrt2[4];
        float *result_p2 = result_sqrt2;
        sqrt2(value_p, result_p2);

        float result_sqrt3[4];
        float *result_p3 = result_sqrt3;
        sqrt3(value_p, result_p3);

        std::cout << "Error found" << " value:" << random_value << " result:" <<result << ", " << result_sqrt1 << ", " << result_sqrt2[0] << ", " << result_sqrt3[0] << std::endl;
    }

}



int main(int argc, char *argv[]) {
    measure_sqrt_time<2>();
}
