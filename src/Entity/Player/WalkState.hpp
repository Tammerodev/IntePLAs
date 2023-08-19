#pragma once
#include "PlayerState.hpp"
#include "Controls.hpp"

class WalkState : public PlayerState {
public:
    virtual void enter();
    virtual void update(sf::Vector2f&,float&,float);
    virtual void draw(sf::RenderTarget&,sf::Sprite&);
    virtual void input(bool);
private:
    float speed = 0.25f;
    sf::Vector2i voxelpos;
    sf::Clock timer;

    sf::Vector2f last_pos;
};