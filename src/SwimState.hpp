#pragma once
#include "PlayerState.hpp"
#include "Controls.hpp"
#include "PlayerGlobal.hpp"

class SwimState : public PlayerState {
public:
    virtual void enter();
    virtual void update(IntPhysicsComponent&,float);
    virtual void draw(sf::RenderTarget&,sf::Sprite&);
    virtual void input(bool);

protected:
    const float speed = 1.f;
    const float water_slowdown_x = 2.5f;
    const float water_slowdown_y = 15.f;

    sf::Clock timer;
    sf::Clock swimCoolDown;

    sf::Vector2i last_pos;
};