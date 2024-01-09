#pragma once
#include "FrogState.hpp"

class FrogDamageState : public FrogState {
    public:
        void enter() {
            
        }

        void update(PhysicsComponent&, float, bool) {
            
        }

        void draw(sf::RenderTarget& target, sf::Sprite& sprite) {
            timer++;

            if(timer > damage_time) {
                FrogState::currentState = FrogState::idleState;
                FrogState::currentState->enter();
                timer = 0;

                sprite.setColor(sf::Color::White);
            }

            sprite.setColor(sf::Color::Red);
        }

    private: 
        int timer = 0;
        int damage_time = 500;
};