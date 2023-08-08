#pragma once

#include "Entity/Player/Controls.hpp"
#include <SFML/Graphics.hpp>
#include "Entity/Player/PlayerState.hpp"
#include "Entity/Player/IdleState.hpp"
#include "Entity/Player/WalkState.hpp"
#include "Entity/Player/JumpState.hpp"
#include "PhysicsComponent.hpp"
#include <iostream>

class Player {
public:

int load();
void update(float);
void draw(sf::RenderTarget&);

const sf::FloatRect &getTopHitbox() {
    return hitbox_top;
}
const sf::FloatRect &getBottomHitbox() {
    return hitbox_bottom;
}
const sf::FloatRect &getLeftHitbox() {
    return hitbox_left;
}
const sf::FloatRect &getRightHitbox() {
    return hitbox_right;
}

PhysicsComponent &getPhysicsComponent() {
    return physComp;
}

void ground() {
    physComp.velocity.y = 0.f;
    grounded = true;
}


void update_hitboxtop() {
    hitbox_top.height = 12;
    hitbox_top.width = 12;
    hitbox_top.top = physComp.transform_position.y - 4;
    hitbox_top.left = physComp.transform_position.x + 3;
}

void update_hitboxbottom() {
    hitbox_bottom.height = 12;
    hitbox_bottom.width = 8;
    hitbox_bottom.top = physComp.transform_position.y + 12;
    hitbox_bottom.left = physComp.transform_position.x + 4;
}

void update_hitboxleft() {
    hitbox_left.height = 2;
    hitbox_left.width = 10;
    hitbox_left.top = physComp.transform_position.y + 8;
    hitbox_left.left = physComp.transform_position.x - 2;
}

void update_hitboxright() {
    hitbox_right.height = 8;
    hitbox_right.width = 10;
    hitbox_right.top = physComp.transform_position.y + 8;
    hitbox_right.left = physComp.transform_position.x + 20;
}


private:

bool grounded = false;

sf::Sprite spr;
sf::Texture tx;

sf::IntRect texture_rect;

sf::Sprite sprite;

sf::FloatRect hitbox_top;
sf::FloatRect hitbox_bottom;
sf::FloatRect hitbox_right;
sf::FloatRect hitbox_left;


PhysicsComponent physComp;
};