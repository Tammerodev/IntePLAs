#pragma once
#include "FrogState.hpp"

class FrogIdleState : public FrogState {
    public:
        void enter() {

        }

        void update(PhysicsComponent& comp, float, bool) {

            comp.transform_position += comp.velocity;

            timer++;

            if(timer > 10) {
                phase++;
                timer = 0;
            }

            if(phase > 1) {
                phase = 0;
            }

            textureRect.height = 20;
            textureRect.width = 20;

            textureRect.top = 0;
            textureRect.left = 20 * phase;
        }

        void draw(sf::RenderTarget& target, sf::Sprite& sprite) {
            sprite.setTextureRect(textureRect);
            target.draw(sprite);
        }

    private: 
        sf::IntRect textureRect;
        int phase = 0;
        int timer = 0;


    protected:
        const float speed = 1.f;
        const float water_slowdown_x = 2.5f;
        const float water_slowdown_y = 15.f;

        sf::Clock timer;
        sf::Clock swimCoolDown;
};