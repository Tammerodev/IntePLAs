#pragma once
#include <random>

namespace math {
    inline float randFloat() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.f,1.f);
        return dis(gen);
    }
    constexpr double PI = 3.1459;
}