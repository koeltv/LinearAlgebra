cmake_minimum_required(VERSION 3.17)
project(LinearAlgebra C)

set(CMAKE_C_STANDARD 99)

add_executable(LinearAlgebra main.c main.h matrix.c matrix.h polynomial.c polynomial.h stringInteractions.c stringInteractions.h register.c register.h variable.c variable.h)