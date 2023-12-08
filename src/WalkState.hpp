#pragma once
#include "PlayerState.hpp"
#include "Controls.hpp"
#include "PlayerGlobal.hpp"

class WalkState : public PlayerState {
public:
    virtual void enter();
    virtual void update(IntPhysicsComponent&,float);
    virtual void draw(sf::RenderTarget&,sf::Sprite&);
    virtual void input(bool);
private:

    const int speed = 2;
    const float acc = 0.15;

    sf::Vector2i voxelpos;
    sf::Clock timer;

    sf::Vector2i last_pos;
};