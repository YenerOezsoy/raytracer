cmake_minimum_required(VERSION 3.17)
project(raytracer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(raytracer
        kdtree.cc
        kdtree.h
        math.h
        math.tcc
        measure_time.h
        sqrt_opt.h
        statistics.cc
        statistics.h
        triangle.h
        triangle.tcc
        main.cc)
