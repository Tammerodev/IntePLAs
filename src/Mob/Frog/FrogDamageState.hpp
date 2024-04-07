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
                timer = 0;
                change = true;
                sprite.setColor(sf::Color::White);
            }

            sprite.setColor(sf::Color::Red);
        }

        FrogStateType::Type changeTo() {
            if(change) return FrogStateType::FrogIdle;
            else return FrogStateType::No;
        }



    private: 
        bool change = false;

        int timer = 0;
        int damage_time = 500;
};