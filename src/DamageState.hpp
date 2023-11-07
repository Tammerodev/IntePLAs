#pragma once
#include "PlayerState.hpp"
#include "common.hpp"

class DamageState : public PlayerState { 
public:
    virtual void enter() {
        timer = 0;
    }

    virtual void update(IntPhysicsComponent&, float) {  
        timer++;
    }
    
    virtual void draw(sf::RenderTarget& targ, sf::Sprite& sprite) {
        sf::Color renderColor;
        renderColor.a = 255;
        renderColor.r = 255 - timer;

        sprite.setColor(renderColor);
        targ.draw(sprite);

                
        if(timer > 100) {
            sprite.setColor(sf::Color::White);
            
            PlayerState::currentState = PlayerState::idleState;
            PlayerState::currentState->enter();
        }
    }

    virtual void input(bool) {

    }

private:
    int timer = 0;
    int damage_time = 100;
};