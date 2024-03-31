#pragma once
#include "UIElement.hpp"
#include "Player.hpp"
#include <SFML/Graphics.hpp>

class BarUI : public UIElement {
    public:
        bool load(const int sizeX, const int sizeY, const std::string& texturePath = "res/img/Player/heart.png") {
            const sf::Vector2f pos = sf::Vector2f(sizeX / 3.5, sizeY - (index * 16));

            heartTexture.loadFromFile(texturePath);
            for(int i = 0; i < max_value; i++) {
                sf::Sprite newSprite;
                newSprite.setTexture(heartTexture);
                newSprite.setScale(2.f, 2.f);
                newSprite.setPosition(pos.x + (i * (16 * newSprite.getScale().x)), pos.y);

                heartSprites.push_back(newSprite);
            }

            return true;
        }

        void update() {

            int healthRemaining = value;

            for(auto &heart : heartSprites) {
                heart.setTexture(heartTexture);

                if (healthRemaining >= valueForHeart) { // Full heart
                    heart.setTextureRect(sf::IntRect(0, 0, 16, 16));
                } else if(healthRemaining > 0 && healthRemaining < valueForHeart) { // Half heart
                    heart.setTextureRect(sf::IntRect(16, 0, 16, 16));
                } else {
                    heart.setTextureRect(sf::IntRect(32, 0, 16, 16));
                }

                healthRemaining -= 1;
            }
        }

        void render(sf::RenderTarget &target) {
            for(const auto &heart : heartSprites) {
                target.draw(heart);
            }
        }

        void setValue(const int val) {
            value = val;
        }

        void setMaxValue(const int max_val) {
            max_value = max_val;
        }

        void setColorFill(const sf::Color& color) {

        } 

        void setColorEmpty(const sf::Color& color) {

        }

        void setIndex(const int ind) {
            index = ind;
        }
    public:
        std::vector<sf::Sprite> heartSprites;

    private:
        int value = 0;
        int max_value = 0;

        int valueForHeart = 2;

        int index = 0;
        sf::Texture heartTexture;
};