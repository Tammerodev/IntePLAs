#pragma once
#include <SFML/Graphics.hpp>
#include "PhysicsComponent.hpp"
#include "MobGlobals.hpp"
#include "MobIDGenerator.hpp"

enum FishStateType {
    No, FishDamage, FishSwim, FishDead, FishIdle
};

class FishState {
public:
    virtual void enter()                              = 0;
    virtual void update(PhysicsComponent&, float, bool)  = 0;
    virtual void draw(sf::RenderTarget&, sf::Sprite&) = 0;
    virtual FishStateType changeTo() {return No;};
};