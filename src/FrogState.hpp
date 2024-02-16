#pragma once
#include <SFML/Graphics.hpp>
#include "PhysicsComponent.hpp"

namespace FrogStateType {
    enum Type {
        No, FrogDamage, FrogJump, FrogDead, FrogIdle
    };
}

class FrogState {
public:
    virtual void enter()                              = 0;
    virtual void update(PhysicsComponent&, float, bool)  = 0;
    virtual void draw(sf::RenderTarget&, sf::Sprite&) = 0;
    virtual FrogStateType::Type changeTo() {return FrogStateType::No;};

private:
};