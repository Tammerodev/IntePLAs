#pragma once
#include <iostream>
#include <thread>
#define DEBUG 1

#if DEBUG
    #define prndd(x) std::cout << ">" << x <<  "   time : "<<time(0) << std::endl;
    #define prnerr(x, y) std::cout << x << y << '\n'
#else
    #define prndd(x)
    #define prnerr(x) perror(x)
#endif