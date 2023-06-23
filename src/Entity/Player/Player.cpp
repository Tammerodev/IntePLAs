#include "Player.hpp"

PlayerState* PlayerState::walkState = new WalkState();
PlayerState* PlayerState::idleState = new IdleState();
PlayerState* PlayerState::jumpState = new JumpState();

PlayerState* PlayerState::currentState = PlayerState::idleState;

int Player::load() {
    if(!tx.loadFromFile("/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/res/img/Player/Player.png")) return 0;
    spr.setTexture(tx);
    return 1;
}
void Player::update(float dt) {
    PlayerState::currentState->update(pos,y_acceleration,dt);
    PlayerState::currentState->input(grounded);
    y_acceleration += gravity * dt;

    spr.setPosition(pos.x, pos.y);
    pos.y += y_acceleration ;

    update_hitboxtop();
    update_hitboxbottom();
    update_hitboxleft();
    update_hitboxright();
}

void Player::draw(sf::RenderTarget & window) {
    PlayerState::currentState->draw(window, spr);
}
