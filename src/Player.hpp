#pragma once

#include "Controls.hpp"
#include <SFML/Graphics.hpp>
#include "PlayerState.hpp"
#include "IdleState.hpp"
#include "WalkState.hpp"
#include "JumpState.hpp"
#include "PhysicsComponent.hpp"
#include <iostream>
#include "Collider.hpp"

class Player {
public:

int load();
void update(float);
void draw(sf::RenderTarget&);

PhysicsComponent &getPhysicsComponent() {
    return physComp;
}

void ground() {
    physComp.velocity.y = 0.f;
    grounded = true;
}

private:

sf::Vector2i position;

bool grounded = false;

sf::Sprite spr;
sf::Texture tx;

sf::IntRect texture_rect;

sf::Sprite sprite;


PhysicsComponent physComp;
};