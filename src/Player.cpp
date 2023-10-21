#include "Player.hpp"

PlayerState* PlayerState::walkState = new WalkState();
PlayerState* PlayerState::idleState = new IdleState();
PlayerState* PlayerState::jumpState = new JumpState();

PlayerState* PlayerState::currentState = PlayerState::idleState;

int Player::load() {
    physComp.gravity = 0.05f;
    if(!tx.loadFromFile("res/img/Player/Player.png")) return 0;
    spr.setTexture(tx);
    return 1;
}
void Player::update(float dt) {
    PlayerState::currentState->update(physComp, dt);
    PlayerState::currentState->input(grounded);

    if(!grounded) {
        physComp.update(dt);
    } else {
        physComp.transform_position.x += physComp.velocity.x * dt;
    }

    spr.setPosition(physComp.transform_position);
    
    grounded = false;
}

void Player::draw(sf::RenderTarget & window) {
    PlayerState::currentState->draw(window, spr);
}
