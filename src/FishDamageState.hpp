#pragma once
#include "FishState.hpp"

class FishDamageState : public FishState {
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

        FishStateType changeTo() {
            if(change) return FishStateType::FishIdle;
            else return No;
        }


    private: 
        int timer = 0;
        int damage_time = 500;

        bool change = false;
};