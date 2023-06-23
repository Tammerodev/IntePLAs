#pragma once
#include "PlayerState.hpp"

class JumpState : public PlayerState {
public:
    virtual void enter();
    virtual void update(sf::Vector2f&,float&,float);
    virtual void draw(sf::RenderTarget&,sf::Sprite&);
    virtual void input(bool);
private:
};