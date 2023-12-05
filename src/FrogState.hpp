#pragma once
#include <SFML/Graphics.hpp>
#include "PhysicsComponent.hpp"

class FrogState {
public:
    virtual void enter()                              = 0;
    virtual void update(PhysicsComponent&, float, bool)  = 0;
    virtual void draw(sf::RenderTarget&, sf::Sprite&) = 0;
    
    static FrogState* currentState;
	static FrogState *idleState, *jumpState, *dead, *damageState;
private:
};