#pragma once
#include "PlayerState.hpp"

class IdleState : public PlayerState {
public:
    virtual void enter();
    virtual void update(sf::Vector2f&,float&,float);
    virtual void draw(sf::RenderTarget&,sf::Sprite&);
    virtual void input(bool);
};