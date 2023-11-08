#pragma once
#include <SFML/Graphics.hpp>

class Particle : public sf::Vector2f {
    public:
        Particle() = default;
        
        virtual void update(const float) = 0;
        virtual void render(sf::RenderTarget&) = 0;

        virtual bool remove() = 0;

        virtual void collide() = 0;

        int energy = 100;
    private:
};