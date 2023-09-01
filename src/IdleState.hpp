#pragma once
#include "PlayerState.hpp"
#include "Controls.hpp"

class IdleState : public PlayerState {
public:
    virtual void enter();
    virtual void update(PhysicsComponent&,float);
    virtual void draw(sf::RenderTarget&,sf::Sprite&);
    virtual void input(bool);
};