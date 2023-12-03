#pragma once
#include <SFML/Graphics.hpp>
#include "PhysicsComponent.hpp"

class PlayerState {
public:
    virtual void enter()                              = 0;
    virtual void update(IntPhysicsComponent&, float)   = 0;
    virtual void draw(sf::RenderTarget&, sf::Sprite&) = 0;
    virtual void input(bool)                              = 0;
    static PlayerState* currentState;
	static PlayerState *walkState, *idleState, *jumpState, *creativeState, *dead, *damageState, *respawnState;
private:
};