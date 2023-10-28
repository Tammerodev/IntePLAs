#pragma once
#include <SFML/Graphics.hpp>

class ParallaxLayer {
    public:

    ParallaxLayer(const std::string& path, float amountOfParallax = 0.f) : parallax(amountOfParallax) {
		tx.loadFromFile(path);
        for(int i = 0; i < 6; i++) {
            sf::Sprite sprite = sf::Sprite();
            sprite.setTexture(tx);
            sprite.setPosition(i * tx.getSize().x, 0);
            spr.push_back(sprite);
        }
    }

    void update(const sf::Vector2f &pos) {

        int i = 0;
        for(auto &sprite : spr) {
            if((sprite.getPosition().x + (tx.getSize().x * 3)
                 - pos.x) < 0) {

                sprite.move(tx.getSize().x * spr.size(), 0);
                
            }
        }
    }

    void render(sf::RenderTarget& target) {
        for(auto &sprite : spr) {
            sprite.setTexture(tx);
            target.draw(sprite);
        }
    }

    private:



    float parallax = 0.f;    
    std::vector<sf::Sprite> spr;
    sf::Texture tx;
};