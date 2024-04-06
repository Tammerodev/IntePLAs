#pragma once
#include <SFML/Graphics.hpp>

class Entity {
    public:
        virtual void load() = 0;
        virtual void update(const float dt) = 0;
        virtual void render(sf::RenderTarget&) = 0; 
        
    protected:
        sf::Sprite sprite;
        sf::Texture texture;
};