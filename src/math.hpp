#pragma once
#include <random>
#include <SFML/Graphics/Vertex.hpp>

namespace math {
    static std::random_device rd;
    constexpr double PI = 3.1415926;

    inline float randFloat() {
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.f,1.f);
        return dis(gen);
    }
    
    inline int randIntInRange(int min, int max) {
        return min + ( std::rand() % ( max - min + 1 ) );
    }

    inline const sf::Vector2f normalize(const sf::Vector2f& source) {
        float length = sqrt((source.x * source.x) + (source.y * source.y));
        if (length != 0)
            return sf::Vector2f(source.x / length, source.y / length);
        else
            return source;
    }

    inline bool randProp(int propability) {
        return randIntInRange(0, 100) < propability;
    }
    
    inline const sf::Vector2f subVector(const sf::Vector2f& a, const sf::Vector2f& b) {
        return sf::Vector2f (
            a.x - b.x,  
            a.y - b.y
        );
    }

    static double degreesToRadians(double degree)
    {
        return (degree * (PI / 180));
    }

    static float distance(const sf::Vector2f& vectorA, const sf::Vector2f& vectorB) {
        float deltaX = vectorB.x - vectorA.x;
        float deltaY = vectorB.y - vectorA.y;
        return std::sqrt(deltaX * deltaX + deltaY * deltaY);
    }

   /* static float fastDistance() {
        float deltaX = vectorB.x - vectorA.x;
        float deltaY = vectorB.y - vectorA.y;
        return 
    }*/

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