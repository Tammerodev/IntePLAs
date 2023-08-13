#pragma once
#include <SFML/Graphics.hpp>

class ParallaxLayer {
    public:

    ParallaxLayer(const std::string& path, float amountOfParallax = 0.f) : parallax(amountOfParallax) {
		tx.loadFromFile(path);
        for(int i = 0; i < 5; i++) {
            sf::Sprite sprite = sf::Sprite();
            sprite.setTexture(tx);
            spr.push_back(std::pair(sprite, 0.f));
        }
    }

    void update(sf::Vector2f pos) {
        
        const sf::Vector2f position = pos;
        const float temp = position.x * (1 - parallax);
        const float distance = position.x * parallax;

        int i = 0;
        for(auto &sprite : spr) {
            float offset = (i - 0) * tx.getSize().x;
            if ((pos.x - sprite.second) + tx.getSize().x > tx.getSize().x) {
                sprite.second += tx.getSize().x;
            }     
            else if ((pos.x - sprite.second) + tx.getSize().x < -tx.getSize().x) {
                sprite.second -= tx.getSize().x;
            }     
            sprite.first.setPosition(((0.f + distance) - offset) + sprite.second, sprite.first.getPosition().y);

            i++;
        }
    }

    void render(sf::RenderTarget& target) {
        for(auto &sprite : spr) {
            sprite.first.setTexture(tx);
            target.draw(sprite.first);
        }
    }

    private:



    float parallax = 0.f;    
    std::vector<std::pair<sf::Sprite, float>> spr;
    sf::Texture tx;
};