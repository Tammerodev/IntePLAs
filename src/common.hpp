#pragma once
#include <iostream>
#include <thread>
#define DEBUG 1

#if DEBUG
    #define prndd(x) std::cout << ">" << x <<  "   time : "<<time(0) << '\n'; std::this_thread::sleep_for(std::chrono::milliseconds(50))
    #define prnerr(x) perror(x)
#else
    #define prndd(x)
    #define prnerr(x) perror(x)
#endif