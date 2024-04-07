#pragma once
#include "FishState.hpp"

class FishSwimState : public FishState {
    public:
        void enter() {

        }

        void update(PhysicsComponent&, float, bool) {
            timer++;

            if(timer > 10) {
                phase++;
                timer = 0;
            }

            if(phase > 1) {
                phase = 0;
            }

            textureRect.height = 20;
            textureRect.width = 16;

            textureRect.top = 0;
            textureRect.left = 16 * phase;
        }

        void draw(sf::RenderTarget& target, sf::Sprite& sprite) {
            sprite.setTextureRect(textureRect);
            target.draw(sprite);
        }

    private: 
        sf::IntRect textureRect;
        int phase = 0;
        int timer = 0;
};