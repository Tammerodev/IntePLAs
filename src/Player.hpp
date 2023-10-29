#pragma once

#include "Controls.hpp"
#include <SFML/Graphics.hpp>
#include "PlayerState.hpp"
#include "IdleState.hpp"
#include "WalkState.hpp"
#include "JumpState.hpp"
#include "CreativeState.hpp"
#include "PhysicsComponent.hpp"
#include <iostream>
#include "Collider.hpp"

class Player {
public:

int load();
void update(float);
void draw(sf::RenderTarget&);

IntPhysicsComponent &getPhysicsComponent() {
    return physComp;
}

void ground() {
    physComp.velocity.y = 0;
    physComp.velocity_buffer = 0;
    
    grounded = true;
}

private:

sf::Vector2i position;

bool grounded = false;

sf::Sprite spr;
sf::Texture tx;

sf::IntRect texture_rect;

sf::Sprite sprite;


IntPhysicsComponent physComp;
};