#include "Player.hpp"

PlayerState* PlayerState::walkState = new WalkState();
PlayerState* PlayerState::idleState = new IdleState();
PlayerState* PlayerState::jumpState = new JumpState();
PlayerState* PlayerState::creativeState = new CreativeState();
PlayerState* PlayerState::dead = new DeadState();
PlayerState* PlayerState::damageState = new DamageState();
PlayerState* PlayerState::respawnState = new RespawnState();
PlayerState* PlayerState::swimState = new SwimState();

PlayerState* PlayerState::currentState = PlayerState::idleState;

int Player::load() {
    physComp.transform_position = PlayerGlobal::spawn_point;

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

    spr.setPosition(sf::Vector2f(physComp.transform_position));

    if(PlayerGlobal::still_radioation > 0) {
        PlayerGlobal::still_radioation--;
    }
    
    
    grounded = false;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
        PlayerState::currentState = PlayerState::creativeState;
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
        PlayerState::currentState = PlayerState::idleState;
    }

    debug_globals::player_position = physComp.transform_position;
}

void Player::draw(sf::RenderTarget & window) {
    PlayerState::currentState->draw(window, spr);
}
