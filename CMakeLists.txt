cmake_minimum_required(VERSION 3.16)
project(su20_proj4_pixelled C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(include)

add_executable(su20_proj4_pixelled
        mat_test.c
        matrix.c
        matrix.h
        numc.c
        numc.h)
