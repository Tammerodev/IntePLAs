#include "RespawnState.hpp"

void RespawnState::enter() {
    firstTime = true;

    PlayerGlobal::temparature = 0;
    PlayerGlobal::bodyTemparature = 0;
}

void RespawnState::update(IntPhysicsComponent& physComp, float dt) {
    if(firstTime) {
        physComp.transform_position = sf::Vector2f(1000, -300);
    }

    physComp.velocity.y /= 1.2;

    firstTime = false;
    timer++;
}

void RespawnState::draw(sf::RenderTarget &window, sf::Sprite &sprite) {
    sprite.setTextureRect(sf::IntRect(0,0,18,28));
    sprite.setColor(sf::Color::Blue);
    window.draw(sprite);
}

void RespawnState::input(bool grounded) {
    if(grounded && timer > 100) {
        PlayerState::currentState = PlayerState::idleState;
        PlayerState::currentState->enter();
    }
}
