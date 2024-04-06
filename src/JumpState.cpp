#include "JumpState.hpp"

void JumpState::enter() {
}

void JumpState::update(IntPhysicsComponent& physComp, float dt) {
    physComp.velocity.y = -4.0;
    physComp.transform_position.y -= 1;

    PlayerState::currentState = idleState;
}

void JumpState::draw(sf::RenderTarget &window, sf::Sprite &sprite){
    sprite.setTextureRect(sf::IntRect(0, 0, PlayerGlobal::characterSize.x, PlayerGlobal::characterSize.y));
    window.draw(sprite);
}

void JumpState::input(bool grounded) {
    if(Controls::moveLeft() || Controls::moveRight()) {
        PlayerState::currentState = walkState;
    } if(Controls::jump()) {
        if(grounded)
        PlayerState::currentState = jumpState;
    }
}
