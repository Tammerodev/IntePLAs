#pragma once
#include <iostream>
#include <thread>
#include <cstdio>

#define DEBUG 1
#define USE_MULTITHREADING 1
#define PREALLOCATE_EFFECTS_COUNT 10

#define CWINDOWS true
#define CLINUX   false

#if DEBUG
    #define prndd(x) std::cout << "[OUTPUT]:" << x <<  "   [TIME]: "<<time(0) << std::endl;
    #define prnmod(x) std::cout << "[MOD]:" << x <<  "   [TIME]: "<<time(0) << std::endl;
    #define logmod(x, y, z) std::cout << "[MOD]" << x << y << z <<'\n'


    #define prnerr(x, y) std::cout << x << y << '\n'
    #define loginf(x, y, z) std::cout << x << y << z <<'\n'

#else
    #define prndd(x)
    #define prnerr(x) perror(x)
    #define loginf(x, y, z)

#endif