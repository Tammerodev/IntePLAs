#pragma once
#include "PlayerState.hpp"
#include "Controls.hpp"

class WalkState : public PlayerState {
public:
    virtual void enter();
    virtual void update(PhysicsComponent&,float);
    virtual void draw(sf::RenderTarget&,sf::Sprite&);
    virtual void input(bool);
private:

    const float speed = 1.5f;
    const float acc = 0.15;

    sf::Vector2i voxelpos;
    sf::Clock timer;

    sf::Vector2f last_pos;
};