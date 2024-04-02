#pragma once
#include "WeatherParticle.hpp"

class SnowParticle : public WeatherParticle {
    public:
        SnowParticle(const sf::Vector2f &pos) : WeatherParticle(pos, sf::Vector2f(1.f, 1.5f), 50, sf::Color(211, 235, 230)) {
            
        }

        ParticleType getType() {
            return ParticleType::SnowParticle;
        }
};