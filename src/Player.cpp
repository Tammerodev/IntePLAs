#include "Player.hpp"

PlayerState* PlayerState::walkState = new WalkState();
PlayerState* PlayerState::idleState = new IdleState();
PlayerState* PlayerState::jumpState = new JumpState();

PlayerState* PlayerState::currentState = PlayerState::idleState;

int Player::load() {
    physComp.gravity = 0.07f;
    if(!tx.loadFromFile("res/img/Player/Player.png")) return 0;
    spr.setTexture(tx);
    return 1;
}
void Player::update(float dt) {
    PlayerState::currentState->update(physComp.transform_position,physComp.velocity.y,dt);
    PlayerState::currentState->input(grounded);
    if(!grounded) {
        physComp.update();
    }
    spr.setPosition(physComp.transform_position);

    update_hitboxtop();
    update_hitboxbottom();
    update_hitboxleft();
    update_hitboxright();

    grounded = false;
}

void Player::draw(sf::RenderTarget & window) {
    PlayerState::currentState->draw(window, spr);

    hitbox_top.debugdraw(window);
    hitbox_bottom.debugdraw(window);
    hitbox_left.debugdraw(window);
    hitbox_right.debugdraw(window);

}
