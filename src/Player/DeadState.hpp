#pragma once
#include "PlayerState.hpp"
#include "../common.hpp"

class DeadState : public PlayerState { 
public:
    virtual void enter() {
        
    }

    virtual void update(IntPhysicsComponent&, float) {
        
        
    }
    
    virtual void draw(sf::RenderTarget& targ, sf::Sprite& sprite) {
        sprite.setColor(sf::Color::Red);
        targ.draw(sprite);
    }

    virtual void input(bool) {

    }
};