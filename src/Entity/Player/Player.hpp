#pragma once

#include <SFML/Graphics.hpp>
#include "PlayerState.hpp"
#include "IdleState.hpp"
#include "WalkState.hpp"
#include "JumpState.hpp"
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

void move_y(float val){
    pos.y += val;
}

void move_x(float val){
    pos.x += val;
}

void ground() {
    acc.y = 0.f;
}


void update_hitboxtop() {
    hitbox_top.height = 12;
    hitbox_top.width = 18;
    hitbox_top.top = pos.y;
    hitbox_top.left = pos.x;
}

void update_hitboxbottom() {
    hitbox_bottom.height = 12;
    hitbox_bottom.width = 18;
    hitbox_bottom.top = pos.y + 12;
    hitbox_bottom.left = pos.x;
}

void update_hitboxleft() {
    hitbox_left.height = 2;
    hitbox_left.width = 4;
    hitbox_left.top = pos.y + 16;
    hitbox_left.left = pos.x - 2;
}

void update_hitboxright() {
    hitbox_right.height = 2;
    hitbox_right.width = 4;
    hitbox_right.top = pos.y + 16;
    hitbox_right.left = pos.x + 20;
}

sf::Vector2f get_voxel_pos() {
    return pos;
}

void setGrounded(bool newVal) {
    grounded = newVal;
}

private:

bool grounded = false;

sf::Sprite spr;
sf::Texture tx;

sf::IntRect texture_rect;

sf::Vector2f pos = sf::Vector2f(500,0);
sf::Vector2i prev_voxel_aligned_pos;

sf::Sprite sprite;
sf::FloatRect hitbox_top;
sf::FloatRect hitbox_bottom;
sf::FloatRect hitbox_right;
sf::FloatRect hitbox_left;

float gravity = 0.07f;
sf::Vector2f acc = sf::Vector2f(0.f,0.f);
};