#pragma once
#include <SFML/Graphics.hpp>



class Particle : public sf::Vector2f {
    public:
        enum ParticleType {
            Neutron, Debris, PickableDebris
        };

        Particle() = default;
        
        virtual void update(const float, const sf::Vector2f &pos) = 0;
        virtual void render(sf::RenderTarget&) = 0;

        virtual bool remove() = 0;

        virtual void collide() = 0;

        virtual ParticleType getType() = 0;

        virtual sf::Color getColor() {
            return sf::Color::Blue;
        }

        int energy = 100;
    private:
};