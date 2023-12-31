#pragma once
#include "PlayerState.hpp"
#include "Controls.hpp"

class RespawnState : public PlayerState {
public:
    virtual void enter();
    virtual void update(IntPhysicsComponent&,float);
    virtual void draw(sf::RenderTarget&,sf::Sprite&);
    virtual void input(bool);

private:
    bool firstTime = true;
};