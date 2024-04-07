#pragma once
#include "FrogState.hpp"

class FrogJumpState : public FrogState {
    public:
        void enter() {
            phase = 0;
            timer = 0;
        }

        void update(PhysicsComponent& physComp, float, bool grounded) {

            if(phase == 4) {
                physComp.velocity.y = -3.0;
            }

            textureRect.height = 20;
            textureRect.width = 20;
            textureRect.top = 20 * 2;
            textureRect.left = phase * 20;

            timer++;
            if(timer > 10) {
                timer = 0;

                phase++;
            }

            if(phase >= 7) {
                phase = 7;
            }

            

            if(grounded && phase == 7) {
                change = true;
            }
        }

        FrogStateType::Type changeTo() {
            if(change) {
                return FrogStateType::FrogIdle;
                change = false;
                timer = 0;
                phase = 0;
            };

            return FrogStateType::No;
        };

        void draw(sf::RenderTarget& target, sf::Sprite& sprite) {
            sprite.setTextureRect(textureRect);
            target.draw(sprite);
        }

    private: 
        bool change = false;

        sf::IntRect textureRect;
        int phase = 0;
        int timer = 0;
};