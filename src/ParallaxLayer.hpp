#pragma once
#include <SFML/Graphics.hpp>

class ParallaxLayer {
    public:

    ParallaxLayer(const std::string& path, float amountOfParallax = 0.f) : parallax(amountOfParallax) {
		tx.loadFromFile(path);
        spr.setTexture(tx);
    }

    void update(sf::Vector2f pos) {
        
        const sf::Vector2f position = pos;
        const float temp = position.x * (1 - parallax);
        const float distance = position.x * parallax;

        spr.setPosition (0.f + distance, spr.getPosition().y);
    }

    void render(sf::RenderTarget& target) {
        spr.setTexture(tx);
        target.draw(spr);
    }

    private:



    float parallax = 0.f;    
    sf::Sprite spr;
    sf::Texture tx;
};