#pragma once
#include <random>

namespace math {
    inline float randFloat() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.f,1.f);
        return dis(gen);
    }
    inline void floodFill(int x, int y, int targetColor, int replacementColor)
        {
            // TODO implement
        }
    inline unsigned short isqrt(unsigned long a) {
        unsigned long rem = 0;
        int root = 0;
        int i;

        for (i = 0; i < 16; i++) {
            root <<= 1;
            rem <<= 2;
            rem += a >> 30;
            a <<= 2;

            if (root < rem) {
                root++;
                rem -= root;
                root++;
            }
        }

        return (unsigned short) (root >> 1);
    }
    constexpr double PI = 3.1459;
}