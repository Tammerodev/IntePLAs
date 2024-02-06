#pragma once
#include <SFML/Graphics.hpp>
#include "PhysicsComponent.hpp"

class FishState {
public:
    virtual void enter()                              = 0;
    virtual void update(PhysicsComponent&, float, bool)  = 0;
    virtual void draw(sf::RenderTarget&, sf::Sprite&) = 0;
    
    static FishState* currentState;
	static FishState *idleState, *swimState, *dead, *damageState;
private:
};