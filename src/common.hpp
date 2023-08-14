#pragma once
#include <iostream>
#include <thread>
#define DEBUG 1
#define USE_MULTITHREADING 1

#if DEBUG
    #define prndd(x) std::cout << ">" << x <<  "   time : "<<time(0) << std::endl;
    #define prnerr(x, y) std::cout << x << y << '\n'
    #define loginf(x, y, z) std::cout << x << y << z <<'\n'

#else
    #define prndd(x)
    #define prnerr(x) perror(x)
    #define loginf(x, y, z)

#endif