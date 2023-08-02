#pragma once
#include <random>

namespace math {
    static std::random_device rd;
    inline float randFloat() {
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.f,1.f);
        return dis(gen);
    }
    
    inline int randIntInRange(int min, int max) {
        return min + ( std::rand() % ( max - min + 1 ) );
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

    static double degreesToRadians(double degree)
    {
        return (degree * (PI / 180));
    }

    static float distance(const sf::Vector2f& vectorA, const sf::Vector2f& vectorB) {
        float deltaX = vectorB.x - vectorA.x;
        float deltaY = vectorB.y - vectorA.y;
        return std::sqrt(deltaX * deltaX + deltaY * deltaY);
    }

    static void limitDistance(const sf::Vector2f& vectorA, sf::Vector2f& vectorB, const float limit) {
        sf::Vector2f direction = vectorB - vectorA;
        float dist = distance(vectorA, vectorB);
    
        if (dist > limit) {
            // Scale the direction vector to a length of 100 units
            direction /= dist;
            direction *= limit;

            vectorB = vectorA + direction;
        }
    }
}