#include "Player.hpp"

PlayerState* PlayerState::walkState = new WalkState();
PlayerState* PlayerState::idleState = new IdleState();
PlayerState* PlayerState::jumpState = new JumpState();
PlayerState* PlayerState::creativeState = new CreativeState();
PlayerState* PlayerState::dead = new DeadState();
PlayerState* PlayerState::damageState = new DamageState();

PlayerState* PlayerState::currentState = PlayerState::idleState;

int Player::load() {
    physComp.gravity = 0.1;

    physComp.transform_position = sf::Vector2i(200, -100);


    if(!tx.loadFromFile("res/img/Player/Player.png")) return 0;
    spr.setTexture(tx);
    return 1;
}

void Player::update(float dt) {
    std::cout << (health) << '\n';
    PlayerState::currentState->update(physComp, dt);
    PlayerState::currentState->input(grounded);

    if(!grounded) {
        physComp.update();
    } else {
        physComp.transform_position.x += physComp.velocity.x * dt;
    }

    spr.setPosition(sf::Vector2f(physComp.transform_position));
    
    grounded = false;
}

void Player::draw(sf::RenderTarget & window) {
    PlayerState::currentState->draw(window, spr);
}
