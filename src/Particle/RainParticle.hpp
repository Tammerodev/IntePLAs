#pragma once
#include "WeatherParticle.hpp"

class RainParticle : public WeatherParticle {
    public:
        RainParticle(const sf::Vector2f &pos) : WeatherParticle(pos, sf::Vector2f(1.5f, 2.5f), 255, sf::Color(30,129,176, 200)) {
            
        }

        ParticleType getType() {
            return ParticleType::RainParticle;
        }
};