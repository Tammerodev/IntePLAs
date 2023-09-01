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

    const float speed = 2.0f;
    const float acc = 0.55;

    sf::Vector2i voxelpos;
    sf::Clock timer;

    sf::Vector2f last_pos;
};