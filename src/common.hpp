#pragma once
#include <iostream>
#include <thread>
#include <cstdio>

#define DEBUG 1
#define USE_MULTITHREADING 1
#define PREALLOCATE_EFFECTS_COUNT 10

#if DEBUG
    #define prndd(x) std::cout << "[OUTPUT]:" << x <<  "   [TIME]: "<<time(0) << std::endl;
    #define prnerr(x, y) std::cout << x << y << '\n'
    #define loginf(x, y, z) std::cout << x << y << z <<'\n'

#else
    #define prndd(x)
    #define prnerr(x) perror(x)
    #define loginf(x, y, z)

#endif